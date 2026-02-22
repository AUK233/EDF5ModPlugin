#pragma once
#include "0DigitCommon.h"
#include "Base/g_system.h"
#include "zEDF/EDFSoldierClass.hpp"

namespace DigitRenderer {
	PG_SoldierBase* __fastcall GetLocalCurrentPlayersPointer();
	int __fastcall GetIsSplitScreen();

	void DigitProcessor_ProcessData();
	void DigitProcessor_ClearData(UINT32 index);

	class DynamicDigitProcessor_t {
	public:
		DigitConstants_t DigitConstantData;

		// 0 is no split screen.
		// 1 is p1, 2 is p2 in split screen.
		__m128 DamageDisplayPos_Human[3];
		__m128 DamageDisplayPos_Vehicle[3];
		// push text's offset
		__m128 DamageDisplayPos_HumanFactor;
		__m128 DamageDisplayPos_VehicleFactor;

		// count 2 players
		DigitData_Damage_t playerDamage[2];
		// 2 weapons for 2 players
		DigitData_Weapon_t playerWeaponInfo[2][2];
		// count 2 players
		std::vector<DigitData_Damage_t> v_playerDamage[2];

		DigitFontControl_t DamageDisplayFont_Human;
		DigitFontControl_t DamageDisplayFont_Vehicle;

		int PlayerInVehicle[2];
	public:
		void Initialize();
		void ClearData(UINT32 index);
		void ProcessData(UINT32 index);
		PG_SoldierBase ProcessData_Damage(UINT32 index);
	};
	typedef DynamicDigitProcessor_t* PDynamicDigitProcessor;
}

extern "C" {
	int __fastcall DigitProcessor_SetLocalCurrentPlayer(PXGS_System_Player pSysPlayer, UINT32 pCount);
}

