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
#include "0GetDXGI.h"
#include "1DigitRenderer.h"

#include "0SetImGui.h"


#define DEBUGMODE

PDynamicDigitRenderer g_DigitRenderer;

typedef HRESULT(__stdcall* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* IDXGISwapChainResizeBuffers)(IDXGISwapChain*, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
BOOL g_bImGuiInitialized = 0;
WNDPROC oWndProc;
IDXGISwapChain* g_pSwapChain = nullptr;
IDXGISwapChainPresent fnIDXGISwapChainPresent = 0;
IDXGISwapChainResizeBuffers fnIDXGISwapChainResizeBuffers = 0;

int __fastcall togui_GetDXGISwapChain(int protectECX, IDXGISwapChain* pSwapChain)
{
	g_pSwapChain = pSwapChain;
	if (pSwapChain) {
		auto pVTable = *(IDXGISwapChainPresent**)(pSwapChain);
		uintptr_t addrToHook;

		if (!fnIDXGISwapChainPresent) {
			auto addrPresent = &pVTable[8]; // vft+0x40
			fnIDXGISwapChainPresent = *addrPresent;

			addrToHook = (uintptr_t)togui_Present;
			WriteHookToProcess(addrPresent, &addrToHook, 8U);
		}

		if (!fnIDXGISwapChainResizeBuffers) {
			auto addrResizeBuffers = (IDXGISwapChainResizeBuffers*)&pVTable[13]; // vft+0x68
			fnIDXGISwapChainResizeBuffers = *addrResizeBuffers;

			addrToHook = (uintptr_t)togui_ResizeBuffers;
			WriteHookToProcess(addrResizeBuffers, &addrToHook, 8U);
		}
		// end
	}
	return protectECX;
}

void __fastcall togui_ClearImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

#if defined(DEBUGMODE)
int testResolution[2];
int bTestResolution = 0;
#endif

HRESULT __stdcall togui_Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!g_bImGuiInitialized) {
		togui_InitializeImGui();
		auto pRender = DXGI_GetGameDXGIRender();
		oWndProc = (WNDPROC)SetWindowLongPtrW(pRender->DXGISwapChainDesc.OutputWindow, GWLP_WNDPROC, (LONG_PTR)togui_WndProc);
		g_bImGuiInitialized = 1;
		return fnIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
	}

	togui_MainDisplay();

	return fnIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
}

HRESULT __stdcall togui_ResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	auto pRender = DXGI_GetGameDXGIRender();
	pRender->pMainRenderTargetView->Release();

	//togui_ClearImGui();

	HRESULT hr = fnIDXGISwapChainResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
	if (SUCCEEDED(hr)) {
		// this is the previous resolution.
		/*testResolution[0] = pRender->DXGISwapChainDesc.BufferDesc.Width;
		testResolution[1] = pRender->DXGISwapChainDesc.BufferDesc.Height;*/
#if defined(DEBUGMODE)
		testResolution[0] = Width;
		testResolution[1] = Height;
#endif

		//togui_InitializeImGui();
	}
	return hr;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall togui_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
	return CallWindowProcW(oWndProc, hWnd, msg, wParam, lParam);
}

void togui_InitializeImGui()
{
	auto pRender = DXGI_GetGameDXGIRender();
	/*if (!pRender->pD3D11Device || !pRender->pD3D11DeviceContext) {
		return;
	}*/

	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr; // do not output INI file.
	io.WantSaveIniSettings = false; // do not save INI file.
	//io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(pRender->DXGISwapChainDesc.OutputWindow);
	ImGui_ImplDX11_Init(pRender->pD3D11Device, pRender->pD3D11DeviceContext);

	g_DigitRenderer = (PDynamicDigitRenderer)_aligned_malloc(sizeof(DynamicDigitRenderer_t), 0x10);
	if (g_DigitRenderer) {
		g_DigitRenderer = new(g_DigitRenderer) DynamicDigitRenderer_t();
		g_DigitRenderer->Initialize();
	}
	// end
}

void togui_MainDisplay()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Todo =====================================================================

#if defined(DEBUGMODE)
	togui_MainDisplayTest();
#endif

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::Begin("EDF hook", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

	togui_MainDisplay_ShowNumber();

	ImGui::End();


	// End ======================================================================
	ImGui::EndFrame();
	ImGui::Render();
	auto pRender = DXGI_GetGameDXGIRender();
	pRender->pD3D11DeviceContext->OMSetRenderTargets(1, &pRender->pMainRenderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void togui_MainDisplayTest()
{
	ImGui::Begin("EDF hook", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
	// ====================
	ImGui::SetWindowFontScale(2.0f);
	//ImGui::SetCursorPos(ImVec2(960, 540));

	// It has been verified that they are same.
	if (bTestResolution) {
		ImGui::Text("resolution: -%d, -%d", testResolution[0], testResolution[1]);
	} else {
		if (testResolution[0]) {
			// they are same.
			auto pRender = DXGI_GetGameDXGIRender();
			if (pRender->DXGISwapChainDesc.BufferDesc.Width != testResolution[0] && pRender->DXGISwapChainDesc.BufferDesc.Height != testResolution[1]) {
				bTestResolution = 1;
			}
		}

		// but they are different from DXGISwapChainDesc.
		auto pResolution = DXGI_GetRealTimeResolution();
		ImGui::Text("resolution: %d, %d", pResolution[0], pResolution[1]);
	}

	ImGui::SetWindowFontScale(1.0f);
	// ====================
	ImGui::End();
}

void togui_MainDisplay_ShowNumber()
{
	g_DigitRenderer->BeginFrame();

	auto pCTX = DXGI_GetGameDXGIRender()->pD3D11DeviceContext;

	DynamicDigitRenderer_t::DigitConstants_t digitData;
	digitData.ScreenSize[0] = 1920.0f;
	digitData.ScreenSize[1] = 1080.0f;

	digitData.OutPos[0] = 100.0f;
	digitData.OutPos[1] = 100.0f;
	digitData.Color[0] = 1;
	digitData.Color[1] = 0;
	digitData.Color[2] = 0;
	digitData.Color[3] = 1;
	g_DigitRenderer->SetRender(pCTX, &digitData);
	g_DigitRenderer->SetImageData();

	digitData.OutPos[0] = 500.0f;
	digitData.OutPos[1] = 200.0f;
	digitData.Color[0] = 0;
	digitData.Color[1] = 1;
	digitData.Color[2] = 0;
	digitData.Color[3] = 1;
	g_DigitRenderer->SetRender(pCTX, &digitData);
	g_DigitRenderer->SetImageData();

	digitData.OutPos[0] = 1000.0f;
	digitData.OutPos[1] = 300.0f;
	digitData.Color[0] = 0;
	digitData.Color[1] = 0;
	digitData.Color[2] = 0;
	digitData.Color[3] = 1;
	g_DigitRenderer->SetRender(pCTX, &digitData);
	g_DigitRenderer->SetImageData();

	g_DigitRenderer->EndFrame();
}
