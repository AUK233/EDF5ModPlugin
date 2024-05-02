#pragma once

// Hook wrapper functions
BOOLEAN __fastcall SetHookWrap(const void *Interceptor, void **Original);
void SetupHook(uintptr_t offset, void **func, void *hook, const char *reason, BOOL active);// remove hook
BOOLEAN __fastcall RemoveHookWrap(void *Original);
void RemoveAllHooks(void);
bool get_module_bounds(const std::wstring name, uintptr_t *start, uintptr_t *end);
// Scan for a byte pattern with a mask in the form of "xxx???xxx".
uintptr_t sigscan(const std::wstring name, const char *sig, const char *mask);
uintptr_t GetPointerAddress(const uintptr_t base, std::initializer_list<int> offsets);
void *AllocatePageNearAddress(void *targetAddr);
// Injects hook into game process
void __fastcall WriteHookToProcess(void *addr, void *data, size_t len);
void __fastcall ThrowsProblemAddressInformation(void* addr);
// This will check if the previous address is ecx
void __fastcall WriteHookToProcessCheckECX(void* addr, void* data, size_t len);
// This will check if the previous address is edx
void __fastcall WriteHookToProcessCheckEDX(void* addr, void* data, size_t len);
// update game's original functions, need 12 bytes
void __fastcall hookGameBlock(void *targetAddr, uint64_t dataAddr);
// update game's original functions use RAX with interruption, need 13 bytes
void __fastcall hookGameBlockRAXWithInt3(void* targetAddr, uint64_t dataAddr);
// update game's original functions with 14 bytes
void __fastcall hookGameBlock14(void *targetAddr, uint64_t dataAddr);
// update game's original functions with interruption, need 15 bytes
void __fastcall hookGameBlockWithInt3(void* targetAddr, uint64_t dataAddr);
// update game's original functions with call, need 12bytes
void __fastcall CallGameBlock(void* targetAddr, uint64_t dataAddr);
// Search the address of the target
intptr_t __fastcall SundaySearch(const byte *target, int tLen, const byte *pattern, int pLen);
// Search the address of the target
intptr_t __fastcall ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t minAddr, uintptr_t maxAddr);
// Search the address of the target
intptr_t __fastcall ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t addr);
// Force the game to end if the user is not responding for a long time
void __fastcall ForceCrashGame();
