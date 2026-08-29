#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>
#include <vector>
#include <d3dcompiler.h>

#include "commonNOP.h"
#include "utiliy.h"
#include "1FullAOData.hpp"
#include "1FullAO.h"

extern PBYTE hmodEXE;

ID3D11ComputeShader* pMainCSshader;

void HookFunction_D3D11_FullAO() {

	// Linear Depth buffer
	WriteHookToProcess((void*)(hmodEXE + 0x607327), (void*)&nop2, 2U);
	WriteHookToProcess((void*)(hmodEXE + 0x60732F), (void*)&nop2, 2U);
	WriteHookToProcess((void*)(hmodEXE + 0x60741F), (void*)&nop6, 6U);

	// yeah, ensure new CSO is less than original one
	static_assert(sizeof(FullAOShaderBin) <= 0x620);
	int newCSOSize = sizeof(FullAOShaderBin);
	WriteHookToProcess((void*)(hmodEXE + 0x607091+2), &newCSOSize, 4);
	WriteHookToProcess((void*)(hmodEXE + 0x1183A50), FullAOShaderBin, newCSOSize);

	// Calculate Linear Depth
	// EDF5.exe+6075D5 set cs shader
	BYTE dispatchX[] = {
		0xC1, 0xE8, 0x04,
		0x41, 0xF6, 0xC5, 0x0F
	};
	WriteHookToProcess((void*)(hmodEXE + 0x607591), dispatchX, 7);

	BYTE dispatchY[] = {
		0xC1, 0xEA, 0x04,
		0x41, 0xF6, 0xC4, 0x0F
	};
	WriteHookToProcess((void*)(hmodEXE + 0x6075AC), dispatchY, 7);

	// EDF5.exe+5D8163
	// update main color cs shader
	BYTE movR8 = 0x8B;

	int newMainCSOSize = sizeof(FullAOMainColorShader);
	WriteHookToProcess((void*)(hmodEXE + 0x5D815D + 2), &newMainCSOSize, 4);
	WriteHookToProcess((void*)(hmodEXE + 0x5D8163 + 1), &movR8, 1);
	uintptr_t newMainCSOAddr = (uintptr_t)FullAOMainColorShader;
	WriteHookToProcess((void*)(hmodEXE + 0x116E780), &newMainCSOAddr, 8);

	int newMainUGCSOSize = sizeof(FullAOMainColorShaderUG);
	WriteHookToProcess((void*)(hmodEXE + 0x5D817D + 2), &newMainUGCSOSize, 4);
	WriteHookToProcess((void*)(hmodEXE + 0x5D8183 + 1), &movR8, 1);
	uintptr_t newMainUGCSOAddr = (uintptr_t)FullAOMainColorShaderUG;
	WriteHookToProcess((void*)(hmodEXE + 0x1165260), &newMainUGCSOAddr, 8);
}

void HookFunction_D3D11_FullAO(ID3D11Device* device) {
	ID3DBlob* cs_blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	D3DCompileFromFile(L"D:\\9SourceCode\\cpp\\EDF5ModPlugin\\Document\\backup\\MainColorCS.hlsl", nullptr, nullptr, "CS_main", "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &cs_blob, &error_blob);
	device->CreateComputeShader(cs_blob->GetBufferPointer(), cs_blob->GetBufferSize(), nullptr, &pMainCSshader);
	cs_blob->Release();
	if (error_blob) error_blob->Release();

	BYTE setCSshader[] = {
		0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x48, 0xBA // mov rdx
	};
	int i_setCSshader = 0x11776D0;
	// EDF5.exe+5DD29C
	i_setCSshader = 0x5DD29C;
	WriteHookToProcess((void*)(hmodEXE + i_setCSshader), setCSshader, 12);
	WriteHookToProcess((void*)(hmodEXE + i_setCSshader + 12), &pMainCSshader, 8);
}
