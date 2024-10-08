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
#include "shlwapi.h"

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include "utiliy.h"

#include "commonNOP.h"
#include "GFnDisplay.h"
#include <GameFunctionInASM.h>

extern PBYTE hmodEXE;


__declspec(align(16)) const wchar_t DMGstrSpace16[] = L"               ";
__declspec(align(16)) const wchar_t DMGstrSpace112[] = L"               \n               \n               \n               \n               \n               \n                ";

extern "C" {
extern int displayDamageIndex;
extern int ModLogStatus;
extern int HUDEnhanceStatus;
}

void __fastcall hookSleep(DWORD time) {
	Sleep(time);
}

void __fastcall debugAllocateMemorySize(void* ptr)
{
	size_t MemerySize = _aligned_msize(ptr, 0x10, 0);
	PLOG_INFO << "allocated memory size: 0x" << std::hex << MemerySize;
}

void __fastcall debugGetWeaponName(EDFWeaponPointer* pWeapon) {
	wchar_t wstr[64];
	wcscpy_s(wstr, pWeapon->WeaponName);
	PLOG_INFO << "weapon name: " << wstr;

	return debugAllocateMemorySize(pWeapon);
}

// __forceinline
size_t __fastcall TextForFormatFloatNumber(const float number, WCHAR *destination, size_t len) {
	std::wstring wstr;
	if (number >= 100.0f) {
		wstr = std::format(L"{:.0f}", number);
	} else if (number >= 10.0f) {
		wstr = std::format(L"{:.1f}", number);
	} else {
		wstr = std::format(L"{:.2f}", number);
	}

	if (wstr.size() <= len) {
		memcpy(destination, wstr.c_str(), wstr.size() * 2);
		return wstr.size();
	} else {
		memcpy(destination, wstr.c_str(), len * 2);
		INT16 nullStr = 0;
		memcpy(destination + len, &nullStr, 2U);
		return len;
	}
}

size_t __fastcall TextForFormatFloatNumber2(const float number, WCHAR *destination, size_t len) {
	std::wstring wstr;
	if (number >= 100.0f) {
		wstr = std::format(L"{:.0f}", number);
	} else if (number >= 10.0f) {
		wstr = std::format(L"{:.1f}", number);
	} else {
		wstr = std::format(L"{:.2f}", number);
	}
	size_t wstrSize = wstr.size();
	wchar_t *textS = destination + 8;
	if (wstrSize <= len) {
		memcpy(destination, wstr.c_str(), wstrSize * 2);
		memcpy(destination + wstrSize, textS, 4U);
		return (wstrSize + 1);
	} else {
		memcpy(destination, wstr.c_str(), len * 2);
		memcpy(destination + len, textS, 4U);
		return (len + 1);
	}
}

size_t __fastcall TextForFormatIntNumber(const int number, WCHAR* destination, size_t len)
{
	std::wstring wstr = std::format(L"{0}", number);

	if (wstr.size() <= len) {
		memcpy(destination, wstr.c_str(), wstr.size() * 2);
		return wstr.size();
	}
	else {
		memcpy(destination, wstr.c_str(), len * 2);
		INT16 nullStr = 0;
		memcpy(destination + len, &nullStr, 2U);
		return len;
	}
}

constexpr float WeaponReloadTimeColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
constexpr float WeaponChargeColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
constexpr float WeaponReloadPercentColor[4] = { 1.0f, 1.0f, 1.0f, 0.5f };

size_t __fastcall eTextForWeaponReloadTime(EDFWeaponStruct *pweapon, WCHAR *destination, EDFColor4Pointer *pcolor) {
	ZeroMemory(destination, 16U);

	if (pweapon->reloadType == 1) {
		if (pweapon->reloadTime== -1 || pweapon->reloadTimeCount == pweapon->reloadTime) {
			pcolor->a = 0;
			return 1;
		}
		else {
			goto ReloadTimeBlock;
		}
	}

	if (pweapon->reloadType == 2) {
		memcpy(pcolor, WeaponReloadPercentColor, 16U);
		float reloadProgress = pweapon->reloadTime - pweapon->reloadTimeCount;
		reloadProgress /= pweapon->reloadTime;
		reloadProgress *= 100.0f;
		wchar_t textS[] = L"��";
		memcpy(destination + 8, textS, 4U);
		return TextForFormatFloatNumber2(reloadProgress, destination, 7);
	}

	if (pweapon->EnergyChargeRequire > 0.0f) {
		memcpy(pcolor, WeaponChargeColor, 16U);
		float chargeProgress = pweapon->EnergyChargeRequire - pweapon->EnergyChargeCount;
		chargeProgress /= pweapon->EnergyChargeRequire;
		chargeProgress *= 100.0f;
		wchar_t textS[] = L"��";
		memcpy(destination + 8, textS, 4U);
		return TextForFormatFloatNumber2(chargeProgress, destination, 7);
	}

	ReloadTimeBlock:
	memcpy(pcolor, WeaponReloadTimeColor, 16U);
	float remainTime = pweapon->reloadTimeCount / 60.0f;
	return TextForFormatFloatNumber(remainTime, destination, 7);
}

/*
void __fastcall eDisplaySoldierWeaponAmmo(HUiHudWeaponPointer* p)
{
	EDFWeaponPointer* pWeapon = p->Weapon;
	HUiHudTextPointer* pTextNumeric = p->TextNumericType2;
	if (pWeapon->use_extraShotType == 16 || pWeapon->addr1452) {
		if (pWeapon->curAmmo > 0) {
			pTextNumeric->font_color = p->TextNumericType2Color;
			HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(pTextNumeric->addr228h);
			pText->text = std::format(L"{0}", pWeapon->curAmmo);
			pText->textLength = pText->text.size();
		}
		else {

		}
	}
	else {
		pTextNumeric->font_color.a = 0;
	}
}
*/

void __fastcall eDisplayFencerBoostAndDash(HUiHudPowerGuagePointer* p, FencerBoostAndDashStruct* fencer)
{
	HUiHudTextPointer* pDash = p->TextFencerDash;
	if (pDash) {
		HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(pDash->addr228h);
		ZeroMemory(&pText->text, 16U);
		if (fencer->DashCurrentCount > 0) {
			memcpy(&pDash->font_color, &p->TextFencerDashColor, 16U);
			TextForFormatIntNumber(fencer->DashCurrentCount, (WCHAR*)&pText->text, 7);
		}else{
			memcpy(&pDash->font_color, WeaponReloadTimeColor, 16U);
			float remainTime = fencer->DashRecoveryTime / 60.0f;
			TextForFormatFloatNumber(remainTime, (WCHAR*)&pText->text, 7);
		}
	}

	HUiHudTextPointer* pBoost = p->TextFencerBoost;
	if (pBoost) {
		HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(pBoost->addr228h);
		ZeroMemory(&pText->text, 16U);
		if (fencer->BoostCurrentCount > 0) {
			memcpy(&pBoost->font_color, &p->TextFencerBoostColor, 16U);
			TextForFormatIntNumber(fencer->BoostCurrentCount, (WCHAR*)&pText->text, 7);
		}
		else {
			memcpy(&pBoost->font_color, WeaponReloadTimeColor, 16U);
			float remainTime = fencer->BoostRecoveryTime / 60.0f;
			TextForFormatFloatNumber(remainTime, (WCHAR*)&pText->text, 7);
		}
	}
}

extern "C" {
uintptr_t hookTextDisplayRetAddr;
void __fastcall ASMhookTextDisplay();

void __fastcall ASMrecordPlayerDamage();
uintptr_t playerDmgRetAddress;
uintptr_t playerAddress[2] = {0,0};
float damageTempValue[2] = {0,0};
void __fastcall ASMresetPlayerDamageTemp(float* ptr, UINT64 Zero);
}

// set damage display duration (short)
constexpr int DAMAGE_DISPLAY_TIME_S = 3.6 * 100;
// set rechargeable damage display duration (short)
constexpr int DAMAGE_CHARGE_TIME_S = 0.5 * 100;

struct GetDamageStruct {
	float value;
	int time;
	int status;
	UINT32 align;
};
union ShowDamageStruct
{
	BYTE on[8];
	UINT64 status;
};
// separate damage
GetDamageStruct damageNumber;
// damage group
GetDamageStruct dmgNumGroup[8];
ShowDamageStruct dmgCheckGroup;

std::wstring __fastcall FormatDamageNumber(const float dmg) {
	if (dmg >= 1000.0f) {
		return std::format(L"{:.0f}", dmg);
	} else if (dmg >= 100.0f) {
		return std::format(L"{:.1f}", dmg);
	} else {
		return std::format(L"{:.2f}", dmg);
	}
}

void __fastcall setDamageDisplayTime(int vstart, int vend, int time) {
	for (int i = vstart; i < vend; i++) {
		if (dmgNumGroup[i + 1].time > 0) {
			dmgNumGroup[i].value = dmgNumGroup[i + 1].value;
			dmgNumGroup[i].time = dmgNumGroup[i + 1].time;
			dmgCheckGroup.on[i] = 1;
		}
	}
	dmgNumGroup[vend].value = damageNumber.value;
	dmgNumGroup[vend].time = time;
	dmgCheckGroup.on[vend] = 1;
}

void WINAPI getPlayerWeaponDamage() {
	while (1) {
		ASMgetPlayerAddress((uintptr_t)(hmodEXE + 0x125AB68), &playerAddress);

		if (playerAddress[0] > 0xFFFF || playerAddress[1] > 0xFFFF) {
			if (damageTempValue[0]) {
				if (damageNumber.time < 1) {
					damageNumber.value = damageTempValue[0];
				}
				else {
					damageNumber.value += damageTempValue[0];
				}
				damageNumber.time = DAMAGE_CHARGE_TIME_S;
				damageNumber.status = 1;
				damageTempValue[0] = 0;
				//damageTempValue[1] = 0;
			}

			if (damageNumber.time > 0) {
				damageNumber.time += -1;
			} else if (damageNumber.status) {
				setDamageDisplayTime(0, 6, DAMAGE_DISPLAY_TIME_S);
				damageNumber.status = 0;
			} else {
				damageNumber.value = 0;
			}

			for (int i = 0; i < 7; i++) {
				if (dmgNumGroup[i].time > 0) {
					dmgNumGroup[i].time += -1;
				}
				else {
					dmgNumGroup[i].value = 0;
					dmgCheckGroup.on[i] = 0;
				}
			}
		}
		else {
			damageNumber.time = 0;
			damageNumber.status = 0;
			dmgCheckGroup.status = 0;
		}

		Sleep(10);
	}
}

void __fastcall eDisplaySoldierWeaponDamage(HUiHudWeaponPointer* p)
{
	// Soldier
	if (displayDamageIndex != 1 || p->Weapon->addr1452 == 0) {
		p->TextDamage->font_color.a = 0;
		if (p->TextDamageUP) {
			p->TextDamageUP->font_color.a = 0;
		}
		return;
	}

	if (damageNumber.time > 0) {
		p->TextDamage->font_color.a = 1.0f;
		HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(p->TextDamage->addr228h);
		float fontSize = damageNumber.time / 50.0f;
		fontSize += p->damageFontSize.x;
		pText->fontSize.x = fontSize;
		pText->fontSize.y = fontSize;
		memcpy(pText->text, DMGstrSpace16, 32U);
		std::wstring displayText = FormatDamageNumber(damageNumber.value);
		size_t strofs = 0;
		size_t strsize = 30;
		if (displayText.size() < 15) {
			strofs = (15 - displayText.size()) / 4;
			strsize = displayText.size() * 2;
		}
		memcpy((pText->text + strofs), displayText.c_str(), strsize);
	}
	else {
		p->TextDamage->font_color.a = 0;
	}

	if (p->TextDamageUP) {
		if (dmgCheckGroup.status) {
			p->TextDamageUP->font_color.a = 1.0f;
			HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(p->TextDamageUP->addr228h);

			memcpy(pText->text, DMGstrSpace112, 224U);

			for (int i = 0; i < 7; i++) {
				if (dmgNumGroup[i].time > 0) {

					std::wstring displayText = FormatDamageNumber(dmgNumGroup[i].value);

					size_t strofs = 0;
					size_t strsize = 30;
					if (displayText.size() < 15) {
						strofs = (15 - displayText.size()) / 2;
						strsize = displayText.size() * 2;
					}
					/*
					std::wstring displayText = L"1234567.7654321";
					size_t strofs = 0;
					size_t strsize = 30;*/
					strofs += (size_t)i * 16;
					memcpy((pText->text + strofs), displayText.c_str(), strsize);
				}
			}
			// end
		}
		else {
			p->TextDamageUP->font_color.a = 0;
		}
	}
	// Soldier end
}

void __fastcall eDisplayVehicleWeaponDamage(HUiHudWeaponPointer* p)
{
	if (displayDamageIndex != 1) {
		p->TextDamage->font_color.a = 0;
		if (p->TextDamageUP) {
			p->TextDamageUP->font_color.a = 0;
		}
		return;
	}

	// Vehicle
	if (damageNumber.time > 0) {
		p->TextDamage->font_color.a = 1.0f;
		HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(p->TextDamage->addr228h);
		float fontSize = damageNumber.time / 50.0f;
		fontSize *= p->damageFontSize.x;
		fontSize += p->damageFontSize.x;
		pText->fontSize.x = fontSize;
		pText->fontSize.y = fontSize;

		memcpy(pText->text, DMGstrSpace16, 32U);
		std::wstring displayText = FormatDamageNumber(damageNumber.value);
		size_t strofs = 0;
		size_t strsize = 30;
		if (displayText.size() < 15) {
			strsize = displayText.size() * 2;
		}
		memcpy((pText->text + strofs), displayText.c_str(), strsize);
	}
	else {
		p->TextDamage->font_color.a = 0;
	}

	if (p->TextDamageUP) {
		if (dmgCheckGroup.status) {
			p->TextDamageUP->font_color.a = 1.0f;
			HUiHudTextContentPointer* pText = ASMgetHUiHudTextContentPointer(p->TextDamageUP->addr228h);

			memcpy(pText->text, DMGstrSpace112, 224U);

			for (int i = 0; i < 7; i++) {
				if (dmgNumGroup[i].time > 0) {

					std::wstring displayText = FormatDamageNumber(dmgNumGroup[i].value);

					size_t strofs = 0;
					size_t strsize = 30;
					if (displayText.size() < 15) {
						strsize = displayText.size() * 2;
					}
					strofs += (size_t)i * 16;
					memcpy((pText->text + strofs), displayText.c_str(), strsize);
				}
			}
			// end
		}
		else {
			p->TextDamageUP->font_color.a = 0;
		}
	}
	// Vehicle end
}

extern "C" {
	char* pSubtitleFile = 0;
	uint32_t pSubtitleIndex = 0;
}

std::vector<SubtitleTextStruct> v_SubtitleText;
std::vector<SubtitleTextStruct> v_SubtitleName;
std::wstring wstr_SubtitleText;
int SubtitleTextMinSize = 0;
int SubtitleTextSpace = 1;

int __fastcall GetSubtitleTextAddress(SubtitleTextStruct* startAddr, WCHAR* pAudioName, int nameSize, int noPos)
{
	int textIndex = -1;
	for (int i = 0; i < v_SubtitleName.size(); i++) {
		if (v_SubtitleName[i].size == nameSize) {
			if (!StrCmpW(v_SubtitleName[i].text, pAudioName)) {
				textIndex = v_SubtitleName[i].v.id;
				break;
			}
		}
	}
	//
	if (textIndex == -1) {
		startAddr->size = 0;
		return 0;
	}
	else {
		float curPos = 320.0f;
		if (!noPos) {
			int curSize = v_SubtitleText[textIndex].size / 2;
			int minSize = SubtitleTextMinSize;
			if (curSize < minSize) {
				float fSize = minSize - curSize;
				float addPos = (fSize / minSize) * 640.0f;
				curPos += addPos;
			}
		}

		startAddr->v.posX = curPos;
		startAddr->text = v_SubtitleText[textIndex].text;
		startAddr->size = v_SubtitleText[textIndex].size;
		return v_SubtitleText[textIndex].size;
	}
}

void __fastcall InitializeTalkSubtitleString()
{
	wstr_SubtitleText = L"";
}

void __fastcall PushTalkSubtitleString(WCHAR* pAudioName, size_t nameSize)
{
	SubtitleTextStruct text;
	int size = GetSubtitleTextAddress(&text, pAudioName, nameSize, 1);

	if (size > 0) {
		if (wstr_SubtitleText != L"") {
			wstr_SubtitleText += L"\n";
		}

		int curSize = size/2;
		int minSize = SubtitleTextMinSize;
		if (curSize < minSize) {
			minSize -= curSize;
			minSize /= SubtitleTextSpace;
			for (int i = 0; i < minSize; i++) {
				wstr_SubtitleText += L"��";
			}
		}

		wstr_SubtitleText += text.text;
	}
}

void __fastcall DisplayTalkSubtitleString(void* pScript)
{
	uint32_t Index = pSubtitleIndex;
	if (Index) {
		ASMshowSubtitleByPlayingSoundOff(pScript, Index);
	}

	SubtitleTextStruct text;
	text.v.posX = 320.0f;
	text.size = wstr_SubtitleText.size();
	text.text = (WCHAR*)wstr_SubtitleText.c_str();

	ASMshowSubtitleByPlayingSoundOn(pScript, &text);
}


void InitialiseSubtitleFile(int language)
{
	std::wstring filePath = L"./subtitle/";
	switch (language)
	{
	case 1:
		// 86 / 2
		filePath += L"voice_subtitle_EN.sgo";
		SubtitleTextMinSize = 43;
		SubtitleTextSpace = 2;
		break;
	case 2:
		filePath += L"voice_subtitle_JA.sgo";
		// 37 / 2
		SubtitleTextMinSize = 18;
		SubtitleTextSpace = 1;
		break;
	case 3:
		filePath += L"voice_subtitle_CN.sgo";
		// 44 / 2
		SubtitleTextMinSize = 22;
		SubtitleTextSpace = 1;
		break;
	case 4:
		filePath += L"voice_subtitle_KR.sgo";
		SubtitleTextMinSize = 11;
		break;
	default:
		filePath += L"test.sgo";
		SubtitleTextMinSize = 22;
		break;
	}

	std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate | std::ios::in);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	pSubtitleFile = (char*)_aligned_malloc(size, 0x10);
	if (pSubtitleFile) {
		file.read(pSubtitleFile, size);
		char* p = pSubtitleFile;
		// read sgo
		int DataNodeCount = *(int*)(p + 8);
		int DataNodeOffset = *(int*)(p + 0xC);
		int DataNameCount = *(int*)(p + 0x10);
		int DataNameOffset = *(int*)(p + 0x14);
		int nodepos, nameoffset;
		// read node name
		v_SubtitleName.resize(DataNameCount);
		for (int i = 0; i < DataNameCount; i++)
		{
			nodepos = DataNameOffset + (i * 0x8);
			nameoffset = *(int*)(p + nodepos);

			v_SubtitleName[i].text = (WCHAR*)(p + nodepos + nameoffset);
			v_SubtitleName[i].v.id = *(int*)(p + nodepos + 4);
			v_SubtitleName[i].size = wcslen(v_SubtitleName[i].text);
		}
		// read node wstring
		int strsize, stroffset;
		v_SubtitleText.resize(DataNodeCount);
		for (int i = 0; i < DataNodeCount; i++) {
			nodepos = DataNodeOffset + (i * 0xC);
			strsize = *(int*)(p + nodepos + 4);
			v_SubtitleText[i].size = strsize;
			if (strsize > 0) {
				stroffset = *(int*)(p + nodepos + 8);
				v_SubtitleText[i].text = (WCHAR*)(p + nodepos + stroffset);
			}
			else {
				v_SubtitleText[i].text = 0;
			}
		}
	}
	file.close();
}

extern "C" {
	void __fastcall ASMscript2C_FA0();
	uintptr_t script2C_FA0ret;
	void __fastcall ASMscript2C_FA2();
	void __fastcall ASMscript2C_1004();
	void __fastcall ASMscript2C_1005();
	uintptr_t script2C_1005ret;
	void __fastcall ASMscript2C_1006();
	uintptr_t script2C_1006ret;
	void __fastcall ASMscript2C_1007();
	uintptr_t script2C_1007ret;
	void __fastcall ASMscript2C_1009();
	uintptr_t script2C_1009ret;
}

void hookDisplaySubtitle(int language)
{
	InitialiseSubtitleFile(language);

	// EDF5.exe+127CC0, play voice
	hookGameBlockWithInt3((void*)(hmodEXE + 0x127CC0), (uintptr_t)ASMscript2C_FA0);
	WriteHookToProcess((void*)(hmodEXE + 0x127CC0 + 15), (void*)&nop2, 2U);
	script2C_FA0ret = (uintptr_t)(hmodEXE + 0x127CEF);

	// EDF5.exe+116708, wait voice
	hookGameBlockWithInt3((void*)(hmodEXE + 0x116708), (uintptr_t)ASMscript2C_FA2);
	WriteHookToProcess((void*)(hmodEXE + 0x116708 + 15), (void*)&nop1, 1U);

	// talk series
	// EDF5.exe+1280B1, talk ready
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1280B1), (uintptr_t)ASMscript2C_1004);
	// EDF5.exe+1281ED, play talk
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1281ED), (uintptr_t)ASMscript2C_1005);
	script2C_1005ret = (uintptr_t)(hmodEXE + 0x1281FC);

	// EDF5.exe+12833A, talk push
	hookGameBlockWithInt3((void*)(hmodEXE + 0x12833A), (uintptr_t)ASMscript2C_1006);
	WriteHookToProcess((void*)(hmodEXE + 0x12833A + 15), (void*)&nop2, 2U);
	script2C_1006ret = (uintptr_t)(hmodEXE + 0x12834B);

	// EDF5.exe+1284BA, talk push
	hookGameBlockWithInt3((void*)(hmodEXE + 0x1284BA), (uintptr_t)ASMscript2C_1007);
	WriteHookToProcess((void*)(hmodEXE + 0x1284BA + 15), (void*)&nop2, 2U);
	script2C_1007ret = (uintptr_t)(hmodEXE + 0x1284CB);

	// EDF5.exe+12880A, talk push
	hookGameBlockWithInt3((void*)(hmodEXE + 0x12880A), (uintptr_t)ASMscript2C_1009);
	WriteHookToProcess((void*)(hmodEXE + 0x12880A + 15), (void*)&nop2, 2U);
	script2C_1009ret = (uintptr_t)(hmodEXE + 0x12881B);
}

extern "C" {
	void __fastcall ASMhookSleep();
	uintptr_t hookSleepRet;
	//
	void __fastcall ASMreadHUiHudWeapon();
	void __fastcall ASMHUiHudWeaponUpdateVehicleText();
	uintptr_t HUiHudWeaponUpdateVehicleTextRet;
	void __fastcall ASMHUiHudWeaponUpdateAmmoText();
	//
	void __fastcall ASMreadHUiHudPowerGuage();
	uintptr_t readHUiHudPowerGuageRet;
	void __fastcall ASMupdateHUiHudPowerGuage();
	uintptr_t updateHUiHudPowerGuageRet;
}

void hookHUDEnhancement() {
	//hookGameBlock((void*)(hmodEXE + 0x60FDEA), (uint64_t)ASMhookSleep);
	//hookSleepRet = (uintptr_t)(hmodEXE + 0x60FE0F);

	HUDEnhanceStatus = 1;
	// EDF5.exe+2DB61F
	hookGameBlock((void*)(hmodEXE + 0x2DB61F), (uintptr_t)ASMrecordPlayerDamage);
	WriteHookToProcess((void*)(hmodEXE + 0x2DB61F + 12), (void*)&nop6, 6U);
	playerDmgRetAddress = (uintptr_t)(hmodEXE + 0x2DB77C);
	//
	HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)getPlayerWeaponDamage, NULL, NULL, NULL);
	if (tempHND) {
		CloseHandle(tempHND);
	}
	//
	WriteHookToProcess((void*)(hmodEXE + 0xECB740), (void*)L"lyt_HudWeaponGuagL1.sgo", 48U);
	WriteHookToProcess((void*)(hmodEXE + 0xECB820), (void*)L"lyt_HudWeaponGuagR1.sgo", 48U);
	WriteHookToProcess((void*)(hmodEXE + 0xECB7D0), (void*)L"lyt_HudEnergyGuageR1.sgo", 48U);
	WriteHookToProcess((void*)(hmodEXE + 0xEC8F90), (void*)L"lyt_HudWeaponGuageVehicl1.sgo", 60U);
	WriteHookToProcess((void*)(hmodEXE + 0xEC8F50), (void*)L"lyt_HudPowerGuageM1.sgo", 48U);
	
	// EDF5.exe+3532C8
	// this has problem, since EDF5.exe+34C8EA
	BYTE ofs3526C1[] = {
		0x49, 0x8B, 0x9D, 0x18, 0x04, 0x00, 0x00, // mov rbx, [r13+418h]
		0x48, 0x81, 0xC3, 0xC0, 0x00, 0x00, 0x00, // add rbx, 0C0h
		0xEB, 0x0C,                               // jmp
		0x0F, 0x1F, 0x00,
		0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	//WriteHookToProcess((void *)(hmodEXE + 0x3532C1), &ofs3526C1, 28U);

	int newWeaponSize = 0xE00;
	static_assert(sizeof(HUiHudWeaponPointer) < 0xE00);
	WriteHookToProcessCheckECX((void *)(hmodEXE + 0x4D1017 + 1), &newWeaponSize, 4U);
	// EDF5.exe+4D1C32
	hookGameBlock((void *)(hmodEXE + 0x4D1C32), (uintptr_t)ASMreadHUiHudWeapon);
	WriteHookToProcess((void *)(hmodEXE + 0x4D1C32 + 12), (void *)&nop6, 6U);
	// EDF5.exe+4D370C
	hookGameBlock((void *)(hmodEXE + 0x4D370C), (uintptr_t)ASMHUiHudWeaponUpdateVehicleText);
	WriteHookToProcess((void*)(hmodEXE + 0x4D370C + 12), (void*)&nop6, 6U);
	HUiHudWeaponUpdateVehicleTextRet = (uintptr_t)(hmodEXE + 0x4D371E);
	// EDF5.exe+4D7110
	hookGameBlock((void*)(hmodEXE + 0x4D7110), (uintptr_t)ASMHUiHudWeaponUpdateAmmoText);
	BYTE _r14_ = 0x4C;
	WriteHookToProcess((void *)(hmodEXE + 0x4D70B5), &_r14_, 1U);
	WriteHookToProcess((void *)(hmodEXE + 0x4D70E2), &_r14_, 1U);

	// old is 0XB20
	int newHPSize = 0xD00;
	static_assert(sizeof(HUiHudPowerGuagePointer) < 0xD00);
	WriteHookToProcessCheckECX((void*)(hmodEXE + 0x4CAAF7 + 1), &newHPSize, 4U);
	// EDF5.exe+4CB4CC
	hookGameBlock((void*)(hmodEXE + 0x4CB4CC), (uintptr_t)ASMreadHUiHudPowerGuage);
	WriteHookToProcess((void*)(hmodEXE + 0x4CB4CC + 12), (void*)&nop6, 6U);
	readHUiHudPowerGuageRet = (uintptr_t)(hmodEXE + 0x4CB4DE);
	// EDF5.exe+4CC8A6
	hookGameBlock((void*)(hmodEXE + 0x4CC8A6), (uintptr_t)ASMupdateHUiHudPowerGuage);
	updateHUiHudPowerGuageRet = (uintptr_t)(hmodEXE + 0x4CC8B2);
}
