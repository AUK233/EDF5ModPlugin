#pragma once
#include "zEDF/EDFSoldierClass.hpp"
#include <d3d11.h>

extern "C" {
	int __fastcall togui_GetDXGISwapChain(int protectECX, IDXGISwapChain* pSwapChain);
	void __fastcall togui_ClearImGui();
}

HRESULT __stdcall togui_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
HRESULT __stdcall togui_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
LRESULT __stdcall togui_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void togui_InitializeImGui();
void togui_MainDisplay();
void togui_MainDisplayTest();
extern "C" void togui_MainDisplayInMission();

void togui_MainDisplay_ToDigit(PG_SoldierBase* pLCP);
void togui_MainDisplay_ToDigit_Damage(UINT32 index, int isSplitScreen);
void togui_MainDisplay_ToDigit_DamageInHitMode();
void togui_MainDisplay_ToDigit_Weapon(UINT32 index);

void togui_MainDisplay_ToDigitTest(ID3D11DeviceContext* pCTX);

