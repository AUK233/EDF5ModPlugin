#pragma once
#include <xmmintrin.h>
#include "CommonCustomStructure.hpp"
#include "EDFPointerStruct.hpp"

extern "C" {
void __fastcall ASMrva38A960mod(void* pObject, __m128* pStartPos, __m128* pR8, __m128* pR9, int length);
void __fastcall ASMrva3B4F40mod(void* pObject);
// LoadAudioToPointer
void __fastcall ASMrva47C6E0(void* pAudio, LPCWSTR FileName);
// LoadAudioToPointer
void __fastcall ASMrva47C6E0mod(void* pAudio, LPCWSTR FileName, int r8, int r9);
// pTail size is 0x210
void __fastcall ASMinitializeAmmoTailSmoke1(void* pAmmo, void* pTail, int tailLength, LPCWSTR tailTextureName, float tailSpeed);
//
size_t __fastcall ASMshowSubtitleByPlayingSoundReady(void* pScript, WCHAR* pAudioName, size_t nameSize);
void __fastcall ASMshowSubtitleByPlayingSoundOn(void* pScript, SubtitleTextStruct* startAddr);
void __fastcall ASMshowSubtitleByPlayingSoundOff(void* pScript, int subtitleIndex);
//
int __fastcall ASMgetCurrentMissionClassModelType();
}
