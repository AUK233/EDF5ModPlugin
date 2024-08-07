#pragma once

void GameStartupInitialization(PBYTE hmodEXE);
void GetGameGlobalPointer(PBYTE hmodEXE);
void GameStartupHook(PBYTE hmodEXE);
void GameStartupUnlock(PBYTE hmodEXE);

extern "C" {
void __fastcall LoadNewVoiceFilesCPP(void* pAudio);
void __fastcall ThrowInvalidSGOFilenameCPP(std::wstring filename);
}
