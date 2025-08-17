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

#include <d3d11.h>
#pragma comment(lib, "D3D11.lib")

#include "utiliy.h"
#include "commonNOP.h"
#include "HuiModConsole.h"

int ModConsoleStatus;
HUiModConsoleFunctionMap* pHuiModConsoleFunction;
DXGI_SWAP_CHAIN_DESC* pDXGISwapChainDesc;

extern "C" {
	void __fastcall ASMgetChatWideString();
	uintptr_t getChatWideStrinRetAddr;
	uintptr_t vedf125ABD8;
	extern uintptr_t vedf125AB68;
}

void module_UpdateHuiModConsole(PBYTE hmodEXE)
{
	// EDF5.exe+4FD1AF
	hookGameBlockWithInt3((void*)(hmodEXE + 0x4FD1AF), (uintptr_t)ASMgetChatWideString);
	WriteHookToProcess((void*)(hmodEXE + 0x4FD1AF + 15), (void*)&nop3, 3U);
	getChatWideStrinRetAddr = (uintptr_t)(hmodEXE + 0x4FD1C1);

	module_InitializeHuiModConsole(hmodEXE);
}

void module_InitializeHuiModConsole(PBYTE hmodEXE)
{
	auto tempP = hmodEXE + 0x1256C40;
	pDXGISwapChainDesc = (DXGI_SWAP_CHAIN_DESC*)tempP;

	vedf125ABD8 = (uintptr_t)(hmodEXE + 0x125ABD8);

	void* pMemory = _aligned_malloc(sizeof(HUiModConsoleFunctionMap), 16U);
	if (pMemory) {
		pHuiModConsoleFunction = new(pMemory) HUiModConsoleFunctionMap();
		(*pHuiModConsoleFunction)[L"~kzt"] = ModConsole_DisableConsole;
		(*pHuiModConsoleFunction)[L"`111"] = ModConsole_CreateKeyMonitor;
		(*pHuiModConsoleFunction)[L"~GiveMeArmorBox"] = ModConsole_GetArmorBox;
		(*pHuiModConsoleFunction)[L"~GiveMeWeaponBox"] = ModConsole_GetWeaponBox;
		// EDF5.exe+592F0, set_invincible
		(*pHuiModConsoleFunction)[L"~WhosYourDaddy"] = (ModConsoleFuncPTR)(hmodEXE + 0x592F0);
		// EDF5.exe+610C0
		(*pHuiModConsoleFunction)[L"~UnlockAllWeapon"] = (ModConsoleFuncPTR)(hmodEXE + 0x610C0);
		// EDF5.exe+60F40, get_whole_clear_item, it has bug
		//(*pHuiModConsoleFunction)[L"~CallRecoveryTeam"] = (ModConsoleFuncPTR)(hmodEXE + 0x60F40);
		// EDF5.exe+61400, max_weapon_quality
		(*pHuiModConsoleFunction)[L"~ImWeaponMaster"] = (ModConsoleFuncPTR)(hmodEXE + 0x61400);
	}
}

void __fastcall module_ActivateModConsole(PEDFWideString wstr)
{
	wchar_t* pText;
	if (wstr->capacity < 8) {
		pText = wstr->text;
	}
	else {
		pText = wstr->pstr;
	}

	if (!ModConsoleStatus) {
		if (wstr->size == 4) {
			//L"~kzt"
			if (*(INT64*)pText == 32651621291458686) {
				ModConsoleStatus = 1;
				MessageBeep(MB_OK);
				//MessageBoxW(NULL, L"Mod Console Activated", L"Mod Console", MB_OK);
			}
		}
		return;
	}

	// check command
	std::wstring_view wstr_sv(pText, wstr->size);
	auto it = pHuiModConsoleFunction->find(wstr_sv);
	if (it != pHuiModConsoleFunction->end()) {
		return it->second();
	}
}

void __fastcall ModConsole_DisableConsole()
{
	ModConsoleStatus = 0;
	MessageBeep(MB_ICONERROR);
}

void __fastcall ModConsole_GetArmorBox()
{
	Pedf125ABD8 pData = *(Pedf125ABD8*)vedf125ABD8;
	pData->ExtraArmorBox += 10000;
	pData->TotalArmorBox += 10000;

	MessageBeep(MB_ICONINFORMATION);
}

void __fastcall ModConsole_GetWeaponBox()
{
	Pedf125ABD8 pData = *(Pedf125ABD8*)vedf125ABD8;
	pData->ExtraWeaponBox += 256;
	pData->TotalWeaponBox += 256;

	MessageBeep(MB_ICONINFORMATION);
}

void __fastcall ModConsole_CreateKeyMonitor()
{
	HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ModConsole_MonitorKeys, NULL, NULL, NULL);
	if (tempHND) {
		CloseHandle(tempHND);
	}
	MessageBeep(MB_ICONINFORMATION);
}

void ModConsole_MonitorKeys()
{
	while (ModConsoleStatus) {
		//
		if (GetForegroundWindow() == pDXGISwapChainDesc->OutputWindow) {

			auto Pressed_ctrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;
			auto Pressed_F1 = GetAsyncKeyState(VK_F1) & 0x8000;
			auto Pressed_F2 = GetAsyncKeyState(VK_F2) & 0x8000;

			if (Pressed_ctrl) {

				if (Pressed_F1) {
					ModConsole_MK_GetPlayerPos();
				}
				//
				if (Pressed_F2) {
					ModConsole_MK_GetCameraPos();
				}
				//
			}
		}

		Sleep(10);
	}

	MessageBeep(MB_ICONERROR);
}

void ModConsole_WriteToClipboard(const std::wstring& text)
{
	if (OpenClipboard(nullptr)) {
		EmptyClipboard();

		size_t size = (text.size() + 1) * sizeof(wchar_t);
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);

		if (hGlobal != 0) {
			wchar_t* clipboardText = (wchar_t*)GlobalLock(hGlobal);
			if (clipboardText != 0) {
				wcscpy_s(clipboardText, size / sizeof(wchar_t), text.c_str());
				SetClipboardData(CF_UNICODETEXT, hGlobal);
			}
			GlobalUnlock(hGlobal);
		}
		CloseClipboard();

		MessageBeep(MB_ICONINFORMATION);
	}
}

void __fastcall ModConsole_MK_GetPlayerPos()
{
	auto pPlayer = ASMModConsole_GetPlayerAddress(vedf125AB68);
	if (!pPlayer) {
		return;
	}

	auto pPlayerPos = pPlayer->basePosMatrix;
	std::wstring text = std::format(L"[ {:.3f}, {:.3f}, {:.3f} ]", pPlayerPos[3][0], pPlayerPos[3][1], pPlayerPos[3][2]);
	//text += std::format(L"\nPlayerAddress: {:X}", (uintptr_t)pPlayer);
	ModConsole_WriteToClipboard(text);
}

void __fastcall ModConsole_MK_GetCameraPos()
{
	auto pPlayerPos = ASMModConsole_GetCameraPostion(vedf125AB68);
	if (!pPlayerPos) {
		return;
	}

	std::wstring text = std::format(L"[ {:.3f}, {:.3f}, {:.3f} ]", pPlayerPos[0], pPlayerPos[1], pPlayerPos[2]);
	ModConsole_WriteToClipboard(text);
}
