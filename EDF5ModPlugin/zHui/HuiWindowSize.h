#pragma once
#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")

void module_UpdateHuiWindowSize(PBYTE hmodEXE);

extern "C" {
	void __fastcall CustomAdjustWindowRect(LPRECT lpRect);
}
