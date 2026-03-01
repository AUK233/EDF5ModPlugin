#pragma once
#include "Base/SSE.hpp"

namespace DigitRenderer {
	void SubtitleRenderer_Initialization(PBYTE hmodEXE);
	void WINAPI SubtitleRenderer_InitSubtitle();

	struct SubtitleTextStruct_t {
		std::string text; // yeah, utf8
		int text_size;
		int current_line; // preprocess current text to how many lines.
	};
	void __fastcall SubtitleRenderer_CheckSubtitle(LPCWSTR pWstr, int textSize, SubtitleTextStruct_t& data, int language);

	struct SubtitleTextShowStruct_t : SubtitleTextStruct_t {
		float active_time;
		float align_factor; // If only one line, it will be displayed at position.
		int bIsScroll;
	};

	class SubtitleRenderer_t {
	public:
		std::vector<std::wstring> SubtitleName[4];
		std::vector<int> SubtitleName_index[4];
		std::vector<SubtitleTextStruct_t> SubtitleList[4];

		CRITICAL_SECTION csSubtitle;
		SubtitleTextShowStruct_t CurrentSubtitle;
	public:
		void GetCurrentSubtitle(LPCWSTR pAudioName, size_t nameSize);
		void DisplayCurrentSubtitle(PG_SoldierBase pPlayer);
	};
	typedef SubtitleRenderer_t* PSubtitleRenderer;
}

extern "C" {
	size_t __fastcall SubtitleRenderer_GetCurrentSubtitle(LPCWSTR pAudioName, size_t nameSize);
	void __fastcall SubtitleRenderer_ClearCurrentSubtitle();
}

