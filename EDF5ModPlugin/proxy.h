#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void setupFunctions(HMODULE);
void cleanupProxy(void);

void __fastcall ftimeBeginPeriod(UINT uPeriod);
void __fastcall ftimeEndPeriod(UINT uPeriod);

#ifdef __cplusplus
} // extern "C"
#endif