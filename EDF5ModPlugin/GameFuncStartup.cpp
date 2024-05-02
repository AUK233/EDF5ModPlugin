#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>
#include <filesystem>
#include "utiliy.h"
#include "GameFunctionInASM.h"

#include "GameFuncStartup.h"
#include <commonNOP.h>
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

extern "C" extern int ModLogStatus;

void GameStartupInitialization(PBYTE hmodEXE)
{
	GetGameGlobalPointer(hmodEXE);
	GameStartupHook(hmodEXE);
}

extern "C" {
	// ReadWString
	uintptr_t rva27380;
	// UpdateWStringPointer
	uintptr_t rva27570;
	//
	uintptr_t rva42460;
	// Int2WString(void* dst, int)
	uintptr_t rvaB7220;
	// Int to WString?
	uintptr_t rva4D86D0;
	// UpdateText
	uintptr_t rva4CA990;
	// LoadAudioToPointer
	uintptr_t rva47C6E0;
	//
	uintptr_t vedf125AB68;
	uintptr_t vedf125ABA0;
	uintptr_t vedf125AB30;
	uintptr_t vedf1259640;
	uintptr_t vedf1259680;
	uintptr_t vedf125B080;
	uintptr_t vedf1153550;
	uintptr_t vedf1256BD0;
	// Audio Pointer
	uintptr_t vedf125AB90;
}

void GetGameGlobalPointer(PBYTE hmodEXE)
{
	//
	rva27380 = (uintptr_t)(hmodEXE + 0x27380);
	rva27570 = (uintptr_t)(hmodEXE + 0x27570);
	rva42460 = (uintptr_t)(hmodEXE + 0x42460);
	rvaB7220 = (uintptr_t)(hmodEXE + 0xB7220);
	rva4D86D0 = (uintptr_t)(hmodEXE + 0x4D86D0);
	rva4CA990 = (uintptr_t)(hmodEXE + 0x4CA990);
	rva47C6E0 = (uintptr_t)(hmodEXE + 0x47C6E0);
	//
	vedf125AB68 = (uintptr_t)(hmodEXE + 0x125AB68);
	vedf125ABA0 = (uintptr_t)(hmodEXE + 0x125ABA0);
	vedf125AB30 = (uintptr_t)(hmodEXE + 0x125AB30);
	vedf1259640 = (uintptr_t)(hmodEXE + 0x1259640);
	vedf1259680 = (uintptr_t)(hmodEXE + 0x1259680);
	vedf125B080 = (uintptr_t)(hmodEXE + 0x125B080);
	vedf1153550 = (uintptr_t)(hmodEXE + 0x1153550);
	vedf1256BD0 = (uintptr_t)(hmodEXE + 0x1256BD0);
	vedf125AB90 = (uintptr_t)(hmodEXE + 0x125AB90);
}

extern "C" {
void __fastcall ASMgameStartupReadVoiceFile();
uintptr_t gameStartupReadVoiceFileRetAddr;
void __fastcall ASMgameReadInvalidSGO();
}

void GameStartupHook(PBYTE hmodEXE)
{
	// EDF5.exe+3D6E8E, Load new voice files
	hookGameBlockWithInt3((void*)(hmodEXE + 0x3D6E8E), (uintptr_t)ASMgameStartupReadVoiceFile);
	WriteHookToProcess((void*)(hmodEXE + 0x3D6E8E + 15), (void*)&nop10, 10U);
	gameStartupReadVoiceFileRetAddr = (uintptr_t)(hmodEXE + 0x3D6EA7);

	// EDF5.exe+613A12, Throw invalid filename
	hookGameBlockRAXWithInt3((void*)(hmodEXE + 0x613A12), (uintptr_t)ASMgameReadInvalidSGO);
}

void __fastcall LoadNewVoiceFilesCPP(void* pAudio)
{
	if (std::filesystem::exists(L"./sound/pc/tikyuu4_voice_en.awb")) {
		ASMrva47C6E0(pAudio, L"app:/sound/adx/tikyuu4_voice_en.awb");
		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading EDF4.1 voice file (English)";
		}
	}
	else if (std::filesystem::exists(L"./sound/pc/tikyuu4_voice.awb")) {
		ASMrva47C6E0(pAudio, L"app:/sound/adx/tikyuu4_voice.awb");
		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading EDF4.1 voice file (Japenese)";
		}
	}
	else {
		if (ModLogStatus == 1) {
			PLOG_INFO << "No EDF4.1 voice file is loaded";
		}
	}
}

void __fastcall ThrowInvalidSGOFilenameCPP(std::wstring filename)
{
	if (ModLogStatus == 1) {
		PLOG_ERROR << "SGO File: " << filename << " doesn't exist";
	}
	else {
		std::wstring message = L"SGO File doesn't exist: ";
		message += filename;
		message += L"\nGame will crash!";

		HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ForceCrashGame, NULL, NULL, NULL);
		if (tempHND) {
			CloseHandle(tempHND);
		}

		MessageBoxW(NULL, message.c_str(), L"error", MB_OK);
	}
	// crash!
	*(int*)0 = 0;
}
