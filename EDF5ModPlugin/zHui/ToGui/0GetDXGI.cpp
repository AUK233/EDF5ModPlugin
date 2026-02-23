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

#include "utiliy.h"
#include "commonNOP.h"
#include "0GetDXGI.h"

extern "C" {
	uintptr_t  xgs_umbra_System125B080;
}

IDXGISwapChain** ppSwapChain;
// 0 is x, 1 is y, 2 is fps, 3 is windowed mode
DXGI_SWAP_CHAIN_DESC* pDXGISwapChainDesc;
// 0 is x, 1 is y, 4 is actual x? 5 is actual y?
int* pRealTimeResolution;

PGameDXGIRender pGameDXGIRenderer;
PGameRenderer1259680* pGameRenderer1259680;

void DXGI_Initialize(PBYTE hmodEXE)
{
	// EDF5.exe+1256BD0
	auto tempP = hmodEXE + 0x1256BD0;
	pGameDXGIRenderer = (PGameDXGIRender)tempP;

	// EDF5.exe+1259680
	tempP = hmodEXE + 0x1259680;
	pGameRenderer1259680 = (PGameRenderer1259680*)tempP;

	// EDF5.exe+125B080
	xgs_umbra_System125B080 = (uintptr_t)(hmodEXE + 0x125B080);



	// EDF5.exe+1256C40
	tempP = hmodEXE + 0x1256C40;
	pDXGISwapChainDesc = (DXGI_SWAP_CHAIN_DESC*)tempP;
	// EDF5.exe+125A9D0
	tempP = hmodEXE + 0x125A9D0;
	pRealTimeResolution = (int*)tempP;
	//
	tempP = hmodEXE + 0x1256C98;
	ppSwapChain = (IDXGISwapChain**)tempP;
}

PGameDXGIRender __fastcall DXGI_GetGameDXGIRender() {
	return pGameDXGIRenderer;
}

PGameRenderer1259680 __fastcall DXGI_GetGameRenderer1259680() {
	return *pGameRenderer1259680;
}

Pxgs_umbra_System125B080 __fastcall DXGI_GetUmbraSystem125B080() {
	return DXGI_GetUmbraSystem125B080ASM(xgs_umbra_System125B080);
}

int* __fastcall DXGI_GetRealTimeResolution() {
	return pRealTimeResolution;
}

DXGI_SWAP_CHAIN_DESC* __fastcall DXGI_GetDXGISwapChainDesc()
{
	return &pGameDXGIRenderer->DXGISwapChainDesc;
}
