#pragma once
#include "EDFPointerStruct.h"


extern "C" {
HUiHudTextContentPointer* __fastcall ASMgetHUiHudTextContentPointer(void* pText);

void __fastcall hookSleep(DWORD time);
void __fastcall debugAllocateMemorySize(void* ptr);
void __fastcall debugGetWeaponName(EDFWeaponPointer* pWeapon);
size_t __fastcall TextForFormatFloatNumber(const float number, WCHAR *destination, size_t len);
size_t __fastcall TextForFormatFloatNumber2(const float number, WCHAR *destination, size_t len);
size_t __fastcall TextForFormatIntNumber(const int number, WCHAR* destination, size_t len);

size_t __fastcall eTextForWeaponReloadTime(EDFWeaponStruct *pweapon, WCHAR *destination, EDFColor4Pointer *pcolor);
//void __fastcall eDisplaySoldierWeaponAmmo(HUiHudWeaponPointer* p);
void __fastcall eDisplayFencerBoostAndDash(HUiHudPowerGuagePointer* p, FencerBoostAndDashStruct* fencer);

void __fastcall eDisplaySoldierWeaponDamage(HUiHudWeaponPointer* p);
void __fastcall eDisplayVehicleWeaponDamage(HUiHudWeaponPointer* p);

// fast get address
void __fastcall ASMgetPlayerAddress(uintptr_t startAddr, void* target);
// Get subtitle address
int __fastcall GetSubtitleTextAddress(SubtitleTextStruct* startAddr, WCHAR* pAudioName, int nameSize, int noPos);
// initialising talk subtitles
void __fastcall InitializeTalkSubtitleString();
void __fastcall PushTalkSubtitleString(WCHAR* pAudioName, size_t nameSize);
void __fastcall DisplayTalkSubtitleString(void* pScript);
}
void WINAPI getPlayerWeaponDamage();
std::wstring __fastcall FormatDamageNumber(const float dmg);
void __fastcall setDamageDisplayTime(int vstart, int vend, int time);

void InitialiseSubtitleFile(int language);
void hookDisplaySubtitle(int language);

void hookHUDEnhancement();
