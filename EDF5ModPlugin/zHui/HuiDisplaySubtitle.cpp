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
#include <filesystem>
#include "shlwapi.h"

#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include "utiliy.h"

#include "commonNOP.h"
#include "GameFunctionInASM.h"

#include "HuiDisplaySubtitle.h"

extern "C" {
	extern int displaySubtitleOn;
	uint32_t pSubtitleIndex;
	PCustomSubtitle pSubtitleFile;
	int* EDFCurrentTextLanguage;
}

PCustomSubtitle pSubtitle[4];
std::wstring wstr_SubtitleText;
int SubtitleTextMinSize = 0;
int SubtitleTextSpace = 1;

int __fastcall GetSubtitleTextAddress(SubtitleTextStruct* startAddr, WCHAR* pAudioName, int nameSize, int noPos)
{
	PCustomSubtitle pSub = pSubtitleFile;
	int textIndex = -1;
	int subSize = pSub->v_Name.size();
	for (int i = 0; i < subSize; i++) {
		if (pSub->v_Name[i].size == nameSize) {
			if (!StrCmpW(pSub->v_Name[i].text, pAudioName)) {
				textIndex = pSub->v_Name[i].v.id;
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
			int curSize = pSub->v_Text[textIndex].size / 2;
			int minSize = SubtitleTextMinSize;
			if (curSize < minSize) {
				float fSize = minSize - curSize;
				float addPos = (fSize / minSize) * 640.0f;
				curPos += addPos;
			}
		}

		startAddr->v.posX = curPos;
		startAddr->text = pSub->v_Text[textIndex].text;
		startAddr->size = pSub->v_Text[textIndex].size;
		return pSub->v_Text[textIndex].size;
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

		int curSize = size / 2;
		int minSize = SubtitleTextMinSize;
		if (curSize < minSize) {
			minSize -= curSize;
			minSize /= SubtitleTextSpace;
			for (int i = 0; i < minSize; i++) {
				wstr_SubtitleText += L"¡¡";
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

	if (!displaySubtitleOn||!pSubtitleFile) {
		InitializeTalkSubtitleString();
		return;
	}

	SubtitleTextStruct text;
	text.v.posX = 320.0f;
	text.size = wstr_SubtitleText.size();
	text.text = (WCHAR*)wstr_SubtitleText.c_str();

	ASMshowSubtitleByPlayingSoundOn(pScript, &text);
}

void __fastcall LoadSelectedSubtitleFile()
{
	int language = *EDFCurrentTextLanguage;
	if (language > 3) {
		pSubtitleFile = 0;
		return;
	}

	std::wstring filePath = L"./subtitle/";
	switch (language)
	{
	case 0:
		filePath += L"voice_subtitle_JA.sgo";
		// 37 / 2
		SubtitleTextMinSize = 18;
		SubtitleTextSpace = 1;
		break;
	case 1:
		// 86 / 2
		filePath += L"voice_subtitle_EN.sgo";
		SubtitleTextMinSize = 43;
		SubtitleTextSpace = 2;
		break;
	case 2:
		filePath += L"voice_subtitle_CN.sgo";
		// 44 / 2
		SubtitleTextMinSize = 22;
		SubtitleTextSpace = 1;
		break;
	case 3:
		filePath += L"voice_subtitle_KR.sgo";
		SubtitleTextMinSize = 22;
		SubtitleTextSpace = 1;
		break;
	default:
		filePath += L"test.sgo";
		SubtitleTextMinSize = 22;
		SubtitleTextSpace = 1;
		break;
	}

	if (!std::filesystem::exists(filePath)) {
		pSubtitleFile = 0;
		return;
	}

	PCustomSubtitle pSub = pSubtitle[language];
	pSubtitleFile = pSub;
	// do not load file repeatedly
	if (pSub->pFile) {
		return;
	}

	std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate | std::ios::in);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	char* p = (char*)_aligned_malloc(size, 0x10);
	if (p) {
		file.read(p, size);
		pSub->pFile = p;

		// read sgo
		int DataNodeCount = *(int*)(p + 8);
		int DataNodeOffset = *(int*)(p + 0xC);
		int DataNameCount = *(int*)(p + 0x10);
		int DataNameOffset = *(int*)(p + 0x14);
		int nodepos, nameoffset;
		// read node name
		pSub->v_Name.resize(DataNameCount);
		for (int i = 0; i < DataNameCount; i++)
		{
			nodepos = DataNameOffset + (i * 0x8);
			nameoffset = *(int*)(p + nodepos);

			pSub->v_Name[i].text = (WCHAR*)(p + nodepos + nameoffset);
			pSub->v_Name[i].v.id = *(int*)(p + nodepos + 4);
			pSub->v_Name[i].size = wcslen(pSub->v_Name[i].text);
		}
		// read node wstring
		int strsize, stroffset;
		pSub->v_Text.resize(DataNodeCount);
		for (int i = 0; i < DataNodeCount; i++) {
			nodepos = DataNodeOffset + (i * 0xC);
			strsize = *(int*)(p + nodepos + 4);
			pSub->v_Text[i].size = strsize;
			if (strsize > 0) {
				stroffset = *(int*)(p + nodepos + 8);
				pSub->v_Text[i].text = (WCHAR*)(p + nodepos + stroffset);
			}
			else {
				pSub->v_Text[i].text = 0;
			}
		}
	}
	file.close();
}

void InitializeSubtitlePointer()
{
	pSubtitleIndex = 0;
	pSubtitleFile = 0;
	PCustomSubtitle pSub;
	std::vector<SubtitleTextStruct> v_init;

	pSub = (PCustomSubtitle)_aligned_malloc(sizeof(CustomSubtitle_t), 16U);
	pSubtitle[0] = pSub;
	if (pSub) {
		ZeroMemory(pSub, sizeof(CustomSubtitle_t));
	}

	pSub = (PCustomSubtitle)_aligned_malloc(sizeof(CustomSubtitle_t), 16U);
	pSubtitle[1] = pSub;
	if (pSub) {
		ZeroMemory(pSub, sizeof(CustomSubtitle_t));
	}

	pSub = (PCustomSubtitle)_aligned_malloc(sizeof(CustomSubtitle_t), 16U);
	pSubtitle[2] = pSub;
	if (pSub) {
		ZeroMemory(pSub, sizeof(CustomSubtitle_t));
	}

	pSub = (PCustomSubtitle)_aligned_malloc(sizeof(CustomSubtitle_t), 16U);
	pSubtitle[3] = pSub;
	if (pSub) {
		ZeroMemory(pSub, sizeof(CustomSubtitle_t));
	}
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

void hookDisplaySubtitle(PBYTE hmodEXE)
{
	EDFCurrentTextLanguage = (int*)(hmodEXE + 0x1137110);
	InitializeSubtitlePointer();

	// Load subtitle file at the same time when select text language.
	// EDF5.exe+3D7D22
	hookGameBlock((void*)(hmodEXE + 0x3D7D22), (uintptr_t)LoadSelectedSubtitleFile);
	// EDF5.exe+524573
	hookGameBlock((void*)(hmodEXE + 0x524573), (uintptr_t)LoadSelectedSubtitleFile);

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

