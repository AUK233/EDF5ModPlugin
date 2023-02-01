#pragma once

// get game function address
void GetGameFunctions();
// here hook all changed functions, written in c++
void hookGameFunctionsC();

typedef void *(__fastcall *fnk391230_func)(uintptr_t);
static fnk391230_func fnk391230_orig;
static bool __fastcall fnk391230_hook(uintptr_t pweapon);
