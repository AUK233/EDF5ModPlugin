#pragma once
#include <unordered_map>
#include "EDFPointerStruct.hpp"

typedef void(__fastcall* ModConsoleFuncPTR)();
typedef std::unordered_map<std::wstring_view, ModConsoleFuncPTR> HUiModConsoleFunctionMap;

void module_UpdateHuiModConsole(PBYTE hmodEXE);
void module_InitializeHuiModConsole(PBYTE hmodEXE);

extern "C" {
	void __fastcall module_ActivateModConsole(PEDFWideString wstr);
}

void __fastcall ModConsole_DisableConsole();
void __fastcall ModConsole_GetArmorBox();
void __fastcall ModConsole_GetWeaponBox();
