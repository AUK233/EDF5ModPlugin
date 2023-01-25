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

#include "utiliy.h"
#include "GameFunc.h"

typedef struct {
	PluginInfo *info;
	void *module;
} PluginData;

static std::vector<PluginData*> plugins; // Holds all plugins loaded
typedef bool (__fastcall *LoadDef)(PluginInfo*);

static std::vector<void*> hooks; // Holds all original hooked functions

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

// Hook wrapper functions
BOOLEAN EDFMLAPI SetHookWrap(const void *Interceptor, void **Original) {
	if (Original != NULL && *Original != NULL && SetHook(*Original, Interceptor, Original)) {
		hooks.push_back(*Original);
		return true;
	} else {
		return false;
	}
}

BOOLEAN EDFMLAPI RemoveHookWrap(void *Original) {
	if (Original != NULL) {
		std::vector<void*>::iterator position = std::find(hooks.begin(), hooks.end(), Original);
		if (position != hooks.end()) {
			if (RemoveHook(Original)) {
				hooks.erase(position);
				return true;
			}
		}
	}
	return false;
}

static void RemoveAllHooks(void) {
	for (std::vector<void*>::iterator it = hooks.begin(); it != hooks.end();) {
		void *hook = *it;
		if (RemoveHook(hook)) {
			it = hooks.erase(it);
		} else {
			// hook is HOOK_DATA->OriginalBeginning
			// hook-16 is HOOK_DATA->OriginalFunction
			// TODO: Fork HookLib and exposde HOOK_DATA or add function to retrieve original address
			PVOID address = *((PVOID*)hook - 16 / sizeof(PVOID));

			HMODULE hmodDLL;
			wchar_t DLLName[MAX_PATH];
			GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCWSTR)address, &hmodDLL);
			GetModuleFileNameW(hmodDLL, DLLName, _countof(DLLName));
			PathStripPathW(DLLName);

			PLOG_ERROR << "Failed to remove " << DLLName << "+" << std::hex << ((ULONG_PTR)address - (ULONG_PTR)hmodDLL) << " hook";
			it++;
		}
	}
}

// Calculate ini path
WCHAR iniPath[MAX_PATH];
// Configuration
static UINT ModLog = 0;
static UINT RTRead = 0;
static UINT DisplayDamage = 1;
static UINT PlayerView = 0;
// Old configuration
static BOOL Redirect = FALSE;
static BOOL LoadPluginsB = FALSE;
static BOOL GameLog = FALSE;

extern "C" {
int playerViewIndex = 0;
}

// Pointer sets
typedef struct {
	uintptr_t offset;
	const wchar_t *search;
	const char *ident;
	const char *plugfunc;
	uintptr_t pointers[4];
} PointerSet;

int pointerSet = -1;
PointerSet psets[1] = { //
	{0xebcbd0, L"EarthDefenceForce 5 for PC", "EDF5", "EML5_Load", {0x9c835a, 0x244d0, 0x27380, 0x27680}}, // EDF 5, = old addr + 6144
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
					LoadDef loadfunc = (LoadDef)GetProcAddress(plugin, psets[pointerSet].plugfunc);
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
						PLOG_WARNING << "Plugin does not contain " << psets[pointerSet].plugfunc << " function";
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

// Early hook into game process
static void* __fastcall initterm_hook(void *unk1, void *unk2) {
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
		PLOG_INFO << "Additional initialization";

		// Load plugins
		if (LoadPluginsB) {
			LoadPlugins();
		} else {
			PLOG_INFO << "Plugin loading disabled";
		}

		PLOG_INFO << "Initialization finished";
	}
	return initterm_orig(unk1, unk2);
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
char hmodName[MAX_PATH];

void SetupHook(uintptr_t offset, void **func, void* hook, const char *reason, BOOL active) {
	if (active) {
		PLOG_INFO << "Hooking " << hmodName << "+" << std::hex << offset << " (" << reason << ")";
		*func = hmodEXE + offset;
		if (!SetHookWrap(hook, func)) {
			// Error
			PLOG_ERROR << "Failed to setup " << hmodName << "+" << std::hex << offset << " hook";
		}
	} else {
		PLOG_INFO << "Skipping " << hmodName << "+" << std::hex << offset << " hook (" << reason << ")";
	}
}


// Since the text is now under the radar, it is only necessary to find an address.
//std::vector<uintptr_t> damageStr1;
uintptr_t damageStrPos1 = 0;
static wchar_t str[] = L"Initialize:Damage1.....";
wchar_t nullStrDisplay[] = L"                      \0";

extern "C" {
void __fastcall ASMrecordPlayerDamage();
uintptr_t playerAddress;
uintptr_t hookRetAddress;
float damage_tmp = 0.0f;
}

HANDLE hProcess = GetCurrentProcess();

constexpr auto DAMAGE_DISPLAY_TIME = 60;
struct Damage {
	float value;
	int time;
};
Damage damageNumber;
//int gameTime = 0;
//uintptr_t damageStr1Do = 0;
BOOL DamageStringHide = 0;

void WINAPI hookWeapon() {
	// If the first address found is in this dll, this function is no longer executed.
	while (DamageStringHide == 0) {
		playerAddress = GetPointerAddress((uintptr_t)hmodEXE, {0x0125AB68, 0x238, 0x290, 0x10});

		if (playerAddress > 0) {
			//!damageStr1.size()
			if (damageStrPos1 < 1) {

				SYSTEM_INFO sysInfo;
				GetSystemInfo(&sysInfo);

				intptr_t oneScan = ScanPattern(hProcess, (byte *)&str, 48U, (uint64_t)sysInfo.lpMinimumApplicationAddress);
				if (oneScan > 0) {
					if (oneScan < (intptr_t)hModSelf) {
						intptr_t nextScan = ScanPattern(hProcess, (byte *)&str, 48U, (oneScan + 48U), (intptr_t)hModSelf);
						// Maybe that bug can be fixed
						intptr_t layoutScan = ScanPattern(hProcess, (byte *)L"Layout", 14U, nextScan, (intptr_t)hModSelf);
						if (nextScan > 0) {
							if (layoutScan - nextScan < 100) {
								PLOG_INFO << "Fake damage string address: " << std::hex << nextScan;
								nextScan = ScanPattern(hProcess, (byte *)&str, 48U, (nextScan + 48U), (intptr_t)hModSelf);
							}

							// Here has a bug
							if (nextScan > 0) {
								damageStrPos1 = nextScan;
							} else {
								damageStrPos1 = oneScan;
							}

							// memcpy((void *)damageStrPos1, &nullStrDisplay, 48U);
							PLOG_INFO << "Damage string address: " << std::hex << damageStrPos1;
						}
						// Remove split screen text
						if (DisplayDamage >= 2) {
							while (nextScan > 0) {
								nextScan = ScanPattern(hProcess, (byte *)&str, 48U, (nextScan + 48U), (intptr_t)hModSelf);
								// done
								if (nextScan > 0) {
									memcpy((void *)nextScan, &nullStrDisplay, 48U);
									PLOG_INFO << "Remove string address: " << std::hex << nextScan;
								}
							}
						}
						//
					} else {
						PLOG_INFO << "Find damage string address is failed, disable damage display: " << std::hex << oneScan;
						DamageStringHide++;
					}

					// This function is useless now
					/*
					damageStr1.push_back(oneScan);
					intptr_t nextScan = oneScan;
					while (nextScan > 0) {
					    scanAddr = nextScan + 0x40;
					    nextScan = ScanPattern(hProcess, (byte *)&str, 48U, scanAddr);
					    PLOG_INFO << "result: " << std::hex << nextScan;
					    if (nextScan > 0 && nextScan < (intptr_t)hModSelf) {
					        damageStr1.push_back(nextScan);
					    }
					}
					*/
				}
				/*
				if (damageStr1.size() > 2) {
				    damageStr1Do = damageStr1.size() - 2;
				    for (int i = damageStr1Do; i > 0; --i) {
				        memcpy((void *)damageStr1[i], &nullStrDisplay, 48U);
				    }
				}
				*/
			}

			// Run only if it > 0
			if (damageStrPos1 > 0) {
				if (damage_tmp != 0) {
					// gameTime - damageNumber.time > DAMAGE_DISPLAY_TIME
					if (damageNumber.time < 1) {
						damageNumber.value = -damage_tmp;
						// gameTime = 0;
					} else {
						damageNumber.value -= damage_tmp;
					}
					// Why +1, because the following will be immediately subtracted.
					damageNumber.time = DAMAGE_DISPLAY_TIME + 1;
					damage_tmp = 0;
				}

				// memcpy((void *)damageStr1[damageStr1Do], &nullStrDisplay, 48U);
				memcpy((void *)damageStrPos1, &nullStrDisplay, 48U);

				// damageStr1.size() > 2
				// gameTime - damageNumber.time < DAMAGE_DISPLAY_TIME
				// damageNumber.value > 0
				if (damageNumber.time > 0) {
					std::wstring displayText;
					if (damageNumber.value >= 100.0f) {
						displayText = std::format(L"{:.0f}", damageNumber.value);
					} else if (damageNumber.value >= 10.0f) {
						displayText = std::format(L"{:.1f}", damageNumber.value);
					} else {
						displayText = std::format(L"{:.2f}", damageNumber.value);
					}

					size_t strofs = 0;
					size_t strsize = 44;
					if (displayText.size() < 22) {
						strofs = (22 - displayText.size()) * 2;
						strsize = displayText.size() * 2;
					}
					// memcpy((void *)(damageStr1[damageStr1Do] + strofs), displayText.c_str(), strsize);
					memcpy((void *)(damageStrPos1 + strofs), displayText.c_str(), strsize);
					damageNumber.time--;
				}
			}
			//
		} else {
			if (damageStrPos1 > 0) {
				damageStrPos1 = 0;
				damageNumber.time = 0;
			}
			/*
			if (damageStr1.size() > 0) {
			    damageStr1.clear();
			    damageStr1.shrink_to_fit();
			}*/
		}

		//gameTime++;
		// 20 fps should be enough
		Sleep(50);
	}
}

void hookWeaponGUImain() {
	// First, load the sgo that we need
	void *RaderStringAddr = (void *)(sigscan(L"EDF5.exe", "l\0y\0t\0_\0H\0u\0d\0R\0a\0d\0e\0r", "xxxxxxxxxxxxxxxxxxxxxxx"));
	std::wstring newRader = L"lyt_HudRaderM1.sgo";
	WriteHookToProcess(RaderStringAddr, (void *)newRader.c_str(), 36U);

	// Then, get the damage
	void *originalFunctionAddr = (void *)(sigscan(L"EDF5.exe", "\xF3\x0F\x58\x87\xFC\x01\x00\x00", "xxxxxxxx"));
	hookRetAddress = (uint64_t)originalFunctionAddr + 0x8;

	void *memoryBlock = AllocatePageNearAddress(originalFunctionAddr);

	uint8_t hookFunction[] = {
	    0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
	    0xFF, 0xE0                                                  // jmp rax
	};
	uint64_t addrToJumpTo64 = (uint64_t)ASMrecordPlayerDamage;

	memcpy(&hookFunction[2], &addrToJumpTo64, sizeof(addrToJumpTo64));
	memcpy(memoryBlock, hookFunction, sizeof(hookFunction));

	uint8_t jmpInstruction[5] = {0xE9, 0x0, 0x0, 0x0, 0x0};
	const uint64_t relAddr = (uint64_t)memoryBlock - ((uint64_t)originalFunctionAddr + sizeof(jmpInstruction));
	memcpy(jmpInstruction + 1, &relAddr, 4);

	WriteHookToProcess(originalFunctionAddr, jmpInstruction, sizeof(jmpInstruction));
}

void ReadINIconfig() {
	// Read configuration
	PlayerView = GetPrivateProfileIntW(L"ModOption", L"PlayerView", PlayerView, iniPath);
	RTRead = GetPrivateProfileIntW(L"ModOption", L"RTRead", RTRead, iniPath);

	// playerViewIndex = PlayerView * 4
	// It means starting with N th data in ptr data
	// It is used in ASM, so it must be observed
	switch (PlayerView) {
	case 1: {
		if (playerViewIndex != 4) {
			playerViewIndex = 4;
			PLOG_INFO << "Set close over-the-shoulder view (left)";
		}
		break;
	}
	case 2: {
		if (playerViewIndex != 8) {
			playerViewIndex = 8;
			PLOG_INFO << "Set long-range over-the-shoulder view (left)";
		}
		break;
	}
	case 3: {
		if (playerViewIndex != 12) {
			playerViewIndex = 12;
			PLOG_INFO << "Set close over-the-shoulder view (right)";
		}
		break;
	}
	case 4: {
		if (playerViewIndex != 16) {
			playerViewIndex = 16;
			PLOG_INFO << "Set long-range over-the-shoulder view (right)";
		}
		break;
	}
	default:
		if (playerViewIndex != 0) {
			playerViewIndex = 0;
			PLOG_INFO << "Set original view";
		}
		break;
	}
}

void ReadINILoop() {
	// wait 30s
	Sleep(30000);

	while (RTRead) {
		ReadINIconfig();

		// once per second
		Sleep(1000);
	}

	PLOG_INFO << "Turn off real-time read profiles";
}

// x64 cannot use inline assembly, you have to create asm files.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	static plog::RollingFileAppender<eml::TxtFormatter<ModLoaderStr>> mlLogOutput("1Mod.log");
	static plog::RollingFileAppender<eml::TxtFormatter<nullptr>> gameLogOutput("game.log");

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
		DisplayDamage = GetPrivateProfileIntW(L"ModOption", L"DisplayDamage", DisplayDamage, iniPath);
		//LoadPluginsB = GetPrivateProfileBoolW(L"ModOption", L"LoadPlugins", LoadPluginsB, iniPath);
		//Redirect = GetPrivateProfileBoolW(L"ModOption", L"Redirect", Redirect, iniPath);
		//GameLog = GetPrivateProfileBoolW(L"ModOption", L"GameLog", GameLog, iniPath);

		// Open Log file
		if (ModLog) {
			DeleteFileW(L"1Mod.log");
#ifdef NDEBUG
			plog::init(plog::info, &mlLogOutput);
#else
			plog::init(plog::debug, &mlLogOutput);
#endif
		}
		// game log, but not now
		if (GameLog) {
			DeleteFileW(L"game.log");
			plog::init<1>(plog::info, &gameLogOutput);
		}

		// Add ourself to plugin list for future reference
		PluginInfo *selfInfo = new PluginInfo;
		selfInfo->infoVersion = PluginInfo::MaxInfoVer;
		selfInfo->name = "EDF5 Mod Plugin";
		selfInfo->version = PLUG_VER(0, 3, 9, 0);
		PluginData *selfData = new PluginData;
		selfData->info = selfInfo;
		selfData->module = hModule;
		plugins.push_back(selfData);

		// Determine what game is hosting us
		hmodEXE = (PBYTE)GetModuleHandleW(NULL);
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

		PluginVersion v = selfInfo->version;
		PLOG_INFO.printf("ModPlugin (%s) v%u.%u.%u Initializing\n", psets[pointerSet].ident, v.major, v.minor, v.patch);

		// Setup DLL proxy
		wchar_t path[MAX_PATH];
		if (!GetWindowsDirectoryW(path, _countof(path))) {
			DWORD dwError = GetLastError();
			PLOG_ERROR << "Failed to get windows directory path: error " << dwError;
			return FALSE;
		}

		wcscat_s(path, L"\\System32\\winmm.dll");

		PLOG_INFO << "Loading real winmm.dll";
		PLOG_INFO << "Setting up dll proxy functions";
		setupFunctions(LoadLibraryW(path));

		// Create ModLoader folders
		//CreateDirectoryW(L"1Mod", NULL);
		//CreateDirectoryW(L"1Mod\\Plugins", NULL);

		// Hook function for additional ModLoader initialization
		SetupHook(pointers[0], (PVOID*)&initterm_orig, initterm_hook, "Additional initialization", TRUE);

		// Add Mods folder redirector hook
		fnk27380_orig = (fnk27380_func)((PBYTE)hmodEXE + pointers[2]);
		SetupHook(pointers[1], (PVOID*)&fnk244d0_orig, fnk244d0_hook, "Mods folder redirector", Redirect);

		// Add internal logging hook
		SetupHook(pointers[3], (PVOID*)&gamelog_orig, gamelog_hook, "Interal logging hook", GameLog);

		hModSelf = (PBYTE)GetModuleHandleW(L"1mod.dll");
		PLOG_INFO << "Get self address: " << std::hex << hModSelf;

		// Add damage display
		if (DisplayDamage) {
			hookWeaponGUImain();
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)hookWeapon, NULL, NULL, NULL);
			PLOG_INFO << "Display damage number";
		} else {
			PLOG_INFO << "Unable to display damage number";
		}

		// Very important!!!!!!!!!!!!
		ReadINIconfig();
		// Very important!!!!!!!!!!!!
		hookGameFunctions();

		if (RTRead) {
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadINILoop, NULL, NULL, NULL);
			PLOG_INFO << "Enable real-time read profiles";
		}
		
		// End, change game title
		std::wstring GameTitle = L"EDF5 for PC in MOD Mode";
		WriteHookToProcess(hmodEXE + 0xebcbd0, (void *)GameTitle.c_str(), 48U);

		// Finished
		PLOG_INFO << "Basic initialization complete";

		break;
	}
	case DLL_PROCESS_DETACH: {
		PLOG_INFO << "ModPlugin Unloading";

		// Remove hooks
		PLOG_INFO << "Removing hooks";
		RemoveAllHooks();

		// Unload all plugins
		PLOG_INFO << "Unloading plugins";
		for (PluginData *pluginData : plugins) {
			delete pluginData->info;
			if (pluginData->module != hModule) {
				FreeLibrary((HMODULE)(pluginData->module));
			}
			delete pluginData;
		}
		plugins.clear();

		// Unload real winmm.dll
		PLOG_INFO << "Unloading real winmm.dll";
		cleanupProxy();

		// TODO: Close log file?
		break;
	}
	}
	return TRUE;
}
