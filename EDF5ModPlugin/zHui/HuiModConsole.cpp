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
#include "ToGui/0GetDXGI.h"
#include "HuiModConsole.h"

Pcmd_KeyState cmd_keyState;
Pcmd_ModConsoleFunc cmd_ModConsoleFunc;

extern "C" {
	uintptr_t vedf125ABD8;
	extern uintptr_t vedf125AB68;
}

void cmd_ModConsole_Initialize(PBYTE hmodEXE) {
	vedf125ABD8 = (uintptr_t)(hmodEXE + 0x125ABD8);

	auto p = (Pcmd_KeyState)_aligned_malloc(sizeof(cmd_KeyState_t), 16U);
	cmd_keyState = p;
	if (p) {
		ZeroMemory(p, sizeof(cmd_KeyState_t));

		HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)cmd_ModConsole_MonitorKeys, NULL, NULL, NULL);
		if (tempHND) {
			CloseHandle(tempHND);
		}
	}

	auto p2 = (Pcmd_ModConsoleFunc)_aligned_malloc(sizeof(cmd_ModConsoleFunc_t), 16U);
	cmd_ModConsoleFunc = p2;
	if (p2) {
		// EDF5.exe+592F0, set_invincible
		p2->SetInvincible = (ModConsoleFuncPTR)(hmodEXE + 0x592F0);
		// EDF5.exe+610C0
		p2->UnlockAllWeapon = (ModConsoleFuncPTR)(hmodEXE + 0x610C0);
		// EDF5.exe+61400, max_weapon_quality
		p2->MaxWeaponQuality = (ModConsoleFuncPTR)(hmodEXE + 0x61400);
		// EDF5.exe+61670, max_weapon_quality
		p2->UnlockMission = (ModConsoleFuncPTR)(hmodEXE + 0x61670);
	}
	// end
}

void cmd_ModConsole_MonitorKeys() {
	union cmd_state_t {
		struct cmd_state_t_s {
			bool cheatonce;
			bool unlock;
			bool get;
		} b;
		std::uint64_t i;
	};

	static cmd_state_t state;
	state.i = 0;
	// delayed startup by 10 seconds.
	Sleep(10000);

	while (true) {
		auto pDXGI = DXGI_GetDXGISwapChainDesc();
		if (GetForegroundWindow() != pDXGI->OutputWindow) {
			ZeroMemory(cmd_keyState, sizeof(cmd_KeyState_t));
			goto gotoSleep;
		}

		cmd_ModConsole_SetKeyState();

		if (cmd_keyState->alt) {

			if (cmd_keyState->AlphabetKey['O' - 'A'] == 1) {
				state.b.cheatonce = !state.b.cheatonce;
			}

			if (cmd_keyState->ctrl){
				// F1
				if (cmd_keyState->F_key[0] == 1) {
					cmd_ModConsole_GetPlayerPos(0);
				}
				// F2
				if (cmd_keyState->F_key[1] == 1) {
					cmd_ModConsole_GetPlayerPos(1);
				}
			}

			if (state.b.cheatonce) {
				if (cmd_keyState->AlphabetKey['U' - 'A'] == 1) {
					state.b.unlock = !state.b.unlock;
				}
				else if (cmd_keyState->AlphabetKey['G' - 'A'] == 1) {
					state.b.get = !state.b.get;
				}

				if (state.b.unlock) {
					//if (cmd_keyState->AlphabetKey['I' - 'A'] == 1) {
					//	cmd_ModConsoleFunc->SetInvincible();
					//}
					if (cmd_keyState->AlphabetKey['M' - 'A'] == 1) {
						cmd_ModConsoleFunc->UnlockMission();
						state.i = 0;
					}
					if (cmd_keyState->AlphabetKey['W' - 'A'] == 1) {
						cmd_ModConsoleFunc->UnlockAllWeapon();
						state.i = 0;
					}
					if (cmd_keyState->AlphabetKey['Q' - 'A'] == 1) {
						cmd_ModConsoleFunc->MaxWeaponQuality();
						state.i = 0;
					}
				}

				if (state.b.get) {
					if (cmd_keyState->AlphabetKey['A' - 'A'] == 1) {
						ModConsole_GetArmorBox();
						state.i = 0;
					}
					if (cmd_keyState->AlphabetKey['W' - 'A'] == 1) {
						ModConsole_GetWeaponBox();
						state.i = 0;
					}
				}
			}
			// end
		}

	gotoSleep:
		Sleep(10);
	}
	//end
}

void cmd_ModConsole_SetKeyState(){
	// ctrl
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
		cmd_keyState->ctrl += 1;
	} else {
		cmd_keyState->ctrl = 0;
	}

	// alt
	if (GetAsyncKeyState(VK_MENU) & 0x8000) {
		cmd_keyState->alt += 1;
	} else {
		cmd_keyState->alt = 0;
	}

	// add
	if (GetAsyncKeyState(VK_ADD) & 0x8000) {
		cmd_keyState->Add += 1;
	} else {
		cmd_keyState->Add = 0;
	}

	// subtract
	if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {
		cmd_keyState->Subtract += 1;
	} else {
		cmd_keyState->Subtract = 0;
	}

	// F1-F12
	for (int i = 0; i < 12; ++i) {
		if (GetAsyncKeyState(VK_F1 + i) & 0x8000) {
			cmd_keyState->F_key[i] += 1;
		} else {
			cmd_keyState->F_key[i] = 0;
		}
	}

	// A-Z
	for (int i = 0; i < 26; ++i) {
		if (GetAsyncKeyState('A' + i) & 0x8000) {
			cmd_keyState->AlphabetKey[i] += 1;
		} else {
			cmd_keyState->AlphabetKey[i] = 0;
		}
	}
	// end
}

int ModConsoleStatus;
HUiModConsoleFunctionMap* pHuiModConsoleFunction;

extern "C" {
	void __fastcall ASMgetChatWideString();
	uintptr_t getChatWideStrinRetAddr;
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
	/*
	auto tempP = hmodEXE + 0x1256C40;
	pDXGISwapChainDesc = (DXGI_SWAP_CHAIN_DESC*)tempP;*/

	vedf125ABD8 = (uintptr_t)(hmodEXE + 0x125ABD8);

	void* pMemory = _aligned_malloc(sizeof(HUiModConsoleFunctionMap), 16U);
	if (pMemory) {
		pHuiModConsoleFunction = new(pMemory) HUiModConsoleFunctionMap();
		(*pHuiModConsoleFunction)[L"~kzt"] = ModConsole_DisableConsole;
		(*pHuiModConsoleFunction)[L"`111"] = ModConsole_CreateKeyMonitor;
		(*pHuiModConsoleFunction)[L"~GiveMeArmorBox"] = ModConsole_GetArmorBox;
		(*pHuiModConsoleFunction)[L"~GiveMeWeaponBox"] = ModConsole_GetWeaponBox;

		(*pHuiModConsoleFunction)[L"~WhosYourDaddy"] = (ModConsoleFuncPTR)(hmodEXE + 0x592F0);
		
		(*pHuiModConsoleFunction)[L"~UnlockAllWeapon"] = (ModConsoleFuncPTR)(hmodEXE + 0x610C0);
		// EDF5.exe+60F40, get_whole_clear_item, it has bug
		//(*pHuiModConsoleFunction)[L"~CallRecoveryTeam"] = (ModConsoleFuncPTR)(hmodEXE + 0x60F40);
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
		auto pDXGISwapChainDesc = DXGI_GetDXGISwapChainDesc();
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

void cmd_ModConsole_WriteToClipboard(const std::wstring& text)
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
	cmd_ModConsole_WriteToClipboard(text);
}

void __fastcall ModConsole_MK_GetCameraPos()
{
	auto pPlayerPos = ASMModConsole_GetCameraPostion(vedf125AB68);
	if (!pPlayerPos) {
		return;
	}

	std::wstring text = std::format(L"[ {:.3f}, {:.3f}, {:.3f} ]", pPlayerPos[0], pPlayerPos[1], pPlayerPos[2]);
	cmd_ModConsole_WriteToClipboard(text);
}

void cmd_ModConsole_GetPlayerPos(int hasOrientation) {
	auto pGameObject = ASMModConsole_GetPlayerAddress(vedf125AB68);
	if (!pGameObject) {
		return;
	}

	auto pPlayerPos = pGameObject->basePosMatrix;
	std::wstring text = std::format(L"            \"positions_1\":[ {:.3f}, {:.3f}, {:.3f} ]", pPlayerPos[3][0], pPlayerPos[3][1], pPlayerPos[3][2]);
	if (hasOrientation) {
		__m128 v_forward = _mm_loadu_ps(pPlayerPos[2]);
		__m128 v_length = _mm_sqrt_ps(_mm_dp_ps(v_forward, v_forward, 0b01111111));

		if (v_length.m128_f32[0] > 0.0001f) {
			v_forward = _mm_div_ps(v_forward, v_length);
		} else {
			v_forward = { 0.0f, 0.0f, 1.0f, 0.0f };
		}
		__m128 v_factor = _mm_set_ps1(10.0f);
		v_factor = _mm_mul_ps(v_factor, v_forward);

		__m128 v_pos = _mm_loadu_ps(pPlayerPos[3]);
		v_pos = _mm_add_ps(v_pos, v_factor);
		text += std::format(L",\n            \"positions_2\":[ {:.3f}, {:.3f}, {:.3f} ]", v_pos.m128_f32[0], v_pos.m128_f32[1], v_pos.m128_f32[2]);
	}
	cmd_ModConsole_WriteToClipboard(text);
}
