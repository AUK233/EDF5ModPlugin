#pragma once

__declspec(align(16)) typedef struct EDFHuiChgDlgMsgButton_t {
	void* vf_table;
	void* pCharacter;
	BYTE pad[0x28];
	void* pVFTable;
} EDFHuiChgDlgMsgButton;
static_assert(sizeof(EDFHuiChgDlgMsgButton_t) == 0x40);

__declspec(align(16)) typedef struct EDFHuiChgDlgMsgVector_t {
	BYTE wstr[0x20];
	BYTE pad1[0x38];
	WCHAR* pWstring;
	INT16 unk161;
	BYTE pad2[0x1E];
	void* pSize60;
	INT64 unk641;
} EDFHuiChgDlgMsgVector;
static_assert(sizeof(EDFHuiChgDlgMsgVector_t) == 0x90);

typedef struct EDFHuiSelectButtonVFT_t {
	void* avi;
	uintptr_t copyVFT;
	uintptr_t copyVFT8;
	uintptr_t activeButton;
	uintptr_t loadAVI;
	uintptr_t checkFree;
	uintptr_t rcx8toRax;
} EDFHuiSelectButtonVFT;
static_assert(sizeof(EDFHuiSelectButtonVFT_t) == 0x38);

typedef WCHAR* (__fastcall* func_Call4738B0)(void* v125ABD0, LPCWSTR pName);
typedef void(__fastcall* func_Call6D8D0)(void* pButtonVector, void* pVector);
typedef void(__fastcall* func_Call9C71C0)(void* pStartButton, int eachSize, int sum, void* vf_table);

// ================================================================

void module_SetHuiMoreCharacterModel(PBYTE hmodEXE);
void module_ShareCharacterModelColor(PBYTE hmodEXE);

extern "C" {
void __fastcall module_HuiPushChaModelSelButton(void* v125ABD0, void* pButtonVector, void* pCharacter);
// asm series
void __fastcall ASMHuiSetCharacterModelButton(void* pTarget, LPCWSTR wstr, void* pSource);
}
