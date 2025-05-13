#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#define PLOG_OMIT_LOG_DEFINES
//#define PLOG_EXPORT

#include <vector>
#include <cstdio>

#include <Windows.h>
#include <shlwapi.h>
#include <HookLib.h>
#include <memory.h>
#include <string.h>
#include <format>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

#include "proxy.h"
#include "PluginAPI.h"
#include "LoggerTweaks.h"

#include "GameFunctionInASM.h"
#include "utiliy.h"
#include "GameFunc.h"
#include "GameFuncSetup.h"
#include "GameFuncStartup.h"
#include "GameFunc_vftable.h"
#include "GFnDisplay.h"

typedef struct {
	PluginInfo *info;
	void *module;
} PluginData;

static std::vector<PluginData*> plugins; // Holds all plugins loaded
typedef bool (__fastcall *LoadDef)(PluginInfo*);

// Called during initialization of CRT
typedef void* (__fastcall *initterm_func)(void*, void*);
static initterm_func initterm_orig;
// Handles opening files from disk or through CRI File System
typedef void* (__fastcall *fnk244d0_func)(void*, void*, void*);
static fnk244d0_func fnk244d0_orig;
// Puts wide string in weird string structure
typedef void* (__fastcall *fnk27380_func)(void*, const wchar_t*, unsigned long long);
static fnk27380_func fnk27380_orig;
// printf-like logging stub, usually given wide strings, sometimes normal strings
typedef void (__fastcall *gamelog_func)(const wchar_t*);
static gamelog_func gamelog_orig;

// Verify PluginData->module can store a HMODULE
static_assert(sizeof(HMODULE) == sizeof(PluginData::module), "module field cannot store an HMODULE");

// Minor utility functions
static inline BOOL FileExistsW(LPCWSTR szPath) {
	DWORD dwAttrib = GetFileAttributesW(szPath);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

static BOOL GetPrivateProfileBoolW(LPCWSTR lpAppName, LPCWSTR lpKeyName, BOOL bDefault, LPCWSTR lpFileName) {
	WCHAR boolStr[6];
	DWORD strlen = GetPrivateProfileStringW(lpAppName, lpKeyName, bDefault ? L"true" : L"false", boolStr, _countof(boolStr), lpFileName);
	return (CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, boolStr, strlen, L"true", 4) == CSTR_EQUAL);
}

template <size_t N>
constexpr size_t cwslen(wchar_t const (&)[N]) {
	return N - 1;
}

#define wcsstart(a, b) (!wcsncmp(a, b, cwslen(b)))

// Calculate ini path
WCHAR iniPath[MAX_PATH];
// Configuration
static UINT ModLog = 0;
static UINT WEOpen = 1;
static UINT RTRead = 0;
static UINT DisplayDamage = 1;
static UINT PlayerView = 0;
static UINT HUDEnhance = 0;
static UINT DisplaySubtitle = 0;
// Old configuration
static BOOL RedirectRead = TRUE;
static BOOL LoadPluginsB = FALSE;
static BOOL GameLog = FALSE;

extern "C" {
int playerViewIndex = 0;
int displayDamageIndex = 0;
int ModLogStatus = 0;
int HUDEnhanceStatus = 0;
int displaySubtitleOn = 0;
UINT noThrowAnime = 0;
UINT newSaveDataUnlock = 0;
}
//HANDLE ddThread;
int weaponEnhance = 0;

// Pointer sets
typedef struct {
	uintptr_t offset;
	const wchar_t *search;
	const char *ident;
	const char *plugfunc;
	uintptr_t pointers[4];
} PointerSet;

//int pointerSet = -1;
PointerSet psets[1] = { //
	{0xebcbd0, L"EarthDefenceForce 5 for PC", "EDF5", "EML5_Load", {0x9c835a, 0x244d0, 0x27380, 0x27680}},
};

// Search and load all *.dll files in 1mod\Plugins\ folder
static void LoadPlugins(void) {
	WIN32_FIND_DATAW ffd;
	PLOG_INFO << "Loading plugins";
	HANDLE hFind = FindFirstFileW(L"1mod\\Plugins\\*.dll", &ffd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				PLOG_INFO << "Loading Plugin: " << ffd.cFileName;
				wchar_t plugpath[MAX_PATH];
				wcscpy_s(plugpath, L"1mod\\Plugins\\");
				wcscat_s(plugpath, ffd.cFileName);
				HMODULE plugin = LoadLibraryW(plugpath);
				if (plugin != NULL) {
					LoadDef loadfunc = (LoadDef)GetProcAddress(plugin, psets[0].plugfunc);
					bool unload = false;
					if (loadfunc != NULL) {
						PluginInfo *pluginInfo = new PluginInfo();
						pluginInfo->infoVersion = 0;
						if (loadfunc(pluginInfo)) {
							// Validate PluginInfo
							if (pluginInfo->infoVersion == 0) {
								PLOG_ERROR << "PluginInfo infoVersion 0, expected " << PluginInfo::MaxInfoVer;
								unload = true;
							} else if (pluginInfo->name == NULL) {
								PLOG_ERROR << "Plugin missing name";
								unload = true;
							} else if (pluginInfo->infoVersion > PluginInfo::MaxInfoVer) {
								PLOG_ERROR << "Plugin has unsupported infoVersion " << pluginInfo->infoVersion << " expected " << PluginInfo::MaxInfoVer;
								unload = true;
							} else {
								switch (pluginInfo->infoVersion) {
								case 1:
								default:
									// Latest info version
									PluginData *pluginData = new PluginData;
									pluginData->info = pluginInfo;
									pluginData->module = plugin;
									plugins.push_back(pluginData);
									break;
								}
								static_assert(PluginInfo::MaxInfoVer == 1, "Supported version changed, update version handling and this number");
							}
						} else {
							PLOG_INFO << "Unloading plugin";
							unload = true;
						}
						if (unload) {
							delete pluginInfo;
						}
					} else {
						PLOG_WARNING << "Plugin does not contain " << psets[0].plugfunc << " function";
						unload = true;
					}
					if (unload) {
						FreeLibrary(plugin);
					}
				} else {
					DWORD dwError = GetLastError();
					PLOG_ERROR << "Failed to load plugin: error " << dwError;
				}
			}
		} while (FindNextFileW(hFind, &ffd) != 0);
		// Check if finished with error
		DWORD dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES) {
			PLOG_ERROR << "Failed to search for plugins: error " << dwError;
		}
		FindClose(hFind);
	} else {
		DWORD dwError = GetLastError();
		if (dwError != ERROR_FILE_NOT_FOUND && dwError != ERROR_PATH_NOT_FOUND) {
			PLOG_ERROR << "Failed to search for plugins: error " << dwError;
		}
	}
}

struct oddstr {
	wchar_t *str;
	void *unk;
	size_t unk2;
	size_t length;
};
static_assert(sizeof(oddstr) == 32, "Weird string structure should have a length of 32");

static void *__fastcall fnk244d0_hook(void *unk1, oddstr *str, void *unk2) {
	// First 8 bytes are a pointer if length >= 8
	// Otherwise string is stored where pointer will be?
	wchar_t *path = (wchar_t*)str;
	if (str->length >= 8) {
		path = str->str;
	}
	if (path != NULL && str->length >= cwslen(L"/cri_bind/") && wcsstart(path, L"/cri_bind/")) {
		size_t newlen = str->length + cwslen(L"./1mod/") - cwslen(L"/cri_bind/");
		wchar_t *modpath = new wchar_t[newlen + 1];
		wcscpy(modpath, L"./1mod/");
		wmemcpy(modpath + cwslen(L"./1mod/"), path + cwslen(L"/cri_bind/"), str->length - cwslen(L"/cri_bind/"));
		modpath[newlen] = L'\0';
		PLOG_DEBUG << "Checking for " << modpath;
		if (FileExistsW(modpath)) {
			PLOG_DEBUG << "Redirecting access to " << modpath;
			fnk27380_orig(str, modpath, newlen);
		}
		delete[] modpath;
	}
	return fnk244d0_orig(unk1, str, unk2);
}

// Internal logging hook
extern "C" {
void __fastcall gamelog_hook(const wchar_t *fmt, ...);   // wrapper to preserve registers
void __fastcall gamelog_hook_main(const char *fmt, ...); // actual logging implementaton

// Thread Local Storage to preserve/restore registers in wrapper
// TODO: Move all of this to winmm.asm
__declspec(thread) UINT64 save_ret;
__declspec(thread) UINT64 save_rax;
__declspec(thread) UINT64 save_rcx;
__declspec(thread) UINT64 save_rdx;
__declspec(thread) UINT64 save_r8;
__declspec(thread) UINT64 save_r9;
__declspec(thread) UINT64 save_r10;
__declspec(thread) UINT64 save_r11;

__declspec(thread) M128A save_xmm0;
__declspec(thread) M128A save_xmm1;
__declspec(thread) M128A save_xmm2;
__declspec(thread) M128A save_xmm3;
__declspec(thread) M128A save_xmm4;
__declspec(thread) M128A save_xmm5;
}

void __fastcall gamelog_hook_main(const char *fmt, ...) {
	if (fmt != NULL) {
		va_list args;
		va_start(args, fmt);
		if (fmt[0] == 'L' && fmt[1] == '\0' && !wcscmp((wchar_t*)fmt, L"LoadComplete:%s %s %d\n")) {
			// This wide string is formatted with normal strings
			fmt = "LoadComplete:%s %s %d";
		}
		// This is sometimes called with wide strings and normal strings
		// Try to automatically detect
		if (fmt[0] != '\0' && fmt[1] == '\0') {
			int required = _vsnwprintf(NULL, 0, (wchar_t*)fmt, args);
			wchar_t *buffer = new wchar_t[(size_t)required + 1];
			_vsnwprintf(buffer, (size_t)required + 1, (wchar_t*)fmt, args);
			va_end(args);
			// Remove new line from end of message if present
			if (required >= 1 && buffer[required - 1] == L'\n') {
				buffer[required - 1] = L'\0';
			}
			PLOG_INFO_(1) << buffer;
			delete[] buffer;
		} else {
			int required = _vsnprintf(NULL, 0, fmt, args);
			char *buffer = new char[(size_t)required + 1];
			_vsnprintf(buffer, (size_t)required + 1, fmt, args);
			va_end(args);
			// See above comment
			if (required >= 1 && buffer[required - 1] == '\n') {
				buffer[required - 1] = '\0';
			}
			PLOG_INFO_(1) << buffer;
			delete[] buffer;
		}
	} else {
		PLOG_INFO_(1) << "(null)";
	}
}

// Names for the log formatter
static const char ModLoaderStr[] = "ModPlugin";

PBYTE hmodEXE;
PBYTE hModSelf;
HANDLE handleEXE;
char hmodName[MAX_PATH];

void ReadINIconfig() {
	// Read configuration
	RTRead = GetPrivateProfileIntW(L"ModOption", L"RTRead", 0, iniPath);

	// playerViewIndex = PlayerView * 4
	// It means starting with N th data in ptr data
	// It is used in ASM, so it must be observed
	PlayerView = GetPrivateProfileIntW(L"ModOption", L"PlayerView", 0, iniPath);
	switch (PlayerView) {
	case 1: {
		if (playerViewIndex != 4) {
			playerViewIndex = 4;

			if (ModLogStatus == 1) {
				PLOG_INFO << "Set close over-the-shoulder view (left)";
			}
		}
		break;
	}
	case 2: {
		if (playerViewIndex != 8) {
			playerViewIndex = 8;

			if (ModLogStatus == 1) {
				PLOG_INFO << "Set long-range over-the-shoulder view (left)";
			}
		}
		break;
	}
	case 3: {
		if (playerViewIndex != 12) {
			playerViewIndex = 12;

			if (ModLogStatus == 1) {
				PLOG_INFO << "Set close over-the-shoulder view (right)";
			}
		}
		break;
	}
	case 4: {
		if (playerViewIndex != 16) {
			playerViewIndex = 16;

			if (ModLogStatus == 1) {
				PLOG_INFO << "Set long-range over-the-shoulder view (right)";
			}
		}
		break;
	}
	default:
		if (playerViewIndex != 0) {
			playerViewIndex = 0;

			if (ModLogStatus == 1) {
				PLOG_INFO << "Set original view";
			}
		}
		break;
	}

	// display damage
	DisplayDamage = GetPrivateProfileIntW(L"ModOption", L"DisplayDamage", 0, iniPath);
	if (DisplayDamage) {
		displayDamageIndex = 1;
		if (ModLogStatus == 1) {
			PLOG_INFO << "Display damage number on weapon (with charge)";
		}
	}
	else {
		displayDamageIndex = 0;
		if (ModLogStatus == 1) {
			PLOG_INFO << "Unable to display damage number";
		}
	}

	// display subtitle
	DisplaySubtitle = GetPrivateProfileIntW(L"ModOption", L"DisplaySubtitle", 0, iniPath);
	if (DisplaySubtitle) {
		displaySubtitleOn = 1;
		if (ModLogStatus == 1) {
			PLOG_INFO << "Enable display subtitles";
		}
	} else {
		displaySubtitleOn = 0;
		if (ModLogStatus == 1) {
			PLOG_INFO << "Disable display subtitles";
		}
	}
	// End
}

void WINAPI ReadINILoop() {
	// wait 30s
	Sleep(30000);

	while (RTRead) {
		ReadINIconfig();

		// once per second
		Sleep(1000);
	}

	PLOG_INFO << "Turn off real-time read profiles";
}

// Early hook into game process
static void *__fastcall initterm_hook(void *unk1, void *unk2) {
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
		if (ModLogStatus == 1) {
			PLOG_INFO << "Additional initialization";
		}

		// Load new function
		// Very important!!!!!!!!!!!!
		GetGameFunctions();
		hookGameFunctionsC();
		hookGameFunctions();
		CreateNewVirtualTables((uintptr_t)hmodEXE);
		// Read config
		ReadINIconfig();
		// Now inject only when needed, for crash rate reduction

		if (HUDEnhance || RTRead) {
			hookHUDEnhancement();
		}

		// It needs to be right here
		if (RTRead) {
			HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadINILoop, NULL, NULL, NULL);
			if (tempHND) {
				CloseHandle(tempHND);
			}

			if (ModLogStatus == 1) {
				PLOG_INFO << "Enable real-time read profiles";
			}
		}

		// Load plugins
		if (LoadPluginsB) {
			LoadPlugins();
		}

		if (ModLogStatus == 1) {
			PLOG_INFO << "Initialization finished";
		}
	}

	return initterm_orig(unk1, unk2);
}

// x64 cannot use inline assembly, you have to create asm files.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	static plog::RollingFileAppender<eml::TxtFormatter<ModLoaderStr>> mlLogOutput("1Mod.log");
	static plog::RollingFileAppender<eml::TxtFormatter<nullptr>> gameLogOutput("1game.log");

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		// For optimization
		DisableThreadLibraryCalls(hModule);

		// Calculate ini path
		GetModuleFileNameW(hModule, iniPath, _countof(iniPath));
		PathRemoveFileSpecW(iniPath);
		wcscat_s(iniPath, L"\\1ModOption.ini");

		// Read configuration
		ModLog = GetPrivateProfileIntW(L"ModOption", L"ModLog", ModLog, iniPath);
		WEOpen = GetPrivateProfileIntW(L"ModOption", L"EnhancedWP", 1, iniPath);
		HUDEnhance = GetPrivateProfileIntW(L"ModOption", L"HUDEnhance", 0, iniPath);
		noThrowAnime = GetPrivateProfileIntW(L"ModOption", L"NoThrowAnime", 0, iniPath);
		newSaveDataUnlock = GetPrivateProfileIntW(L"ModOption", L"StarterKit", 0, iniPath);
		//LoadPluginsB = GetPrivateProfileBoolW(L"ModOption", L"LoadPlugins", LoadPluginsB, iniPath);
		//Redirect = GetPrivateProfileBoolW(L"ModOption", L"Redirect", Redirect, iniPath);
		//GameLog = GetPrivateProfileBoolW(L"ModOption", L"GameLog", GameLog, iniPath);

		// Open Log file
		if (ModLog) {
			GameLog = TRUE;
			ModLogStatus = 1;
			DeleteFileW(L"1Mod.log");
#ifdef NDEBUG
			plog::init(plog::info, &mlLogOutput);
#else
			plog::init(plog::debug, &mlLogOutput);
#endif
		}
		// game log, but not now
		if (GameLog) {
			DeleteFileW(L"1game.log");
			plog::init<1>(plog::info, &gameLogOutput);
		}

		// Add ourself to plugin list for future reference
		PluginInfo *selfInfo = new PluginInfo;
		selfInfo->infoVersion = PluginInfo::MaxInfoVer;
		selfInfo->name = "EDF5 Mod Plugin";
		selfInfo->version = PLUG_VER(0, 4, 8, 0);
		PluginData *selfData = new PluginData;
		selfData->info = selfInfo;
		selfData->module = hModule;
		plugins.push_back(selfData);

		handleEXE = GetCurrentProcess();
		hmodEXE = (PBYTE)GetModuleHandleW(NULL);
		// Determine what game is hosting us
		/*
		GetModuleFileNameA((HMODULE)hmodEXE, hmodName, _countof(hmodName));
		char *hmodFName = PathFindFileNameA(hmodName);
		memmove(hmodName, hmodFName, strlen(hmodFName) + 1);
		for (int i = 0; i < _countof(psets); i++) {
			size_t search_len = wcslen(psets[i].search);
			if (!IsBadReadPtr(hmodEXE + psets[i].offset, search_len+1) && !wcsncmp((wchar_t*)(hmodEXE + psets[i].offset), psets[i].search, search_len)) {
				pointerSet = i;
				break;
			}
		}
		if (pointerSet == -1) {
			PLOG_ERROR << "Failed to determine what exe is running";
			return FALSE;
		}
		uintptr_t *pointers = psets[pointerSet].pointers;
		*/
		uintptr_t *pointers = psets[0].pointers;

		PluginVersion v = selfInfo->version;
		if (ModLogStatus == 1) {
			PLOG_INFO.printf("ModPlugin (%s) v%u.%u.%u.%u Initializing\n", psets[0].ident, v.major, v.minor, v.patch, v.build);
		}

		// Setup DLL proxy
		wchar_t path[MAX_PATH];
		if (!GetWindowsDirectoryW(path, _countof(path))) {
			DWORD dwError = GetLastError();
			if (ModLogStatus == 1) {
				PLOG_ERROR << "Failed to get windows directory path: error " << dwError;
			}
			return FALSE;
		}

		wcscat_s(path, L"\\System32\\winmm.dll");

		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading real winmm.dll";
			PLOG_INFO << "Setting up dll proxy functions";
		}
		setupFunctions(LoadLibraryW(path));

		// Create ModLoader folders
		//CreateDirectoryW(L"1Mod", NULL);
		//CreateDirectoryW(L"1Mod\\Plugins", NULL);

		//
		hModSelf = (PBYTE)GetModuleHandleW(L"1mod.dll");
		if (ModLogStatus == 1) {
			PLOG_INFO << "Get self address: " << std::hex << hModSelf;
		}
		// Weapon Enhancement
		if (WEOpen) {
			weaponEnhance = 1;

			if (ModLogStatus == 1) {
				PLOG_INFO << "Enable weapon enhancement";
			}
		}
		else {
			if (ModLogStatus == 1) {
				PLOG_INFO << "Disable weapon enhancement";
			}
		}

		// Restrict the cpu number used by the application
		UINT CPULimit = GetPrivateProfileIntW(L"ModOption", L"LimitCPU", 0, iniPath);
		if (CPULimit > 3) {
			DWORD_PTR ProcessAffinityMask = 1;
			UINT isHT = GetPrivateProfileIntW(L"ModOption", L"HasHT", 0, iniPath);
			if (isHT == 0) {
				if (CPULimit >= 32) {
					ProcessAffinityMask = 0xFFFFFFFF;
					CPULimit = 32;
				} else {
					// Nth power of 2
					ProcessAffinityMask <<= CPULimit;
					ProcessAffinityMask -= 1;
				}

				if (ModLogStatus == 1) {
					PLOG_INFO << "Set the number of threads to be used to: " << CPULimit;
				}
			} else {
				if (CPULimit >= 16) {
					ProcessAffinityMask = 0b01010101010101010101010101010101;
					CPULimit = 16;
				} else {
					UINT CPUCore = CPULimit * 2;
					for (UINT i = 2; i < CPUCore; i += 2) {
						DWORD_PTR temp = 1Ui64 << i;
						ProcessAffinityMask += temp;
					}
				}

				if (ModLogStatus == 1) {
					PLOG_INFO << "Set the number of cores to be used to: " << CPULimit;
				}
			}

			SetProcessAffinityMask(GetCurrentProcess(), ProcessAffinityMask);
		}

		// Hook function for additional ModLoader initialization
		// offset is 0x9C775A
		SetupHook(pointers[0], (PVOID*)&initterm_orig, initterm_hook, "Additional initialization", TRUE);

		//void *memoryBlock = AllocatePageNearAddress(hmodEXE + 0x9c835a);
		//uintptr_t hookFunction = (uintptr_t)initterm_hook2;
		//memcpy(memoryBlock, (void *)hookFunction, 8U);
		//uint64_t relAddr = (uint64_t)memoryBlock - (uint64_t)(hmodEXE + 0x9c835a + 6);
		//(hmodEXE + 0x9c835a + 2, &relAddr, 4U);

		// Add Mods folder redirector hook
		fnk27380_orig = (fnk27380_func)((PBYTE)hmodEXE + pointers[2]);
		SetupHook(pointers[1], (PVOID*)&fnk244d0_orig, fnk244d0_hook, "Mods folder redirector", RedirectRead);

		// Add internal logging hook
		SetupHook(pointers[3], (PVOID*)&gamelog_orig, gamelog_hook, "Interal logging hook", GameLog);

		// Set startup behavior
		GameStartupInitialization(hmodEXE);
		// Provide basic equipment to new players
		if (newSaveDataUnlock) {
			GameStartupUnlock(hmodEXE);
			if (ModLogStatus == 1) {
				PLOG_INFO << "You will get all weapons and maximum mission limit armor values!";
			}
		}
		// End, change game title
		std::wstring GameTitle = L"EDF5 for PC in MOD Mode";
		WriteHookToProcess(hmodEXE + 0xebcbd0, (void *)GameTitle.c_str(), 48U);

		// Finished
		if (ModLogStatus == 1) {
			PLOG_INFO << "Basic initialization complete";
		}
		// timeBeginPeriod(1);
		//ftimeBeginPeriod(50);

		break;
	}
	case DLL_PROCESS_DETACH: {
		if (ModLogStatus == 1) {
			PLOG_INFO << "ModPlugin Unloading";

		// Remove hooks
			PLOG_INFO << "Removing hooks";
		}
		RemoveAllHooks();

		// Unload all plugins
		if (ModLogStatus == 1) {
			PLOG_INFO << "Unloading plugins";
		}
		for (PluginData *pluginData : plugins) {
			delete pluginData->info;
			if (pluginData->module != hModule) {
				FreeLibrary((HMODULE)(pluginData->module));
			}
			delete pluginData;
		}
		plugins.clear();

		// Unload real winmm.dll
		if (ModLogStatus == 1) {
			PLOG_INFO << "Unloading real winmm.dll";
		}
		cleanupProxy();

		// TODO: Close log file?
		break;
	}
	}
	return TRUE;
}
