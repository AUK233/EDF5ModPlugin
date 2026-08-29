#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "commonNOP.h"
#include "2ReadINIConfig.h"

std::wstring ConfigINIPath;
extern "C" {
	int Config_RTRead; // 0 is no, 1 is open

	int Config_HUDEnhance; // 0 is no, 1 is open
	int Config_DisplayDamageType; // 0 is none, 1 is fixed pos, 2 is dynamic pos
	int Config_DisplaySubtitle; // 0 is no, 1 is open
	int Config_PostProcess; // 0 is no, 1 is open
	int Config_DLAA; // 0 is no, 1 is open
	int Config_EnhanceAO; // 0 is no, 1 is open
}

void __fastcall INIConfig_Initialize(LPCWSTR path)
{
	ConfigINIPath = path;

	Config_HUDEnhance = GetPrivateProfileIntW(L"Graphic", L"HUDEnhance", 0, path);
	Config_PostProcess = GetPrivateProfileIntW(L"Graphic", L"PostProcess", 0, path);
	Config_DLAA = GetPrivateProfileIntW(L"Graphic", L"DLAA", 0, path);
	Config_EnhanceAO = GetPrivateProfileIntW(L"Graphic", L"EnhanceAO", 0, path);
	INIConfig_ReadIngameConfigurable();
}

void __fastcall INIConfig_ReadIngameConfigurable()
{
	Config_RTRead = GetPrivateProfileIntW(L"ModOption", L"RTRead", 0, ConfigINIPath.c_str());
	Config_DisplayDamageType = GetPrivateProfileIntW(L"Graphic", L"DisplayDamage", 0, ConfigINIPath.c_str());
	Config_DisplaySubtitle = GetPrivateProfileIntW(L"Graphic", L"DisplaySubtitle", 0, ConfigINIPath.c_str());
}
