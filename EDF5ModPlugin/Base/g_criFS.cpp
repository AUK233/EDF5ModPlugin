
#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "utiliy.h"
#include "g_criFS.h"

extern "C" {
	uintptr_t CriFileSystem_OpenFile; // EDF5.exe+83D90
	uintptr_t CriFileSystem_ReleaseData; // EDF5.exe+71630
}

void __fastcall CriFileSystem_Initialize(PBYTE hmodEXE) {
	CriFileSystem_OpenFile = (uintptr_t)(hmodEXE + 0x83D90);
	CriFileSystem_ReleaseData = (uintptr_t)(hmodEXE + 0x71630);
}

bool CriFileSystemGet_t::Open(const std::wstring& filename) {
	typedef bool(__fastcall* callFunc)(PCriFileSystemGet out, const std::wstring& filename);
	callFunc func = (callFunc)CriFileSystem_OpenFile;
	auto ret = func(this, filename);
	return ret;
}

void CriFileSystemGet_t::AddRef() {
	if (!manager) return;

	auto cs = &manager->cs;

	EnterCriticalSection(cs);
	fs->counter++;
	LeaveCriticalSection(cs);
}

void CriFileSystemGet_t::Release() {
	if (!manager) return;

	auto cs = &manager->cs;

	EnterCriticalSection(cs);

	fs->counter -= 1;
	if(fs->counter == 0){
		uintptr_t temp;
		typedef void(__fastcall* callFunc)(PCriFileSystemManager manager, uintptr_t* saveRSP, PCriFileSystem fs);
		callFunc func = (callFunc)CriFileSystem_ReleaseData;
		func(manager, &temp, fs);
	}

	LeaveCriticalSection(cs);
}
