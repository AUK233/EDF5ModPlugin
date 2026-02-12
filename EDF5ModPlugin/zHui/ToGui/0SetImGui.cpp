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

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "utiliy.h"
#include "commonNOP.h"
#include "0GetDXGI.h"
#include "0SetImGui.h"

typedef HRESULT(__stdcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* IDXGISwapChainResizeBuffers)(IDXGISwapChain*, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
BOOL g_bImGuiInitialized = 0;
IDXGISwapChain* g_pSwapChain = nullptr;
IDXGISwapChainPresent fnIDXGISwapChainPresent = 0;
IDXGISwapChainResizeBuffers fnIDXGISwapChainResizeBuffers = 0;

int __fastcall togui_GetDXGISwapChain(int protectECX, IDXGISwapChain* pSwapChain)
{
	g_pSwapChain = pSwapChain;
	if (pSwapChain) {
		auto pVTable = *(IDXGISwapChainPresent**)(pSwapChain);
		auto addrPresent = &pVTable[8]; // vft+0x40
		fnIDXGISwapChainPresent = *addrPresent;

		uintptr_t addrToHook = (uintptr_t)togui_Present;
		WriteHookToProcess(addrPresent, &addrToHook, 8U);

		auto addrResizeBuffers = (IDXGISwapChainResizeBuffers*)&pVTable[13]; // vft+0x68
		fnIDXGISwapChainResizeBuffers = *addrResizeBuffers;
		addrToHook = (uintptr_t)togui_ResizeBuffers;
		WriteHookToProcess(addrResizeBuffers, &addrToHook, 8U);

	}
	return protectECX;
}


ID3D11RenderTargetView* mainRenderTargetView;

HRESULT __stdcall togui_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!g_bImGuiInitialized) {
		togui_InitializeImGui();
		g_bImGuiInitialized = 1;
		return fnIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
	}


	auto Pressed_ctrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;
	if (Pressed_ctrl) {
		//MessageBeep(MB_ICONINFORMATION);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//
	/*ImGui::SetWindowFontScale(2.0f);
	ImGui::SetCursorPos(ImVec2(960, 540));
	ImGui::SetWindowFontScale(1.0f);*/
	ImGui::Text("HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello");
	//

	/*ImGui::End();
	ImGui::EndFrame();*/

	ImGui::Render();
	auto pRender = DXGI_GetGameDXGIRender();
	pRender->pD3D11DeviceContext->OMSetRenderTargets(1, &pRender->pMainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return fnIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall togui_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	auto pRender = DXGI_GetGameDXGIRender();
	pRender->pMainRenderTargetView->Release();

	ImGui_ImplDX11_Shutdown();

	HRESULT hr = fnIDXGISwapChainResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (SUCCEEDED(hr)) {
		togui_InitializeImGui();
	}
	return hr;
}

void togui_InitializeImGui()
{
	auto pRender = DXGI_GetGameDXGIRender();
	if (!pRender->pD3D11Device || !pRender->pD3D11DeviceContext) {
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(pRender->DXGISwapChainDesc.OutputWindow);
	ImGui_ImplDX11_Init(pRender->pD3D11Device, pRender->pD3D11DeviceContext);
}
