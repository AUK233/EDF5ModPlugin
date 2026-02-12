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

IDXGISwapChain** ppSwapChain;
// 0 is x, 1 is y, 2 is fps, 3 is windowed mode
DXGI_SWAP_CHAIN_DESC* pDXGISwapChainDesc;
// 0 is x, 1 is y, 4 is actual x? 5 is actual y?
int* pRealTimeResolution;

PGameDXGIRender pGameDXGIRenderer;

void DXGI_Initialize(PBYTE hmodEXE)
{
	// EDF5.exe+1256BD0
	auto tempP = hmodEXE + 0x1256BD0;
	pGameDXGIRenderer = (PGameDXGIRender)tempP;

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

PGameDXGIRender __fastcall DXGI_GetGameDXGIRender()
{
	return pGameDXGIRenderer;
}

DXGI_SWAP_CHAIN_DESC* __fastcall DXGI_GetDXGISwapChainDesc()
{
	return &pGameDXGIRenderer->DXGISwapChainDesc;
}
