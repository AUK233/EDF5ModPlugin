#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "Base/g_gameFunc.h"
#include "1DigitProcessor.h"

//#define DEBUGMODE

extern DigitRenderer::PDynamicDigitProcessor g_DigitProcessor;

namespace DigitRenderer {
PG_SoldierBase pLocalCurrentPlayers[2];
int bIsSplitScreen;

static const int i_BaseTimeFactor = 180;
static const float f_DamageChargeTime = 0.5;
static const float f_DamageDisplayTime = 3.0;
static const float f_DamageFadeTime = 0.5;
static const float f_DamageHitDisplayTime = 0.5;
static const float f_DamageHitFadeTime = 0.2;

PG_SoldierBase* __fastcall GetLocalCurrentPlayersPointer() {
	return pLocalCurrentPlayers;
}

int __fastcall GetIsSplitScreen() {
	return bIsSplitScreen;
}

void DigitProcessor_ProcessData() {
	if (!pLocalCurrentPlayers[0]) return;

	g_DigitProcessor->ProcessData(0);
	g_DigitProcessor->ProcessData_DamageInHitMode();

	if (bIsSplitScreen) {
		if (!pLocalCurrentPlayers[1]) return;
		g_DigitProcessor->ProcessData(1);
	}
	// end
}

void DigitProcessor_ClearData(UINT32 index) {
	g_DigitProcessor->ClearData(index);
}

void DynamicDigitProcessor_t::Initialize() {
#ifndef DEBUGMODE
	// set constant buffer
	DigitConstantData.ScreenSize[0] = 1920.0f;
	DigitConstantData.ScreenSize[1] = 1080.0f;
	DigitConstantData.ScreenScale[0] = 1;
	DigitConstantData.ScreenScale[1] = 1;
	DigitConstantData.ScaleSpeed = 0.02;
	DigitConstantData.FadeSpeed = -0.01;
	DigitConstantData.pad18[0] = 0;
	DigitConstantData.pad18[1] = 0;
	DigitConstantData.BorderColor = { 0,0,0,1 };

	// reserve vector space to avoid frequent reallocation.
	v_playerDamage[0].reserve(0x10);
	v_playerDamage[1].reserve(0x10);
	v_p1DamageHitInThisFrame.reserve(MAX_DamageHitPerFrame);
	v_p1DamageHitText.reserve(MAX_DamageHitBufferSize);

	ClearData(0);
	ClearData(1);

	InitializeCriticalSectionAndSpinCount(&csDamageHit, 4000);

	// initialize damage display (main)
	ZeroMemory(&DamageDisplayFont_Human, sizeof(DigitFontControl_t));
	DamageDisplayFont_Human.charAlignFactor = 0.5;
	DamageDisplayFont_Human.i_fontSize = 28;
	DamageDisplayFont_Human.f_fontSize = 28;
	// 1370 - 14, 850 + 14
	DamageDisplayPos_Human[0] = { 1356, 864, 1356 + 28, 864 + 28};
	DamageDisplayPos_HumanFactor = { 0, -32, 0, -32 };
	// Vehicle!
	ZeroMemory(&DamageDisplayFont_Vehicle, sizeof(DigitFontControl_t));
	DamageDisplayFont_Vehicle.charAlignFactor = 0;
	DamageDisplayFont_Vehicle.i_fontSize = 40;
	DamageDisplayFont_Vehicle.f_fontSize = 40;
	DamageDisplayPos_Vehicle[0] = { 480, 920, 480 + 40, 920 + 40 };
	DamageDisplayPos_VehicleFactor = { 0, -42, 0, -42 };

	// initialize damage display (split screen)
	DamageDisplayPos_Human[1] = { 580, 906, 580 + 28, 906 + 28 };
	DamageDisplayPos_Human[2] = { 1490, 906, 1490 + 28, 906 + 28 };
	DamageDisplayPos_Vehicle[1] = { 370, 960, 370 + 40, 960 + 40 };
	DamageDisplayPos_Vehicle[2] = { 1270, 960, 1270 + 40, 960 + 40 };

	// initialize weapon info position
	WeaponInfoPos[0][0] = { 960 + 40, 540 + 25, 0, 0 };
	WeaponInfoPos[0][1] = { 960 + 40, 540 - 45, 0, 0 };
	WeaponInfoPos[0][2] = { 960 - 40, 540 + 25, 0, 1 };
	WeaponInfoPos[1][0] = { 960 - 480 + 30, 540 + 25, 0, 0 };
	WeaponInfoPos[1][1] = { 960 - 480 + 30, 540 - 45, 0, 0 };
	WeaponInfoPos[1][2] = { 960 - 480 - 30, 540 + 25, 0, 1 };
	WeaponInfoPos[2][0] = { 960 + 480 + 30, 540 + 25, 0, 0 };
	WeaponInfoPos[2][1] = { 960 + 480 + 30, 540 - 45, 0, 0 };
	WeaponInfoPos[2][2] = { 960 + 480 - 30, 540 + 25, 0, 1 };

	// initialize weapon info color
	WeaponInfoColor[0] = DigitRendererColor_Cyan;
	WeaponInfoColor[1] = DigitRendererColor_Yellow;
	WeaponInfoColor[2] = DigitRendererColor_Cyan;
	WeaponStatusColor[0] = DigitRendererColor_Red;
	WeaponStatusColor[1] = DigitRendererColor_Blue;
	WeaponStatusColor[2] = DigitRendererColor_Green;

#endif // !DEBUGMODE
}

void DynamicDigitProcessor_t::ClearData(UINT32 index) {
	if (index > 1) return;

	bIsSplitScreen = 0;

	pLocalCurrentPlayers[index] = 0;
	v_playerDamage[index].clear();

	ZeroMemory(&playerDamage[index], sizeof(DigitData_Damage_t));
	ZeroMemory(&playerWeaponInfo[index][0], sizeof(DigitData_Weapon_t) * 2);
	ZeroMemory(&playerWeaponRenderInfo[index][0], sizeof(DigitData_Weapon_Render_t) * 2);

	PlayerInVehicle[index] = 0;
	PlayerSoldierType[index] = 0;

	if (index) return;
	v_p1DamageHitInThisFrame.clear();
	v_p1DamageHitText.clear();
}

void DynamicDigitProcessor_t::ProcessData(UINT32 index) {
	auto pPlayer = ProcessData_Damage(index);
	if (!pPlayer) return;

	ProcessData_Weapon(index);
	// end
}

PG_SoldierBase DynamicDigitProcessor_t::ProcessData_Damage(UINT32 index) {
	if (index > 1) return 0;
	auto pPlayer = pLocalCurrentPlayers[index];
	// now, we get it from the game's global.
	// A check has been fixed in module_HUiHudPowerGuageFuncP10, so it is temporarily not in use.
	/*auto pSys = XGS_GetXGSSystemPointer();
	auto pCamera = pSys->player[index].pCamera;
	if (!pCamera) {
		ClearData(index);
		return 0;
	}
	auto pPlayer = (PG_SoldierBase)pCamera->pGameObject;
	if (!pPlayer) {
		ClearData(index);
		return 0;
	}*/

#ifndef DEBUGMODE
	auto currentTime = pPlayer->CurrentTime;
	// 1. get inflict damage
	BaseDigitData_u tempDmg;
	tempDmg.fp32 = pPlayer->f34InflictDamage;
	pPlayer->f34InflictDamage = 0;

	// check in vehicle
	// sometimes game crashes here when leaving a mission
	auto pVehicle = pPlayer->pVehicle;
	if (pVehicle) {
		PlayerInVehicle[index] = 1;
		auto vehicleDmg = pVehicle->f34InflictDamage;
		pVehicle->f34InflictDamage = 0;
		tempDmg.fp32 += vehicleDmg;
	} else {
		PlayerInVehicle[index] = 0;
	}

	if (tempDmg.s32) {
		// If it changes, set the time.
		playerDamage[index].value.fp32 += tempDmg.fp32;
		playerDamage[index].time = currentTime;
	}
	// check duration
	float aliveTime;
	DigitData_Damage_t pushDamage;
	pushDamage.fadeFactor = -1;
	if (playerDamage[index].value.s32) {
		aliveTime = currentTime - playerDamage[index].time;
		if (aliveTime <= f_DamageChargeTime) {
			//float effectTime = aliveTime * f_BaseTimeFactor;
			float effectTime = std::fmaxf(0.0, std::fminf(aliveTime, f_DamageChargeTime));
			playerDamage[index].effectTime = std::sqrt(effectTime / f_DamageChargeTime) * f_DamageChargeTime;
			playerDamage[index].scaleFactor = i_BaseTimeFactor;
		} else {
			pushDamage.value = playerDamage[index].value;
			pushDamage.time = currentTime;
			pushDamage.effectTime = 0;
			pushDamage.scaleFactor = 0;
			pushDamage.fadeFactor = 0;
			// reset to 0
			playerDamage[index].value.s32 = 0;
		}
		// end
	}

	// 2. update damage history
	// check duration
	if (v_playerDamage[index].size()) {
		auto last = v_playerDamage[index].back();
		aliveTime = currentTime - last.time;
		if (aliveTime > f_DamageDisplayTime + f_DamageFadeTime) {
			v_playerDamage[index].pop_back();
		}
		// end
	}
	// update data
	for (auto& dmg : v_playerDamage[index]) {
		aliveTime = currentTime - dmg.time;

		// check need fade
		if (aliveTime > f_DamageDisplayTime) {
			float effectTime = (aliveTime - f_DamageDisplayTime);
			dmg.effectTime = effectTime;
			dmg.fadeFactor = i_BaseTimeFactor;
		}
	}
	// check push
	if (pushDamage.fadeFactor == -1) return pPlayer;

	v_playerDamage[index].insert(v_playerDamage[index].begin(), pushDamage);

	return pPlayer;
#endif
}

void DynamicDigitProcessor_t::ProcessData_DamageInHitMode()
{
	std::vector<DigitData_DamageInHit_t> DamageHit;
	DamageHit.reserve(MAX_DamageHitPerFrame);
	//=================================
	EnterCriticalSection(&csDamageHit);
	std::swap(DamageHit, v_p1DamageHitInThisFrame);
	LeaveCriticalSection(&csDamageHit);
	//=================================

	auto pPlayer = pLocalCurrentPlayers[0];
	auto currentTime = pPlayer->CurrentTime;

	DigitProcessor_DamageInHit_t temp;
	DigitProcessor_DamageInHit_Group_t out;
	out.data.reserve(MAX_DamageHitPerFrame);

	auto bufferSize = DamageHit.size();
	if (bufferSize) {
		out.time = currentTime;
		out.fadeTime = 0;
		for (int i = 0; i < bufferSize; i++) {
			auto& dmg = DamageHit[i];
			temp.pos = dmg.pos;
			temp.text = FormatNumberToDigitRendererChars_Damage(dmg.value.fp32);
			out.data.push_back(temp);
		}
		// end
	}

	// check duration
	auto v_Size = v_p1DamageHitText.size();
	if (bufferSize && v_Size >= MAX_DamageHitBufferSize){
		v_p1DamageHitText.pop_back();
	} else {
		if (v_Size){
			auto& last = v_p1DamageHitText.back();
			auto aliveTime = currentTime - last.time;
			if (aliveTime > f_DamageHitDisplayTime + f_DamageHitFadeTime) {
				v_p1DamageHitText.pop_back();
			}
		}
		// end
	}

	// update data
	for (auto& group : v_p1DamageHitText) {
		auto aliveTime = currentTime - group.time;
		// check need fade
		if (aliveTime > f_DamageHitDisplayTime) {
			group.fadeTime = (aliveTime - f_DamageHitDisplayTime) * 10;
		}
	}

	// check push
	if (bufferSize) {
		v_p1DamageHitText.insert(v_p1DamageHitText.begin(), out);
	}
	// end
}

void DynamicDigitProcessor_t::ProcessData_Weapon(UINT32 index) {
	ProcessData_Weapon_Update(&playerWeaponInfo[index][0], &playerWeaponRenderInfo[index][0], index);
	ProcessData_Weapon_Update(&playerWeaponInfo[index][1], &playerWeaponRenderInfo[index][1], index);
}

void DynamicDigitProcessor_t::ProcessData_Weapon_Update(PDigitData_Weapon pIn, PDigitData_Weapon_Render pOut, UINT32 index) {
	if (!pIn->weaponAlignType) {
		pOut->isEnabled = 0;
		return;
	}

	auto alignType = pIn->weaponAlignType - 1;
	pOut->pos = WeaponInfoPos[index + bIsSplitScreen][alignType];

	auto weaponStatus = pIn->weaponStatus;
	if (weaponStatus == DigitRendererWeaponStatus_Normal) {
		pOut->colorIndex = WeaponInfoColor[alignType];
		pOut->text = FormatNumberToDigitRendererChars_Ammo(pIn->value.s32);
	}
	else {
		UINT32 statusIndex = weaponStatus - 1;
		if (statusIndex > 2) {
			pOut->isEnabled = 0;
			return;
		}

		pOut->colorIndex = WeaponStatusColor[statusIndex];
		if (!statusIndex) {
			pOut->text = FormatNumberToDigitRendererChars_Second(pIn->value.fp32);
		} else {
			pOut->text = FormatNumberToDigitRendererChars_Percentage(pIn->value.fp32);
		}
	}

	pOut->isEnabled = 1;
	pOut->scaleTime = pIn->effectTime;

	pIn->effectTime += pIn->timeIncrement;
	if (pIn->effectTime < 0) {
		pIn->effectTime = 0;
		pIn->timeIncrement = 0;
	}
	// end
}

//end
}

int __fastcall DigitProcessor_SetLocalCurrentPlayer(PXGS_System_Camera pCamera, UINT32 pCount) {
	auto playerID = pCamera->PlayerID;
	if (pCount > 2) return playerID;
	using namespace DigitRenderer;

	if (Game_IsOnlineMode()){
		bIsSplitScreen = 0;
		if(playerID == 0){
			auto pPlayerObject = (PG_SoldierBase)pCamera->pGameObject;
			pLocalCurrentPlayers[0] = pPlayerObject;

			auto pVFT = (CallFunc_SoldierBase_GetSoldierType*)pPlayerObject->vf_table;
			g_DigitProcessor->PlayerSoldierType[0] = pVFT[0x47]();
		}
		// end
	}
	else{
		if (pCount == 1) {
			bIsSplitScreen = 0;
		} else {
			bIsSplitScreen = 1;
		}

		auto pPlayerObject = (PG_SoldierBase)pCamera->pGameObject;
		pLocalCurrentPlayers[playerID] = pPlayerObject;

		auto pVFT = (CallFunc_SoldierBase_GetSoldierType*)pPlayerObject->vf_table;
		g_DigitProcessor->PlayerSoldierType[playerID] = pVFT[0x47]();
	}

	return playerID;
}

void __fastcall DigitProcessor_GetPlayerWeaponStatus(PG_SoldierBase pObject, int WeaponAlignType, PG_WeaponBase pWeapon)
{
	if (!pObject) return;

	using namespace DigitRenderer;
	int playerIndex;
	if (pObject == pLocalCurrentPlayers[0]) playerIndex = 0;
	else if (pObject == pLocalCurrentPlayers[1]) playerIndex = 1;
	else return;

	// =================================================
	int weaponIndex = 0;
	if (WeaponAlignType != 1) weaponIndex = 1;

	DigitData_Weapon_t out;
	out.weaponAlignType = WeaponAlignType;
	out.pWeapon = pWeapon;

	auto curAmmo = pWeapon->CurrentAmmoCount;
	if (curAmmo > 0) {
		out.value.s32 = curAmmo;
		out.weaponStatus = DigitRendererWeaponStatus_Normal;
		goto checkData;
	}

	if (pWeapon->Info.ReloadType == 1) {
		if (pWeapon->Info.ReloadTime == -1 || pWeapon->reloadTimeCount == pWeapon->Info.ReloadTime) {
			ZeroMemory(&g_DigitProcessor->playerWeaponInfo[playerIndex][weaponIndex], sizeof(DigitData_Damage_t));
			return;
		} else {
			goto reloadBlock;
		}
	}

	if (pWeapon->Info.ReloadType == 2) {
		float reloadProgress = pWeapon->Info.ReloadTime - pWeapon->reloadTimeCount;
		reloadProgress /= pWeapon->Info.ReloadTime;
		reloadProgress *= 100.0f;
		out.value.fp32 = reloadProgress;
		out.weaponStatus = DigitRendererWeaponStatus_Credit;
		goto checkData;
	}

	if (pWeapon->Info.EnergyChargeRequire > 0.0f) {
		float chargeProgress = pWeapon->Info.EnergyChargeRequire - pWeapon->EnergyChargeCount;
		chargeProgress /= pWeapon->Info.EnergyChargeRequire;
		chargeProgress *= 100.0f;
		out.value.fp32 = chargeProgress;
		out.weaponStatus = DigitRendererWeaponStatus_Charge;
		goto checkData;
	}

reloadBlock:
	{
		float remainTime = pWeapon->reloadTimeCount / 60.0f;
		out.value.fp32 = remainTime;
		out.weaponStatus = DigitRendererWeaponStatus_Reload;
	}

checkData:
	auto checkValue = g_DigitProcessor->playerWeaponInfo[playerIndex][weaponIndex].value.s32;
	auto checkWeapon = g_DigitProcessor->playerWeaponInfo[playerIndex][weaponIndex].pWeapon;
	if (checkWeapon != pWeapon || checkValue == out.value.s32){
		out.effectTime = g_DigitProcessor->playerWeaponInfo[playerIndex][weaponIndex].effectTime;
		out.timeIncrement = g_DigitProcessor->playerWeaponInfo[playerIndex][weaponIndex].timeIncrement;
	} else {
		out.effectTime = 15;
		out.timeIncrement = -1;
	}
	memcpy(&g_DigitProcessor->playerWeaponInfo[playerIndex][weaponIndex], &out, sizeof(DigitData_Weapon_t));
}

float __fastcall DigitProcessor_GetPlayerHitDamage(float damage, Pxgs_DamageData pIn, PG_SoldierBase pObject)
{
	using namespace DigitRenderer;
	auto pPlayer = pLocalCurrentPlayers[0];
	if (!pPlayer) return damage;

	if (pObject != pPlayer) {
		if (pObject != pPlayer->pVehicle) return damage;
	}

	// ==================================================
	EnterCriticalSection(&g_DigitProcessor->csDamageHit);

	auto bufferSize = g_DigitProcessor->v_p1DamageHitInThisFrame.size();
	if (bufferSize < DynamicDigitProcessor_t::MAX_DamageHitPerFrame) {
		auto pPos = pIn->pHitPos;
		DigitData_DamageInHit_t newData;

		if (pPos) newData.pos = pPos[0];
		else newData.pos = pIn->pos;

		newData.value.fp32 = damage;
		g_DigitProcessor->v_p1DamageHitInThisFrame.push_back(newData);
	}

	LeaveCriticalSection(&g_DigitProcessor->csDamageHit);
	// ==================================================

	return damage;
}
