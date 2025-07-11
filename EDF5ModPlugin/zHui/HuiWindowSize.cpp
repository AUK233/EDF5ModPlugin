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
#include "HuiWindowSize.h"

IDXGISwapChain** ppSwapChain;
extern "C" {
	// 0 is x, 1 is y, 2 is fps, 3 is windowed mode
	DXGI_SWAP_CHAIN_DESC* pDXGISwapChainDesc;
	// 0 is x, 1 is y, 4 is actual x? 5 is actual y?
	int* pRealTimeResolution;

	void __fastcall ASMAdjustWindowRect();
	uintptr_t AdjustWindowRectRetAddr;
}

void module_UpdateHuiWindowSize(PBYTE hmodEXE)
{
	// EDF5.exe+1256C40
	auto tempP = hmodEXE + 0x1256C40;
	pDXGISwapChainDesc = (DXGI_SWAP_CHAIN_DESC*)tempP;
	// EDF5.exe+125A9D0
	tempP = hmodEXE + 0x125A9D0;
	pRealTimeResolution = (int*)tempP;
	//
	tempP = hmodEXE + 0x1256C98;
	ppSwapChain = (IDXGISwapChain**)tempP;

	// EDF5.exe+60B7DD
	hookGameBlockWithInt3((void*)(hmodEXE + 0x60B7DD), (uintptr_t)ASMAdjustWindowRect);
	WriteHookToProcess((void*)(hmodEXE + 0x60B7DD + 15), (void*)&nop1, 1U);
	AdjustWindowRectRetAddr = (uintptr_t)(hmodEXE + 0x60B7ED);
}

void __fastcall CustomAdjustWindowRect(LPRECT lpRect)
{
	if (!*ppSwapChain) {
		return;
	}

	/*IDXGISwapChain* pSwapChain = *ppSwapChain;
	DXGI_SWAP_CHAIN_DESC chainDesc = { 0 };
	pSwapChain->GetDesc(&chainDesc);
	chainDesc.BufferDesc.Width = 1920;
	chainDesc.BufferDesc.Height = 1080;
	chainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	pSwapChain->ResizeTarget(&chainDesc.BufferDesc);*/

	pRealTimeResolution[0] = 1920;
	pRealTimeResolution[1] = 1080;
	lpRect->right = 1920 + 8;
	lpRect->bottom = 1080 + 8;
}
