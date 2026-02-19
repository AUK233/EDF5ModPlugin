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

#include "HUiHudCommonStruct.h"

PG_HUiHudTextCommonContent __fastcall HUiHudCommon_GetTextContent(PG_HUiHudCommonText pText)
{
	auto ppContent = pText->ppTextContent;
	auto pContent = **ppContent;
	return pContent;
}

size_t __fastcall HUiHudCommon_TextForFormatFloatNumber(const float number, WCHAR* destination, size_t len)
{
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

size_t __fastcall HUiHudCommon_TextForFormatIntNumber(const int number, WCHAR* destination, size_t len)
{
	std::wstring wstr = std::format(L"{0}", number);

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

__m128 __fastcall HUiHudCommon_GetWeaponTextColor(int index)
{
	switch (index) {
	case 2: return { 1.0f, 1.0f, 1.0f, 0.5f };
	case 1: return { 0.0f, 0.0f, 1.0f, 1.0f };
	default: return { 1.0f, 0.0f, 0.0f, 1.0f };
	}
	// end
}
