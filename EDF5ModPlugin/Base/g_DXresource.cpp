#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "g_DXresource.h"

extern "C" {
	uintptr_t pOutRenderColor1259640, vedf1259640; // EDF5.exe+1259640

	uintptr_t vedf1259680; // EDF5.exe+1259680
	PGameRenderer1259680* pGameRenderer1259680;

	PG_Application* pApplication1253708; // EDF5.exe+1253708
	PG_TextureBackBuffer* pTextureBackBuffer1256CB8; // EDF5.exe+1256CB8
}

void __fastcall XGS_DXresource_Initialize(PBYTE hmodEXE) {
	uintptr_t tempP;
	vedf1259640 = (uintptr_t)(hmodEXE + 0x1259640);
	pOutRenderColor1259640 = vedf1259640;

	vedf1259680 = (uintptr_t)(hmodEXE + 0x1259680);
	pGameRenderer1259680 = (PGameRenderer1259680*)vedf1259680;

	tempP = (uintptr_t)(hmodEXE + 0x1253708);
	pApplication1253708 = (PG_Application*)tempP;

	tempP = (uintptr_t)(hmodEXE + 0x1256CB8);
	pTextureBackBuffer1256CB8 = (PG_TextureBackBuffer*)tempP;
}

PGameRenderer1259680 __fastcall DXGI_GetGameRenderer1259680() {
	return *pGameRenderer1259680;
}

PG_Application __fastcall DXGI_GetApplication1253708() {
	return *pApplication1253708;
}

PG_TextureBackBuffer __fastcall DXGI_GetTextureBackBuffer1256CB8() {
	return *pTextureBackBuffer1256CB8;
}
