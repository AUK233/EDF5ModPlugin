#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>
#include <stdexcept>
#include <list>

#include "utiliy.h"

// Injects hook into game process
void __fastcall WriteHookToProcess(void *addr, void *data, size_t len) {
	DWORD oldProtect;
	VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(addr, data, len);
	VirtualProtect(addr, len, oldProtect, &oldProtect);
	// Refresh cpu instruction?
	//FlushInstructionCache(handleEXE, addr, len);
}

// update game's original functions
void __fastcall hookGameBlock(void *targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
	    0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
	    0xFF, 0xE0                                                  // jmp rax
	};
	memcpy(&hookFunction[2], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

void __fastcall hookGameBlockRAXWithInt3(void* targetAddr, uint64_t dataAddr)
{
	uint8_t hookFunction[] = {
		   0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
		   0xFF, 0xE0, // jmp rax
		   0xCC        // int3
	};
	memcpy(&hookFunction[2], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// update game's original functions with 14 bytes
void __fastcall hookGameBlock14(void* targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,            // jmp
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // addr
	};
	memcpy(&hookFunction[6], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// update game's original functions with interruption, need 15 bytes
void __fastcall hookGameBlockWithInt3(void* targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// jmp
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // addr
		0xCC											// int3
	};
	memcpy(&hookFunction[6], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}

// update game's original functions with call, need 12bytes
void __fastcall CallGameBlock(void* targetAddr, uint64_t dataAddr) {
	uint8_t hookFunction[] = {
		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, addr
		0xFF, 0xD0                                                  // call rax
	};
	memcpy(&hookFunction[2], &dataAddr, sizeof(dataAddr));

	WriteHookToProcess(targetAddr, hookFunction, sizeof(hookFunction));
}
