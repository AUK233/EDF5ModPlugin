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

#include "utiliy.h"
#include "commonNOP.h"
#include "GameFunctionInASM.h"
#include "XGS_Weapon_Throw.h"

extern "C" {
	uintptr_t Weapon_Throw_VFT; // EDF5.exe+EBB7C8
	void __fastcall ASMxgs_Weapon_ThrowInit();

	void __fastcall ASMcharacterGhostCameraSetIndicatorLine();
	uintptr_t characterGhostCameraSetIndicatorLineRetAddr;

	void __fastcall ASMxgs_Weapon_ThrowFuncP0();
	uintptr_t xgs_Weapon_ThrowFuncP0Ret;

	void __fastcall ASMxgs_Weapon_ThrowFuncFree();
	uintptr_t xgs_Weapon_ThrowFuncFreeRet;
}

void xgs_Weapon_Throw_Init(PBYTE hmodEXE)
{
	Weapon_Throw_VFT = (uintptr_t)(hmodEXE + 0xEBB7C8);
	// EDF5.exe+3A8D0F
	hookGameBlockWithInt3((void*)(hmodEXE + 0x3A8D0F), (uintptr_t)ASMxgs_Weapon_ThrowInit);
	WriteHookToProcess((void*)(hmodEXE + 0x3A8D0F + 15), (void*)&nop3, 3U);

	// EDF5.exe+9EF45
	//hookGameBlockWithInt3((void*)(hmodEXE + 0x9EF45), (uintptr_t)ASMcharacterGhostCameraSetIndicatorLine);
	characterGhostCameraSetIndicatorLineRetAddr = (uintptr_t)(hmodEXE + 0x9EF45 + 15);

	// EDF5.exe+EBB7C8
	uintptr_t funcP0Addr = (uintptr_t)ASMxgs_Weapon_ThrowFuncP0;
	WriteHookToProcess((void*)(Weapon_Throw_VFT + 0), &funcP0Addr, 8U);
	xgs_Weapon_ThrowFuncP0Ret = (uintptr_t)(hmodEXE + 0x3AE1D0);

	// EDF5.exe+EBB810
	uintptr_t funcFreeAddr = (uintptr_t)ASMxgs_Weapon_ThrowFuncFree;
	WriteHookToProcess((void*)(Weapon_Throw_VFT + 0x48), &funcFreeAddr, 8U);
	xgs_Weapon_ThrowFuncFreeRet = (uintptr_t)(hmodEXE + 0x3A8D3A);
}

int __fastcall xgs_Weapon_Throw_SetIndicatorPos(PG_Weapon_Throw pThis, __m128* pInGravity)
{
	auto pVector = (__m128*)(pThis->Info.pVector168);
	auto v_basePos = pVector[8];

	static const __m128 v_frame = { 60, 60, 60, 60 };
	__m128 v_AmmoOwnerMove = _mm_set_ps1(pThis->Info.AmmoOwnerMove);
	v_AmmoOwnerMove = _mm_mul_ps(v_AmmoOwnerMove, pThis->vector130);
	v_AmmoOwnerMove = _mm_div_ps(v_AmmoOwnerMove, v_frame);
	//v_AmmoOwnerMove.m128_f32[3] = 1.0f; //xmm8

	auto v_FireVector = pThis->Info.Fire.Vector;
	auto v_xmm3 = _mm_mul_ps(_mm_set_ps1(v_FireVector.m128_f32[0]), pVector[5]);
	auto v_xmm2 = _mm_mul_ps(_mm_set_ps1(v_FireVector.m128_f32[1]), pVector[6]);
	auto v_xmm0 = _mm_mul_ps(_mm_set_ps1(v_FireVector.m128_f32[2]), pVector[7]);
	v_FireVector = _mm_add_ps(v_xmm3, v_xmm2);
	v_FireVector = _mm_add_ps(v_FireVector, v_xmm0);
	//v_FireVector.m128_f32[3] = pThis->Info.Fire.Vector.m128_f32[3];

	float ammo_speed = std::fmaxf(pThis->Ammo.data.Speed, 0);

	float speed_dot = _mm_dp_ps(v_FireVector, v_FireVector, 0b01110001).m128_f32[0];
	__m128 v_speed = v_FireVector;
	if (speed_dot > 0) {
		speed_dot = std::sqrtf(speed_dot);
		ammo_speed /= speed_dot;
		v_speed = _mm_mul_ps(v_speed, _mm_set_ps1(ammo_speed));
	}

	v_speed = _mm_add_ps(v_speed, v_AmmoOwnerMove);
	v_speed.m128_f32[3] = 0;

	__m128 v_gravity = pInGravity[2];
	static const __m128 v_gravityFactor = { 3600, 3600, 3600, 3600 };
	v_gravity = _mm_mul_ps(v_gravity, _mm_set_ps1(pThis->Ammo.data.GravityFactor));
	v_gravity = _mm_div_ps(v_gravity, v_gravityFactor);
	v_gravity.m128_f32[3] = 0;

	__m128 v_start_pos = v_basePos;
	__m128 v_end_pos = _mm_add_ps(v_basePos, v_speed);
	__m128 v_out = v_end_pos;
	for (int i = 0; i < 240; i++) {
		auto isHit = ASMLaserCollisionDetection(&v_out, &v_start_pos);
		if (isHit) {
			pThis->indicatorCircle.StartPos = v_out;
			return 1;
		}

		v_speed = _mm_add_ps(v_speed, v_gravity);
		v_start_pos = v_end_pos;
		v_end_pos = _mm_add_ps(v_end_pos, v_speed);
		v_out = v_end_pos;
	}

	return 0;
}
