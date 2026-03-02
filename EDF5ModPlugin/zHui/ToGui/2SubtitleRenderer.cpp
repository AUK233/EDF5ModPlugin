#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "commonNOP.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "0GetDXGI.h"
#include "0DigitCommon.h"
#include "1DigitProcessor.h"

#include "2SubtitleRenderer.h"

extern ImFont* MyDefaultFont;

extern "C"{
	extern int* g_CurrentTextLanguage;

	void __fastcall ASMscript2C_FA0();
	uintptr_t script2C_FA0ret;
	void __fastcall ASMscript2C_FA2();

	void __fastcall ASMaddTalkToList();
	uintptr_t addTalkToList_ret;

	void __fastcall ASMsetPlayTalk();
	uintptr_t setPlayTalk_ret;
}

extern DigitRenderer::PSubtitleRenderer g_SubtitleRenderer;

static const float f_FontSize = 32.0;

namespace DigitRenderer {

	void SubtitleRenderer_Initialization(PBYTE hmodEXE) {
		// Load subtitle file at the same time when select text language.
		// Not using it now
		// EDF5.exe+3D7D22
		//hookGameBlock((void*)(hmodEXE + 0x3D7D22), (uintptr_t)LoadSelectedSubtitleFile);
		// EDF5.exe+524573
		//hookGameBlock((void*)(hmodEXE + 0x524573), (uintptr_t)LoadSelectedSubtitleFile);

		// EDF5.exe+127CC0, play voice
		hookGameBlockWithInt3((void*)(hmodEXE + 0x127CC0), (uintptr_t)ASMscript2C_FA0);
		WriteHookToProcess((void*)(hmodEXE + 0x127CC0 + 15), (void*)&nop2, 2U);
		script2C_FA0ret = (uintptr_t)(hmodEXE + 0x127CEF);

		// EDF5.exe+116708, wait voice
		hookGameBlockWithInt3((void*)(hmodEXE + 0x116708), (uintptr_t)ASMscript2C_FA2);
		WriteHookToProcess((void*)(hmodEXE + 0x116708 + 15), (void*)&nop1, 1U);

		// EDF5.exe+3278F3, initialize talk information
		BYTE TalkInit[] = {
			0x89, 0x73, 0x6C,
			0x90,
			0x48, 0x8B, 0xC3
		};
		WriteHookToProcess((void*)(hmodEXE + 0x3278F3), TalkInit, 7U);


		// EDF5.exe+32B2E1, copy talk information
		BYTE CopyTalkInit[]= {
			0x8B, 0x47, 0x6C,
			0x89, 0x43, 0x6C,
			0x90
		};
		WriteHookToProcess((void*)(hmodEXE + 0x32B2E1), CopyTalkInit, 7U);

		// EDF5.exe+326D20, add talk to list
		hookGameBlockWithInt3((void*)(hmodEXE + 0x326D20), (uintptr_t)ASMaddTalkToList);
		WriteHookToProcess((void*)(hmodEXE + 0x326D20 + 15), (void*)&nop1, 1U);
		addTalkToList_ret = (uintptr_t)(hmodEXE + 0x326D30);

		// EDF5.exe+327A2E, set play talk
		hookGameBlockRAXWithInt3((void*)(hmodEXE + 0x327A2E), (uintptr_t)ASMsetPlayTalk);
		setPlayTalk_ret = (uintptr_t)(hmodEXE + 0x327A3B);

		// initialize subtitles data
		HANDLE tempHND = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SubtitleRenderer_InitSubtitle, NULL, NULL, NULL);
		if (tempHND) {
			CloseHandle(tempHND);
		}
	}

	void WINAPI SubtitleRenderer_InitSubtitle() {
		auto p = (PSubtitleRenderer)_aligned_malloc(sizeof(SubtitleRenderer_t), 0x10);
		if (!p) return;

		p = new(p) SubtitleRenderer_t();

		InitializeCriticalSectionAndSpinCount(&p->csSubtitle, 4000);
		p->CurrentSubtitle.text_size = 0;

		// load subtitle
		static LPCWSTR filePath[] = {
			L"./subtitle/voice_subtitle_JA.sgo",
			L"./subtitle/voice_subtitle_EN.sgo",
			L"./subtitle/voice_subtitle_CN.sgo",
			L"./subtitle/voice_subtitle_KR.sgo",
			L"./subtitle/test.sgo"
		};
		//
		for (int i = 0; i < 4; i++) {
			std::ifstream file(filePath[i], std::ios::binary | std::ios::ate | std::ios::in);
			std::streamsize size = file.tellg();
			file.seekg(0, std::ios::beg);
			char* pData = (char*)_aligned_malloc(size, 0x10);
			if (pData) {
				file.read(pData, size);
				// read sgo
				int DataNodeCount = *(int*)(pData + 8);
				int DataNodeOffset = *(int*)(pData + 0xC);
				int DataNameCount = *(int*)(pData + 0x10);
				int DataNameOffset = *(int*)(pData + 0x14);
				//
				int nodepos, strSize, dataOffset;
				// read node name
				WCHAR* pWstr;
				p->SubtitleMap[i].rehash(DataNameCount);
				//p->SubtitleName[i].reserve(DataNameCount);
				//p->SubtitleName_index[i].reserve(DataNameCount);
				for (int j = 0; j < DataNameCount; j++) {
					nodepos = DataNameOffset + (j * 8);
					dataOffset = *(int*)(pData + nodepos);
					pWstr = (WCHAR*)(pData + nodepos + dataOffset);
					auto textID = *(int*)(pData + nodepos + 4);
					p->SubtitleMap[i][std::wstring(pWstr)] = textID;
					//p->SubtitleName[i].push_back(pWstr);
					//p->SubtitleName_index[i].push_back(textID);
				}
				// read node wstring
				p->SubtitleList[i].resize(DataNodeCount);
				for (int j = 0; j < DataNodeCount; j++){
					nodepos = DataNodeOffset + (j * 0xC);
					strSize = *(int*)(pData + nodepos + 4);
					p->SubtitleList[i][j].text_size = strSize;
					if (strSize > 0) {
						dataOffset = *(int*)(pData + nodepos + 8);
						SubtitleRenderer_CheckSubtitle((LPCWSTR)(pData + nodepos + dataOffset), strSize, p->SubtitleList[i][j], i);
					}
				}
				_aligned_free(pData);
			}

			file.close();
		}

		//
		g_SubtitleRenderer = p;
	}

	void __fastcall SubtitleRenderer_CheckSubtitle(LPCWSTR pWstr, int textSize, SubtitleTextStruct_t& data, int language) {

		if (textSize <= 20) {
			data.text = UnicodeToUTF8(pWstr, textSize + 1);
			data.current_line = std::count(pWstr, pWstr + textSize, L'\n');
			return;
		}

		std::wstring newText;
		int newline_count = 0;

		int eachline_count = 20;
		int estimated_newlines = textSize / 20;
		if (language == 1) {
			eachline_count = 40;
			estimated_newlines /= 2;
		}

		newText.reserve(textSize + estimated_newlines);

		int word_count = 0;
		for (int i = 0; i < textSize; i++) {
			newText += pWstr[i];
			word_count++;

			if (pWstr[i] == L'\n') {
				newline_count++;
				word_count = 0;
			} else {
				if (word_count == eachline_count) {
					newText += L'\n';
					newline_count++;
					word_count = 0;
				}
			}
			//end
		}
		newText += L'\0';
		data.text = UnicodeToUTF8(newText.c_str(), newText.size());
		data.current_line = newline_count;
	}

	void SubtitleRenderer_t::GetCurrentSubtitle(LPCWSTR pAudioName, size_t nameSize){
		auto pPlayer = DigitRenderer::GetLocalCurrentPlayersPointer();

		if (!pPlayer[0] || !nameSize) {
			EnterCriticalSection(&csSubtitle);
			CurrentSubtitle.text_size = 0;
			CurrentSubtitle.active_time = 0;
			LeaveCriticalSection(&csSubtitle);
			return;
		}

		auto curLang = *g_CurrentTextLanguage;
		if ((UINT32)curLang > 3) curLang = 0;

		int index = -1;
		auto& subtitleMap = SubtitleMap[curLang];
		std::wstring inName(pAudioName, nameSize);
		auto it = subtitleMap.find(inName);
		if (it != subtitleMap.end()) index = it->second;

		/*auto& subtitleMap = SubtitleName_index[curLang];
		auto& subtitleName = SubtitleName[curLang];

		int index = -1;
		std::wstring inName(pAudioName, nameSize);
		for (size_t i = 0; i < subtitleName.size(); i++) {
			if (inName == subtitleName[i]) {
				index = subtitleMap[i];
				break;
			}
		}*/

		if (index == -1) {
			EnterCriticalSection(&csSubtitle);
			CurrentSubtitle.text_size = 0;
			CurrentSubtitle.active_time = 0;
			LeaveCriticalSection(&csSubtitle);
			return;
		}

		SubtitleTextShowStruct_t out;
		out.active_time = pPlayer[0]->CurrentTime;

		auto textSize = SubtitleList[curLang][index].text_size;
		out.text = SubtitleList[curLang][index].text;
		out.text_size = textSize;

		auto hasNewlineN = SubtitleList[curLang][index].current_line;
		if (hasNewlineN > 4) {
			out.align_factor = 1;
			out.current_line = hasNewlineN;
			out.bIsScroll = 1;
		} else {
			// check displayed location
			int iSize = textSize;
			float fSize = 1.0f;
			if (curLang == 1) {
				// English has double word length
				iSize /= 2;
				fSize = 0.5f;
			}

			// if only one line, it will be displayed at position.
			if (!hasNewlineN && iSize < 20) {
				fSize *= textSize;
				out.align_factor = fSize / 20.0f;
				out.current_line = 0;
				out.bIsScroll = 0;
			} else {
				out.align_factor = 1;
				out.current_line = hasNewlineN;
				out.bIsScroll = 0;
			}
			// end
		}

		EnterCriticalSection(&csSubtitle);
		std::swap(CurrentSubtitle, out);
		//CurrentSubtitle = out;
		LeaveCriticalSection(&csSubtitle);
	}

	void SubtitleRenderer_t::DisplayCurrentSubtitle(PG_SoldierBase pPlayer) {
		if (!pPlayer) {
			CurrentSubtitle.text_size = 0;
			CurrentSubtitle.active_time = 0;
			return;
		}

		SubtitleTextShowStruct_t in;
		// ========================================
		EnterCriticalSection(&csSubtitle);
		auto tempSize = CurrentSubtitle.text_size;
		if (tempSize > 0) {
			in = CurrentSubtitle;
		} else {
			CurrentSubtitle.active_time = 0;
		}
		LeaveCriticalSection(&csSubtitle);
		// ========================================

		if (tempSize <= 0) return;

		auto active_time = pPlayer->CurrentTime - in.active_time;
		if (active_time < -1.0f || active_time > 120.0f) {
			CurrentSubtitle.text_size = 0;
			CurrentSubtitle.active_time = 0;
			return;
		}


		// base
		static const __m128 v_baseResolution = { 1920, 1080, 1, 1 };
		__m128 baseOffset_border = { -10, -10, 10, 10 };
		__m128 baseOffset_main = { -321, -16, 321, 16 }; // 32x20+2/2
		__m128 basePos = { 960, 770, 960, 770 };
		// end

		// calculate scale factor
		auto pRes = (__m128i*)DXGI_GetRealTimeResolution();
		auto v_resolution = _mm_cvtepi32_ps(_mm_loadu_si128(pRes));

		auto v_scale = _mm_div_ps(v_resolution, v_baseResolution);
		v_scale = _mm_shuffle_ps(v_scale, v_scale, MY_SHUFFLE(0, 1, 0, 1));

		// set position
		baseOffset_border = _mm_mul_ps(baseOffset_border, v_scale);
		baseOffset_main = _mm_mul_ps(baseOffset_main, v_scale);
		basePos = _mm_mul_ps(basePos, v_scale);

		float fontSize = f_FontSize * v_scale.m128_f32[0];
		float rounding = 10.0f * v_scale.m128_f32[1];

		// ===============
		ImVec2 rectPos[2];
		ImVec2 clipPos[2];
		ImVec2 textPos;

		if (in.current_line == 0) {
			__m128 v_align_factor;
			v_align_factor.m128_f32[1] = 1.0f;
			v_align_factor.m128_f32[0] = in.align_factor;
			v_align_factor = _mm_shuffle_ps(v_align_factor, v_align_factor, MY_SHUFFLE(0, 1, 0, 1));
			baseOffset_main = _mm_mul_ps(baseOffset_main, v_align_factor);
		}

		__m128 displayPos = _mm_add_ps(basePos, baseOffset_main);
		textPos.x = displayPos.m128_f32[0];
		textPos.y = displayPos.m128_f32[1];
		_mm_storeu_ps(&clipPos[0].x, displayPos);

		__m128 v_RectPos = _mm_add_ps(displayPos, baseOffset_border);
		_mm_storeu_ps(&rectPos[0].x, v_RectPos);

		auto current_line = in.current_line;
		if (current_line > 4) current_line = 4;

		float rectHeight = fontSize * current_line;
		rectPos[1].y += rectHeight;
		clipPos[1].y += rectHeight;

		// ============================================
		// display
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		draw_list->AddRectFilled(rectPos[0], rectPos[1], 0x80000000, rounding, ImDrawFlags_RoundCornersAll);

		draw_list->PushClipRect(clipPos[0], clipPos[1]);

		if (in.bIsScroll && active_time > 16.0f) {
			float line_time = in.current_line * 4.0f;
			line_time = std::fminf(line_time, active_time);
			textPos.y += (line_time - 16.0f) * -fontSize * 0.25f;
		}
		draw_list->AddText(MyDefaultFont, fontSize, textPos, 0xFFFFFFFF, in.text.c_str(), in.text.c_str() + in.text.size());

		draw_list->PopClipRect();
	}
// end
}

size_t __fastcall SubtitleRenderer_GetCurrentSubtitle(LPCWSTR pAudioName, size_t nameSize) {
	if (g_SubtitleRenderer) {
		g_SubtitleRenderer->GetCurrentSubtitle(pAudioName, nameSize);
	}
	return nameSize;
}

void __fastcall SubtitleRenderer_ClearCurrentSubtitle() {
	if (g_SubtitleRenderer) {
		EnterCriticalSection(&g_SubtitleRenderer->csSubtitle);
		g_SubtitleRenderer->CurrentSubtitle.text_size = 0;
		g_SubtitleRenderer->CurrentSubtitle.active_time = 0;
		LeaveCriticalSection(&g_SubtitleRenderer->csSubtitle);
	}
}
