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

#include "utiliy.h"
#include "1FullAOData.hpp"
#include "1FullAO.h"

extern PBYTE hmodEXE;

ID3D11ComputeShader* pMainCSshader;

void HookFunction_D3D11_FullAO(ID3D11Device* device) {

	//WriteHookToProcess((void*)(hmodDLL + 0x117750F), (void*)&nop2, 2U);
	//WriteHookToProcess((void*)(hmodDLL + 0x117752A), (void*)&nop2, 2U);
	//WriteHookToProcess((void*)(hmodDLL + 0x117765A), (void*)&nop6, 6U);

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

	// ================================================

	
	//WriteHookToProcess((void*)(hmodDLL + i_setCSshader), setCSshader, 12);
	//WriteHookToProcess((void*)(hmodDLL + i_setCSshader + 12), &pFullAOCSshader, 8);



	// other

	// Calculate Linear Depth
	// edf.dll+1177701 set cs shader

	/*
	BYTE dispatchX[] = {
		0xC1, 0xE8, 0x04,
		0x41, 0xF6, 0xC5, 0x0F
	};
	WriteHookToProcess((void*)(hmodDLL + 0x11777D1), dispatchX, 7);

	BYTE dispatchY[] = {
		0xC1, 0xEA, 0x04,
		0x41, 0xF6, 0xC4, 0x0F
	};
	WriteHookToProcess((void*)(hmodDLL + 0x11777EC), dispatchY, 7);

	BYTE noUndergroundCS[] = {
		0x31, 0xC0
	};
	WriteHookToProcess((void*)(hmodDLL + 0x11B541E), noUndergroundCS, 2);*/
}
