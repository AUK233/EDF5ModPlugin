#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>

#include <Shlwapi.h>
#include <HookLib.h>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

#include "proxy.h"
#include "PluginAPI.h"
#include "LoggerTweaks.h"

#include "utiliy.h"

extern PBYTE hmodEXE;
extern HANDLE handleEXE;
extern "C" extern int ModLogStatus;

static std::vector<void *> hooks; // Holds all original hooked functions
static const char hmodGameName[] = "EDF5.exe";

// Hook wrapper functions
BOOLEAN __fastcall SetHookWrap(const void *Interceptor, void **Original) {
	if (Original != NULL && *Original != NULL && SetHook(*Original, Interceptor, Original)) {
		hooks.push_back(*Original);
		return true;
	} else {
		return false;
	}
}

void SetupHook(uintptr_t offset, void **func, void *hook, const char *reason, BOOL active) {
	if (active) {
		// PLOG_INFO << "Hooking " << hmodGameName << "+" << std::hex << offset << " (" << reason << ")";
		*func = hmodEXE + offset;
		if (!SetHookWrap(hook, func)) {
			// Error
			PLOG_ERROR << "Failed to setup " << hmodGameName << "+" << std::hex << offset << " hook";
		}
	} else {
		// PLOG_INFO << "Skipping " << hmodGameName << "+" << std::hex << offset << " hook (" << reason << ")";
	}
}

// remove hook
BOOLEAN __fastcall RemoveHookWrap(void *Original) {
	if (Original != NULL) {
		std::vector<void *>::iterator position = std::find(hooks.begin(), hooks.end(), Original);
		if (position != hooks.end()) {
			if (RemoveHook(Original)) {
				hooks.erase(position);
				return true;
			}
		}
	}
	return false;
}

void RemoveAllHooks(void) {
	for (std::vector<void *>::iterator it = hooks.begin(); it != hooks.end();) {
		void *hook = *it;
		if (RemoveHook(hook)) {
			it = hooks.erase(it);
		} else {
			// hook is HOOK_DATA->OriginalBeginning
			// hook-16 is HOOK_DATA->OriginalFunction
			// TODO: Fork HookLib and exposde HOOK_DATA or add function to retrieve original address
			PVOID address = *((PVOID *)hook - 16 / sizeof(PVOID));

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


bool get_module_bounds(const std::wstring name, uintptr_t *start, uintptr_t *end)
{
	const auto module = GetModuleHandle(name.c_str());
	if (module == nullptr)
		return false;

	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(info));
	*start = (uintptr_t)(info.lpBaseOfDll);
	*end = *start + info.SizeOfImage;
	return true;
}

// Scan for a byte pattern with a mask in the form of "xxx???xxx".
uintptr_t sigscan(const std::wstring name, const char *sig, const char *mask)
{
	uintptr_t start, end;
	if (!get_module_bounds(name, &start, &end))
		throw std::runtime_error("Module not loaded");

	const auto last_scan = end - strlen(mask) + 1;

	for (auto addr = start; addr < last_scan; addr++) {
		for (size_t i = 0;; i++) {
			if (mask[i] == '\0')
				return addr;
			if (mask[i] != '?' && sig[i] != *(char *)(addr + i))
				break;
		}
	}

	return NULL;
}

uintptr_t GetPointerAddress(const uintptr_t base, std::initializer_list<int> offsets) {
	uintptr_t out = base;
	const int *it = offsets.begin();
	for (int i = 0; i < offsets.size(); i++) {
		out = *(uintptr_t *)(out + *(it + i));
		if (out == 0) {
			return 0;
		}
	}
	return out;
}

void *AllocatePageNearAddress(void *targetAddr) {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	const uint64_t PAGE_SIZE = sysInfo.dwPageSize;

	uint64_t startAddr = (uint64_t(targetAddr) & ~(PAGE_SIZE - 1)); // round down to nearest page boundary
	uint64_t minAddr = min(startAddr - 0x7FFFFF00, (uint64_t)sysInfo.lpMinimumApplicationAddress);
	uint64_t maxAddr = max(startAddr + 0x7FFFFF00, (uint64_t)sysInfo.lpMaximumApplicationAddress);

	uint64_t startPage = (startAddr - (startAddr % PAGE_SIZE));

	uint64_t pageOffset = 1;
	while (1) {
		uint64_t byteOffset = pageOffset * PAGE_SIZE;
		uint64_t highAddr = startPage + byteOffset;
		uint64_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

		bool needsExit = highAddr > maxAddr && lowAddr < minAddr;

		if (highAddr < maxAddr) {
			void *outAddr = VirtualAlloc((void *)highAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (outAddr)
				return outAddr;
		}

		if (lowAddr > minAddr) {
			void *outAddr = VirtualAlloc((void *)lowAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
			if (outAddr != nullptr)
				return outAddr;
		}

		pageOffset++;

		if (needsExit) {
			break;
		}
	}

	return nullptr;
}

// Injects hook into game process
void __fastcall WriteHookToProcess(void *addr, void *data, size_t len) {
	DWORD oldProtect;
	VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(addr, data, len);
	VirtualProtect(addr, len, oldProtect, &oldProtect);
	// Refresh cpu instruction?
	//FlushInstructionCache(handleEXE, addr, len);
}

void __fastcall ThrowsProblemAddressInformation(void* addr)
{
	uintptr_t rva = (uintptr_t)addr - (uintptr_t)hmodEXE;
	if (ModLogStatus == 1) {
		PLOG_ERROR << "Wrong address: +0x" << std::hex << rva;
	} else {
		std::wstring message = L"Wrong address: +0x";
		message += std::format(L"{:0X}", rva);

		HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ForceCrashGame, NULL, NULL, NULL);
		if (tempHND) {
			CloseHandle(tempHND);
		}

		MessageBoxW(NULL, message.c_str(), L"warning", MB_OK);
	}
}

// This will check if the previous address is ecx
void __fastcall WriteHookToProcessCheckECX(void* addr, void* data, size_t len)
{
	if (*((BYTE*)addr - 1) == 0xB9) {
		return WriteHookToProcess(addr, data, len);
	}
	else {
		return ThrowsProblemAddressInformation(addr);
	}
}

// This will check if the previous address is edx
void __fastcall WriteHookToProcessCheckEDX(void* addr, void* data, size_t len)
{
	if (*((BYTE*)addr - 1) == 0xBA) {
		return WriteHookToProcess(addr, data, len);
	}
	else {
		return ThrowsProblemAddressInformation(addr);
	}
}

// update game's original functions
void __fastcall hookGameBlock(void *targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
	    0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
	    0xFF, 0xE0                                                  // jmp rax
	};
	memcpy(&hookFunction[2], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

void __fastcall hookGameBlockRAXWithInt3(void* targetAddr, uint64_t dataAddr)
{
	uint8_t hookFunction[] = {
		   0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
		   0xFF, 0xE0, // jmp rax
		   0xCC        // int3
	};
	memcpy(&hookFunction[2], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// update game's original functions with 14 bytes
void __fastcall hookGameBlock14(void* targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,            // jmp
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // addr
	};
	memcpy(&hookFunction[6], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// update game's original functions with interruption, need 15 bytes
void __fastcall hookGameBlockWithInt3(void* targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// jmp
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // addr
		0xCC											// int3
	};
	memcpy(&hookFunction[6], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// update game's original functions with call, need 12bytes
void __fastcall CallGameBlock(void* targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
		0xFF, 0xD0                                                  // call rax
	};
	memcpy(&hookFunction[2], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// Search the address of the target
intptr_t __fastcall SundaySearch(const byte *target, int tLen, const byte *pattern, int pLen) {
	const int SHIFT_SIZE = 0x100;
	byte shift[SHIFT_SIZE] = {0};

	memset(shift, pLen + 1, SHIFT_SIZE);
	for (int i = 0; i < pLen; i++)
		shift[pattern[i]] = pLen - i;

	for (int i = 0; i < tLen - pLen; i += shift[target[i + pLen]]) {
		for (int j = 0; j < pLen; j++) {
			if (target[i + j] != pattern[j])
				break;
			if (j == pLen - 1)
				return i;
		}
	}

	return -1;
}

intptr_t __fastcall ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t minAddr, uintptr_t maxAddr) {
	const int MEM_SIZE = 0x1000;
	byte mem[MEM_SIZE] = {0};

	MEMORY_BASIC_INFORMATION mbi;
	for (uintptr_t curAddress = minAddr; curAddress < maxAddr; curAddress += mbi.RegionSize) {
		VirtualQueryEx(hProcess, (void *)curAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		if ((intptr_t)mbi.RegionSize <= 0)
			break;
		if (mbi.State != MEM_COMMIT)
			continue;
		// if (mbi.Protect != PAGE_READONLY && mbi.Protect != PAGE_READWRITE) continue;

		for (intptr_t memIndex = 0; memIndex < (intptr_t)mbi.RegionSize / MEM_SIZE; memIndex++) {
			uintptr_t beginAddress = curAddress + memIndex * MEM_SIZE;
			ReadProcessMemory(hProcess, (void *)(beginAddress), mem, MEM_SIZE, 0);
			intptr_t offset = SundaySearch(mem, MEM_SIZE, pattern, pLen);
			if (offset >= 0)
				return beginAddress + offset;
		}
	}

	return -1;
}

// Search the address of the target
intptr_t __fastcall ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t addr) {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	return ScanPattern(hProcess, pattern, pLen, addr, (uint64_t)sysInfo.lpMaximumApplicationAddress);
}

// Force the game to end if the user is not responding for a long time
void __fastcall ForceCrashGame()
{
	Sleep(60000);
	*(int*)0 = 0;
}
