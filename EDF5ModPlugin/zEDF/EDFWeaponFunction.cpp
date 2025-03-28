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

#include "CommonCustomStructure.hpp"
#include "utiliy.h"
#include "commonNOP.h"
#include "EDFWeaponFunction.h"

#include "Weapon/XGS_Weapon_LaserMarkerCallFire.h"
#include "Weapon/XGS_Weapon_RadioContact.h"

extern "C" {
	extern PCustomMissionData pMissionCustomData;
}

void module_SetFunction_EDFWeapon(PBYTE hmodEXE)
{
	xgs_Weapon_LaserMarkerCallFire_Init(hmodEXE);
	xgs_Weapon_RadioContact_Init(hmodEXE);
}

int __fastcall module_WeaponFunction_SetDelayInt32(int frame)
{
	return frame * pMissionCustomData->AirRaider_DelayX;
}
