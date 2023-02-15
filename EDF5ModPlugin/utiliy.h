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
void WriteHookToProcess(void *addr, void *data, size_t len);
// update game's original functions
void hookGameBlock(void *targetAddr, uint64_t hookAddr);
// update game's original functions with 14 bytes
void hookGameBlock14(void *targetAddr, uint64_t hookAddr);
// Search the address of the target
intptr_t SundaySearch(const byte *target, int tLen, const byte *pattern, int pLen);
// Search the address of the target
intptr_t ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t minAddr, uintptr_t maxAddr);
// Search the address of the target
intptr_t ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t addr);
