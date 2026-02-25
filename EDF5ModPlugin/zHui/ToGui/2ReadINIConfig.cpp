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
}

void __fastcall INIConfig_Initialize(LPCWSTR path)
{
	ConfigINIPath = path;

	Config_RTRead = GetPrivateProfileIntW(L"ModOption", L"RTRead", 0, path);
	Config_HUDEnhance = GetPrivateProfileIntW(L"ModOption", L"HUDEnhance", 0, path);
	INIConfig_ReadIngameConfigurable();
}

void __fastcall INIConfig_ReadIngameConfigurable()
{
	Config_DisplayDamageType = GetPrivateProfileIntW(L"ModOption", L"DisplayDamage", 0, ConfigINIPath.c_str());
	Config_DisplaySubtitle = GetPrivateProfileIntW(L"ModOption", L"DisplaySubtitle", 0, ConfigINIPath.c_str());
}
