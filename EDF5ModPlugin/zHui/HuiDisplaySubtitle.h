#pragma once

extern "C" {
	// Get subtitle address
	int __fastcall GetSubtitleTextAddress(SubtitleTextStruct* startAddr, WCHAR* pAudioName, int nameSize, int noPos);
	// initialising talk subtitles
	void __fastcall InitializeTalkSubtitleString();
	void __fastcall PushTalkSubtitleString(WCHAR* pAudioName, size_t nameSize);
	void __fastcall DisplayTalkSubtitleString(void* pScript);
}

//void __fastcall LoadSubtitleFileToMap();
void __fastcall LoadSelectedSubtitleFile();
void InitializeSubtitlePointer();
void hookDisplaySubtitle(PBYTE hmodEXE);
