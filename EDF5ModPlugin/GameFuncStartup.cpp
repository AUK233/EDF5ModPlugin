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
#include "commonNOP.h"
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"


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
	// load audio
	uintptr_t rva478020;
	// read bgm file
	uintptr_t rva4781C0;
	// read audio file
	uintptr_t rva47C9D0;
	//
	uintptr_t vedf125AB68;
	uintptr_t vedf125ABA0;
	// maybe is mission data pointer
	uintptr_t vedf125AB30;
	//
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
	rva478020 = (uintptr_t)(hmodEXE + 0x478020);
	rva4781C0 = (uintptr_t)(hmodEXE + 0x4781C0);
	rva47C9D0 = (uintptr_t)(hmodEXE + 0x47C9D0);
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

#include "zHui/HuiDisplaySubtitle.h"

extern "C" {
void __fastcall ASMgameStartupReadVoiceFile();
uintptr_t gameStartupReadVoiceFileRetAddr;
uintptr_t ASMrva47C6E0modRetAddr;
void __fastcall ASMgameStartupReadMusicFile();
uintptr_t gameStartupReadMusicFileRetAddr;
void __fastcall ASMSoundControllerPlayBgm();
uintptr_t SoundControllerPlayBgmRetAddr;

void __fastcall ASMgameReadInvalidSGO();
}

void GameStartupHook(PBYTE hmodEXE)
{
	// EDF5.exe+3D6E8E, Load new voice files
	hookGameBlockWithInt3((void*)(hmodEXE + 0x3D6E8E), (uintptr_t)ASMgameStartupReadVoiceFile);
	WriteHookToProcess((void*)(hmodEXE + 0x3D6E8E + 15), (void*)&nop10, 10U);
	gameStartupReadVoiceFileRetAddr = (uintptr_t)(hmodEXE + 0x3D6EA7);

	// load voice files
	// EDF5.exe+47C6F4
	WriteHookToProcess((void*)(hmodEXE + 0x47C6F4), (void*)&nop8, 8U);
	WriteHookToProcess((void*)(hmodEXE + 0x47C6F4 + 8), (void*)&nop7, 7U);
	// EDF5.exe+47C7E3
	WriteHookToProcess((void*)(hmodEXE + 0x47C7E3), (void*)&nop5, 5U);
	WriteHookToProcess((void*)(hmodEXE + 0x47C7E3 + 5), (void*)&nop8, 8U);
	//
	ASMrva47C6E0modRetAddr = (uintptr_t)(hmodEXE + 0x47C74F);

	// here has problem
	// EDF5.exe+3D6D1E, should be add 0x88, but 16-byte aligned
	int newAuidoPointerSize = 0xD0 + 0x90;
	//WriteHookToProcessCheckECX((void*)(hmodEXE + 0x3D6D1E + 1), &newAuidoPointerSize, 4U);
	// load music files
	// EDF5.exe+47B5E5
	//hookGameBlockWithInt3((void*)(hmodEXE + 0x47B5E5), (uintptr_t)ASMgameStartupReadMusicFile);
	//WriteHookToProcess((void*)(hmodEXE + 0x47B5E5 + 15), (void*)&nop2, 2U);
	gameStartupReadMusicFileRetAddr = (uintptr_t)(hmodEXE + 0x47B5F6);

	// play bgm, playing voice as MUSIC is wrong!
	// EDF5.exe+11B9E2
	//hookGameBlockWithInt3((void*)(hmodEXE + 0x11B9E2), (uintptr_t)ASMSoundControllerPlayBgm);
	//WriteHookToProcess((void*)(hmodEXE + 0x11B9E2 + 15), (void*)&nop2, 2U);
	SoundControllerPlayBgmRetAddr = (uintptr_t)(hmodEXE + 0x11B9F3);


	// EDF5.exe+613A12, Throw invalid filename
	hookGameBlockRAXWithInt3((void*)(hmodEXE + 0x613A12), (uintptr_t)ASMgameReadInvalidSGO);

	hookDisplaySubtitle(hmodEXE);
}

extern "C" {
	void __fastcall ASMgameStartupUnlockAllWeapon();
	uintptr_t gameStartupUnlockAllWeaponRetAddr;
	void __fastcall ASMgameStartupSetMinClassArmor();
	uintptr_t gameStartupSetMinClassArmorRetAddr;
}

void GameStartupUnlock(PBYTE hmodEXE)
{
	// EDF5.exe+8CB20, Unlock all weapons for new players (0 stars, activate once per run)
	hookGameBlockWithInt3((void*)(hmodEXE + 0x8CB20), (uintptr_t)ASMgameStartupUnlockAllWeapon);
	WriteHookToProcess((void*)(hmodEXE + 0x8CB20 + 15), (void*)&nop2, 2U);
	gameStartupUnlockAllWeaponRetAddr = (uintptr_t)(hmodEXE + 0x8CB3F);
	// EDF5.exe+5091F8, Set current class minimum HP to 30,000 boxes.
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5091F8), (uintptr_t)ASMgameStartupSetMinClassArmor);
	WriteHookToProcess((void*)(hmodEXE + 0x5091F8 + 15), (void*)&nop1, 1U);
	gameStartupSetMinClassArmorRetAddr = (uintptr_t)(hmodEXE + 0x509208);
}

void __fastcall LoadNewVoiceFilesCPP(void* pAudio)
{
	if (std::filesystem::exists(L"./sound/pc/add_audio41_main.awb")) {
		ASMrva47C6E0(pAudio, L"app:/sound/adx/add_audio41_main.awb");
		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading EDF4.1 voice file with increased volume";
		}
	}
	else if (std::filesystem::exists(L"./sound/pc/tikyuu4_voice_en.awb")) {
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
	// edf6 main
	if (std::filesystem::exists(L"./sound/pc/add_audio6_main.awb")) {
		ASMrva47C6E0(pAudio, L"app:/sound/adx/add_audio6_main.awb");
		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading EDF6 voice file";
		}
	}
	// edf6 dlc 1
	if (std::filesystem::exists(L"./sound/pc/add_audio6_dlc1.awb")) {
		ASMrva47C6E0(pAudio, L"app:/sound/adx/add_audio6_dlc1.awb");
		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading EDF6 Mission Pack 1 voice file";
		}
	}
	// test file
	if (std::filesystem::exists(L"./sound/pc/test6.awb")) {
		ASMrva47C6E0(pAudio, L"app:/sound/adx/test6.awb");
		//ASMrva47C6E0mod(pAudio, L"app:/sound/adx/test6.awb",1,2);
		if (ModLogStatus == 1) {
			PLOG_INFO << "Loading test voice file";
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
