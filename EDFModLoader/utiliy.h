#pragma once

bool get_module_bounds(const std::wstring name, uintptr_t *start, uintptr_t *end);
// Scan for a byte pattern with a mask in the form of "xxx???xxx".
uintptr_t sigscan(const std::wstring name, const char *sig, const char *mask);
uintptr_t GetPointerAddress(const uintptr_t base, std::initializer_list<int> offsets);
void *AllocatePageNearAddress(void *targetAddr);
// Injects hook into game process
void WriteHookToProcess(void *addr, void *data, size_t len);
// Search the address of the target
intptr_t SundaySearch(const byte *target, int tLen, const byte *pattern, int pLen);
// Search the address of the target
intptr_t ScanPattern(HANDLE hProcess, byte *pattern, int pLen, uintptr_t addr);