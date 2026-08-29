#pragma once

void __fastcall ASFunc_Hook_Initialize(PBYTE hmodEXE);

extern "C" {
	void __fastcall ASFunc_Hook_ResetControllerAssign();
}
