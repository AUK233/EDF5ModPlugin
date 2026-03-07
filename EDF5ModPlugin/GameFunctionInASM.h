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
int __fastcall ASMgetCurrentMissionClassModelType();
// return 0 means no collision.
int __fastcall ASMLaserCollisionDetection(__m128* pEnd, const __m128* pStart);
}
