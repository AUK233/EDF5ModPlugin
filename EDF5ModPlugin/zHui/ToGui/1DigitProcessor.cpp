#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "Base/g_gameFunc.h"
#include "1DigitProcessor.h"

extern DigitRenderer::PDynamicDigitProcessor g_DigitProcessor;

namespace DigitRenderer {
	PG_SoldierBase pLocalCurrentPlayers[2];
	int bIsSplitScreen;

	static const float f_BaseTimeFactor = 180.0;
	static const float f_DamageChargeTime = 0.5;
	static const float f_DamageDisplayTime = 3.0;
	static const float f_DamageFadeTime = 0.5;

	PG_SoldierBase* __fastcall GetLocalCurrentPlayersPointer() {
		return pLocalCurrentPlayers;
	}

	int __fastcall GetIsSplitScreen() {
		return bIsSplitScreen;
	}

	void DigitProcessor_ProcessData() {
		if (!pLocalCurrentPlayers[0]) return;

		g_DigitProcessor->ProcessData(0);

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

		v_playerDamage[0].reserve(0x10);
		v_playerDamage[1].reserve(0x10);
		ClearData(0);
		ClearData(1);

		// initialize damage display (main)
		ZeroMemory(&DamageDisplayFont_Human, sizeof(DigitFontControl_t));
		DamageDisplayFont_Human.charAlignType = DigitRendererAlign_Center;
		DamageDisplayFont_Human.i_fontSize = 28;
		DamageDisplayFont_Human.f_fontSize = 28;
		// 1370 - 14, 850 + 14
		DamageDisplayPos_Human[0] = { 1356, 864, 1356 + 28, 864 + 28};
		DamageDisplayPos_HumanFactor = { 0, -32, 0, -32 };
		// Vehicle!
		ZeroMemory(&DamageDisplayFont_Vehicle, sizeof(DigitFontControl_t));
		DamageDisplayFont_Vehicle.charAlignType = DigitRendererAlign_Left;
		DamageDisplayFont_Vehicle.i_fontSize = 40;
		DamageDisplayFont_Vehicle.f_fontSize = 40;
		DamageDisplayPos_Vehicle[0] = { 480, 920, 480 + 40, 920 + 40 };
		DamageDisplayPos_VehicleFactor = { 0, -42, 0, -42 };

		// initialize damage display (split screen)
		DamageDisplayPos_Human[1] = { 580, 906, 580 + 28, 906 + 28 };
		DamageDisplayPos_Human[2] = { 1490, 906, 1490 + 28, 906 + 28 };
		DamageDisplayPos_Vehicle[1] = { 370, 960, 370 + 40, 960 + 40 };
		DamageDisplayPos_Vehicle[2] = { 1270, 960, 1270 + 40, 960 + 40 };
	}

	void DynamicDigitProcessor_t::ClearData(UINT32 index) {
		if (index > 1) return;

		bIsSplitScreen = 0;

		pLocalCurrentPlayers[index] = 0;
		v_playerDamage[index].clear();

		ZeroMemory(&playerDamage[index], sizeof(DigitData_Damage_t));
		ZeroMemory(&playerWeaponInfo[index][0], sizeof(DigitData_Weapon_t) * 2);

		PlayerInVehicle[index] = 0;
	}

	void DynamicDigitProcessor_t::ProcessData(UINT32 index) {
		auto pPlayer = ProcessData_Damage(index);
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
		pushDamage.fadeEnable = -1;
		if (playerDamage[index].value.s32) {
			aliveTime = currentTime - playerDamage[index].time;
			if (aliveTime <= f_DamageChargeTime) {
				float effectTime = aliveTime * f_BaseTimeFactor;
				playerDamage[index].effectTime = effectTime;
			} else {
				pushDamage.value = playerDamage[index].value;
				pushDamage.time = currentTime;
				pushDamage.effectTime = 0;
				pushDamage.fadeEnable = 0;
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
				float effectTime = (aliveTime - f_DamageDisplayTime) * f_BaseTimeFactor;
				dmg.effectTime = effectTime;
				dmg.fadeEnable = 1;
			}
		}
		// check push
		if (pushDamage.fadeEnable == -1) return pPlayer;

		v_playerDamage[index].insert(v_playerDamage[index].begin(), pushDamage);

		return pPlayer;
	}

	// end
}

int __fastcall DigitProcessor_SetLocalCurrentPlayer(PXGS_System_Player pSysPlayer, UINT32 pCount) {
	auto playerID = pSysPlayer->PlayerID;
	if (pCount > 2) return playerID;

	if (Game_IsOnlineMode()){
		DigitRenderer::bIsSplitScreen = 0;
		if(playerID == 0){
			auto pPlayerObject = (PG_SoldierBase)pSysPlayer->pCamera->pGameObject;
			DigitRenderer::pLocalCurrentPlayers[0] = pPlayerObject;
		}
		// end
	}
	else{
		if (pCount == 1) {
			DigitRenderer::bIsSplitScreen = 0;
		} else {
			DigitRenderer::bIsSplitScreen = 1;
		}

		auto pPlayerObject = (PG_SoldierBase)pSysPlayer->pCamera->pGameObject;
		DigitRenderer::pLocalCurrentPlayers[playerID] = pPlayerObject;
	}

	return playerID;
}
