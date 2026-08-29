#pragma once
#include <unordered_map>
#include "EDFPointerStruct.hpp"

typedef struct cmd_KeyState_t {
	int ctrl, alt;
	int Add, Subtract;
	int AlphabetKey[26];
	int F_key[12];
} *Pcmd_KeyState;
typedef void(__fastcall* ModConsoleFuncPTR)();

void cmd_ModConsole_Initialize(PBYTE hmodEXE);
void cmd_ModConsole_MonitorKeys();
void cmd_ModConsole_SetKeyState();

typedef std::unordered_map<std::wstring_view, ModConsoleFuncPTR> HUiModConsoleFunctionMap;

void module_UpdateHuiModConsole(PBYTE hmodEXE);
void module_InitializeHuiModConsole(PBYTE hmodEXE);

extern "C" {
	void __fastcall module_ActivateModConsole(PEDFWideString wstr);
	PEDFBaseClass __fastcall ASMModConsole_GetPlayerAddress(uintptr_t p);
	float* __fastcall ASMModConsole_GetCameraPostion(uintptr_t p);
}

void __fastcall ModConsole_DisableConsole();
void __fastcall ModConsole_GetArmorBox();
void __fastcall ModConsole_GetWeaponBox();
void __fastcall ModConsole_CreateKeyMonitor();

void ModConsole_MonitorKeys();
void ModConsole_WriteToClipboard(const std::wstring& text);
void __fastcall ModConsole_MK_GetPlayerPos();
void __fastcall ModConsole_MK_GetCameraPos();
