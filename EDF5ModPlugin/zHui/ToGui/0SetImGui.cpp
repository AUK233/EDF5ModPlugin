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
#include "1DigitProcessor.h"

#include "0SetImGui.h"


//#define DEBUGMODE

DigitRenderer::PDynamicDigitRenderer g_DigitRenderer;
DigitRenderer::PDynamicDigitProcessor g_DigitProcessor;

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
#if defined(DEBUGMODE)
		if (!fnIDXGISwapChainResizeBuffers) {
			auto addrResizeBuffers = (IDXGISwapChainResizeBuffers*)&pVTable[13]; // vft+0x68
			fnIDXGISwapChainResizeBuffers = *addrResizeBuffers;

			addrToHook = (uintptr_t)togui_ResizeBuffers;
			WriteHookToProcess(addrResizeBuffers, &addrToHook, 8U);
		}
#endif
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

#if defined(DEBUGMODE)
	togui_MainDisplay();
#endif

	static bool rKeyDown = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000) { // Alt 按下
		if ((GetAsyncKeyState('R') & 0x8001) && !rKeyDown) {
			rKeyDown = true;

			// 确保 ImGui 没有捕获键盘
			if (!ImGui::GetIO().WantCaptureKeyboard) {
				g_DigitRenderer->ReloadDynamicPosShader();
			}
		}
	}

	if (!(GetAsyncKeyState('R') & 0x8000)) {
		rKeyDown = false;
	}

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
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	//io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(pRender->DXGISwapChainDesc.OutputWindow);
	ImGui_ImplDX11_Init(pRender->pD3D11Device, pRender->pD3D11DeviceContext);

	using namespace DigitRenderer;
	g_DigitRenderer = (PDynamicDigitRenderer)_aligned_malloc(sizeof(DynamicDigitRenderer_t), 0x10);
	g_DigitRenderer = new(g_DigitRenderer) DynamicDigitRenderer_t();
	g_DigitRenderer->Initialize();

	g_DigitProcessor = (PDynamicDigitProcessor)_aligned_malloc(sizeof(DynamicDigitProcessor_t), 0x10);
	g_DigitProcessor = new(g_DigitProcessor) DynamicDigitProcessor_t();
	g_DigitProcessor->Initialize();
	// end
}

void togui_MainDisplay()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// Todo =====================================================================

	togui_MainDisplayTest();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::Begin("EDF hook", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

	togui_MainDisplay_ToDigit();

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
#if defined(DEBUGMODE)
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
#endif

	ImGui::SetWindowFontScale(1.0f);
	// ====================
	ImGui::End();
}

void togui_MainDisplayInMission() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGui::Begin("EDF hook", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

	// Todo =====================================================================
	togui_MainDisplay_ToDigit();
	// End ======================================================================

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void togui_MainDisplay_ToDigit()
{
	auto pCTX = DXGI_GetGameDXGIRender()->pD3D11DeviceContext;
	ImDrawList* pDrawList = ImGui::GetWindowDrawList();

#if defined(DEBUGMODE)
	ImVec2 origin = pDrawList->GetClipRectMin();
	pDrawList->AddCircleFilled(origin, 100, IM_COL32(255, 0, 0, 255));
#else
	auto pLCP = DigitRenderer::GetLocalCurrentPlayersPointer();
	if (!pLCP[0]) return;
#endif

	g_DigitRenderer->BeginFrame();

	// for availability check
#if defined(DEBUGMODE)
	togui_MainDisplay_ToDigitTest(pCTX);
#endif

	g_DigitRenderer->SetRender(pCTX, &g_DigitProcessor->DigitConstantData, DigitRenderer::DigitRendererShader_Fixed);

	auto isSplitScreen = DigitRenderer::GetIsSplitScreen();

	togui_MainDisplay_ToDigit_Damage(0, isSplitScreen);

	if (isSplitScreen) {
		togui_MainDisplay_ToDigit_Damage(1, isSplitScreen);
	}

	// end


	auto pGameRender = DXGI_GetGameRenderer1259680();
	if (pGameRender) {
		using namespace DigitRenderer;

		memcpy(&g_DigitRenderer->g_constants0, &pGameRender->ConstantBuffer0, sizeof(xgl_system_CB_t));
		memcpy(&g_DigitRenderer->g_constants1, &pGameRender->ConstantBuffer1, sizeof(xgl_transform_CB_t));
		/*auto pSys = XGS_GetXGSSystemPointer();
		auto pCamera = pSys->player[0].pCamera;
		memcpy(&g_DigitRenderer->g_constants1, &pCamera->CameraTransform, sizeof(xgl_transform_CB_t));*/
		g_DigitRenderer->SetRender(pCTX, &g_DigitProcessor->DigitConstantData, DigitRenderer::DigitRendererShader_Dynamic);

		DigitFontControl_t fontControl;
		ZeroMemory(&fontControl, sizeof(DigitFontControl_t));
		fontControl.charAlignFactor = 0.5;
		fontControl.i_fontSize = 48;
		fontControl.f_fontSize = 48;
		fontControl.scaleFactor = 0;
		fontControl.fadeFactor = 0;
		fontControl.time = 1;

		auto text_damage = FormatNumberToDigitRendererChars_Damage(45.7);
		g_DigitRenderer->SetImageDataInDynamicPos(text_damage, { 395, 5, 461, 1 }, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_Red);
		g_DigitRenderer->SetImageDataInDynamicPos(text_damage, { -395, 50, -461, 1 }, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_Blue);
	}

	g_DigitRenderer->EndFrame();
}

void togui_MainDisplay_ToDigit_Damage(UINT32 index, int isSplitScreen)
{
#ifndef DEBUGMODE
	using namespace DigitRenderer;

	DigitFontControl_t fontControl;
	__m128 text_pos, pos_factor;
	DigitTextByte text_damage;
	auto isInVehicle = g_DigitProcessor->PlayerInVehicle[index];

	// set history damage display
	int damageCount = g_DigitProcessor->v_playerDamage[index].size();
	if (damageCount) {
		// initial settings
		if (!isInVehicle) {
			memcpy(&fontControl, &g_DigitProcessor->DamageDisplayFont_Human, sizeof(DigitFontControl_t));

			text_pos = g_DigitProcessor->DamageDisplayPos_Human[index + isSplitScreen];
			pos_factor = g_DigitProcessor->DamageDisplayPos_HumanFactor;
		} else {
			memcpy(&fontControl, &g_DigitProcessor->DamageDisplayFont_Vehicle, sizeof(DigitFontControl_t));

			text_pos = g_DigitProcessor->DamageDisplayPos_Vehicle[index + isSplitScreen];
			pos_factor = g_DigitProcessor->DamageDisplayPos_VehicleFactor;
		}

		// read history damage
		for (int i = 0; i < damageCount; i++) {
			auto& dmg = g_DigitProcessor->v_playerDamage[index][i];

			fontControl.time = dmg.effectTime;
			fontControl.scaleFactor = dmg.scaleFactor;
			fontControl.fadeFactor = dmg.fadeFactor;

			auto text_damage = FormatNumberToDigitRendererChars_Damage(dmg.value.fp32);
			g_DigitRenderer->SetImageDataInFixedPos(text_damage, text_pos, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_White);

			text_pos = _mm_add_ps(text_pos, pos_factor);
		}
		// end
	}

	// set charge damage display
	auto pDmg = &g_DigitProcessor->playerDamage[index];
	if (!pDmg->value.s32) return;


	if (!isInVehicle) {
		memcpy(&fontControl, &g_DigitProcessor->DamageDisplayFont_Human, sizeof(DigitFontControl_t));
		text_pos = g_DigitProcessor->DamageDisplayPos_Human[index + isSplitScreen];

		static const __m128 offset_human = { -28, 16, -28, 16 };
		text_pos = _mm_add_ps(text_pos, offset_human);
	} else {
		memcpy(&fontControl, &g_DigitProcessor->DamageDisplayFont_Vehicle, sizeof(DigitFontControl_t));
		text_pos = g_DigitProcessor->DamageDisplayPos_Vehicle[index + isSplitScreen];

		static const __m128 offset_vehicle = { 40, -20, 40, -20 };
		text_pos = _mm_add_ps(text_pos, offset_vehicle);
	}

	fontControl.time = pDmg->effectTime;
	fontControl.scaleFactor = pDmg->scaleFactor;
	fontControl.fadeFactor = pDmg->fadeFactor;

	text_damage = FormatNumberToDigitRendererChars_Damage(pDmg->value.fp32);
	g_DigitRenderer->SetImageDataInFixedPos(text_damage, text_pos, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_White);
#endif
}

void togui_MainDisplay_ToDigitTest(ID3D11DeviceContext* pCTX)
{
#ifdef DEBUGMODE
	using namespace DigitRenderer;

	static int time = 0;
	if (time > 40) {
		time = 0;
	}

	DigitConstants_t cbDigitData;
	cbDigitData.ScreenSize[0] = 1920.0f;
	cbDigitData.ScreenSize[1] = 1080.0f;
	cbDigitData.ScreenScale[0] = 1;
	cbDigitData.ScreenScale[1] = 1;
	cbDigitData.ScaleSpeed = 0.025;
	cbDigitData.FadeSpeed = -0.025;
	cbDigitData.pad18[0] = 0;
	cbDigitData.pad18[1] = 0;
	cbDigitData.BorderColor = {0,0,0,1};

	__m128 v_basePos = { 1900, 900, 1964, 964 };

	g_DigitRenderer->SetRender(pCTX, &cbDigitData, DigitRendererShader_Fixed);

	DigitFontControl_t fontControl;
	ZeroMemory(&fontControl, sizeof(DigitFontControl_t));
	fontControl.charAlignFactor = 1;
	fontControl.i_fontSize = 64;
	fontControl.f_fontSize = 64;
	fontControl.scaleFactor = 0;
	fontControl.fadeFactor = time;
	fontControl.time = 1;

	auto textData = FormatNumberToDigitRendererChars_Damage(12.1f);
	g_DigitRenderer->SetImageDataInFixedPos(textData, v_basePos, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_Blue);

	__m128 v_basePos1 = { 100, 100, 148, 148 };
	fontControl.charAlignFactor = 0;
	fontControl.i_fontSize = 48;
	fontControl.f_fontSize = 48;
	fontControl.scaleFactor = time;
	fontControl.fadeFactor = time;
	auto textData1 = FormatNumberToDigitRendererChars_Percentage(2.34f);
	g_DigitRenderer->SetImageDataInFixedPos(textData1, v_basePos1, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_Red);

	__m128 v_basePos2 = { 1340, 860, 1372, 892 };
	fontControl.charAlignFactor = 0.5;
	fontControl.i_fontSize = 32;
	fontControl.f_fontSize = 32;
	fontControl.scaleFactor = time;
	fontControl.fadeFactor = 0;
	auto textData2 = FormatNumberToDigitRendererChars_Damage(121456789.1f);
	g_DigitRenderer->SetImageDataInFixedPos(textData2, v_basePos2, &fontControl, DynamicDigitRenderer_t::DigitRendererColor_White);

	time++;
#endif // DEBUGMODE
}
