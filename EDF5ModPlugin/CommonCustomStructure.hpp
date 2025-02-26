#pragma once
#include <vector>

typedef struct SubtitleTextStruct {
	WCHAR* text;
	union
	{
		int id;
		float posX;
	} v;
	int size;
} *PSubtitleText;

__declspec(align(16)) typedef struct CustomSubtitle_t {
	char* pFile;
	int pad[2];
	// vector must be set to 0!!!
	std::vector<SubtitleTextStruct> v_Name;
	int pad1[2];
	std::vector<SubtitleTextStruct> v_Text;
	int pad2[2];
} *PCustomSubtitle;

__declspec(align(16)) typedef struct CustomMissionData_t {
	int armorBox;
	int weaponBox;
	int pad[2];
	// They are multiplier factors
	float Ranger_SpeedX;
	float PaleWing_ChargeX;
	float AirRaider_CreditX;
	float Fencer_ChargeX;
	//
	float Class_SpeedX;
	float Ranger_DashSpeedX;
	float PaleWing_WeaponChargeX;
	float PaleWing_BoostSpeedX;
	// They are support weapons for delay and damage
	float AirRaider_DelayX;
	float AirRaider_DamageX;
	//
	float Fencer_DashX;
	float Fencer_BoostX;
} *PCustomMissionData;
static_assert(offsetof(CustomMissionData_t, Ranger_SpeedX) == 0x10);
static_assert(offsetof(CustomMissionData_t, Fencer_ChargeX) == 0x1C);
