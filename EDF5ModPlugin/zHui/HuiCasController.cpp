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
#include "HuiCasController.h"

// fp0 is [0], fp3 is [3]
#define MY_SHUFFLE(fp0,fp1,fp2,fp3) (((fp3) << 6) | ((fp2) << 4) | ((fp1) << 2) | ((fp0)))
// unused xmm register location
#define my_unused_xmm 0

void module_CasControllerInitialization(PBYTE hmodEXE)
{
	// EDF5.exe+5F36F0
	WriteHookToProcess((void*)(hmodEXE + 0x5F36F0), (void*)&nop3, 3U);
	hookGameBlockWithInt3((void*)(hmodEXE + 0x5F3702), (uintptr_t)module_CanmGetPlayAnimationCoordinate);
	WriteHookToProcess((void*)(hmodEXE + 0x5F3702 + 15), (void*)&nop5, 5U);
}

void __fastcall module_CanmGetPlayAnimationCoordinate(void*, __m128* outF4, const PCanmAnimationData pData, const PCanmInputCoordinate inV2, int index, float factor)
{
	intptr_t ofsKF = pData->keyframeOffset;
	uintptr_t pKFdata = uintptr_t(pData) + ofsKF;

	if (pData->type != 625) {
		__m128i temp;
		temp.m128i_u32[3] = 1;

		intptr_t currentKF = index * 3;
		UINT16* pKF = (UINT16*)pKFdata + currentKF;
		temp.m128i_u32[0] = pKF[0];
		temp.m128i_u32[1] = pKF[1];
		temp.m128i_u32[2] = pKF[2];
		__m128 current = _mm_cvtepi32_ps(temp);

		intptr_t nextKF = (index + 1) * 3;
		pKF = (UINT16*)pKFdata + nextKF;
		temp.m128i_u32[0] = pKF[0];
		temp.m128i_u32[1] = pKF[1];
		temp.m128i_u32[2] = pKF[2];
		__m128 next = _mm_cvtepi32_ps(temp);

		__m128 out = _mm_sub_ps(next, current);
		out = _mm_mul_ps(out, _mm_set_ps1(factor));
		out = _mm_add_ps(out, current);
		out = _mm_mul_ps(out, inV2->delta);
		out = _mm_add_ps(out, inV2->base);

		*outF4 = out;
	}
	else {
		__m128* pKF = (__m128*)pKFdata;

		__m128 resultQ = vector_QuaternionSlerp(pKF[index], pKF[index + 1], factor);
		*outF4 = vector_QuaternionToEuler(resultQ);

		/*
		__m128 current = pKF[index];
		__m128 next = pKF[index+1];

		__m128 cxnV4 = _mm_mul_ps(current, next);
		float dot = cxnV4.m128_f32[0] + cxnV4.m128_f32[1] + cxnV4.m128_f32[2] + cxnV4.m128_f32[3];
		if (dot < 0.0f) {
			dot = -dot;
			next = _mm_xor_ps(_mm_set_ps1(-0.0f), next);
		}

		float theta = acos(dot);
		float sinTheta = sin(theta);
		float w1, w2;
		if (sinTheta > 0.001f) {
			w1 = sin((1.0f - factor) * theta) / sinTheta;
			w2 = sin(factor * theta) / sinTheta;
		}
		else {
			w1 = 1.0f - factor;
			w2 = factor;
		}

		current = _mm_mul_ps(current, _mm_set_ps1(w1));
		next = _mm_mul_ps(next, _mm_set_ps1(w2));
		current = _mm_add_ps(current, next);

		float qx = current.m128_f32[0];
		float qy = current.m128_f32[1];
		float qz = current.m128_f32[2];
		float qw = current.m128_f32[3];

		float sinr_cosp = 2 * (qw * qx + qy * qz);
		float cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
		outF4->m128_f32[0] = std::atan2(sinr_cosp, cosr_cosp);

		float sinp = 2 * (qw * qy - qz * qx);
		outF4->m128_f32[1] = std::asin(sinp);

		float siny_cosp = 2 * (qw * qz + qx * qy);
		float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
		outF4->m128_f32[2] = std::atan2(siny_cosp, cosy_cosp);
		outF4->m128_f32[3] = 1.0f;*/
	}
	// end
}

__m128 __vectorcall vector_QuaternionSlerp(__m128 q1, __m128 q2, float factor)
{
	float dot = _mm_dp_ps(q1, q2, 0b11110001).m128_f32[0];
	if (dot < 0.0f) {
		dot = -dot;
		q2 = _mm_xor_ps(_mm_set_ps1(-0.0f), q2);
	}

	float theta = acos(dot);
	float sinTheta = sin(theta);
	float w1, w2;
	if (sinTheta > 0.001f) {
		w1 = sin((1.0f - factor) * theta) / sinTheta;
		w2 = sin(factor * theta) / sinTheta;
	}
	else {
		w1 = 1.0f - factor;
		w2 = factor;
	}

	q1 = _mm_mul_ps(q1, _mm_set_ps1(w1));
	q2 = _mm_mul_ps(q2, _mm_set_ps1(w2));
	return _mm_add_ps(q1, q2);
}

__m128 __vectorcall vector_QuaternionToEuler(__m128 in)
{
	alignas(16) float out[4];
	out[3] = 1.0f;

	// START: get out's x, z
	__m128 qxq = _mm_mul_ps(in, in);
	// [0] is x+y, [1] is y+z
	__m128 q2_out = _mm_add_ps( _mm_shuffle_ps(qxq, qxq, MY_SHUFFLE(0, 1, my_unused_xmm, my_unused_xmm)),
								_mm_shuffle_ps(qxq, qxq, MY_SHUFFLE(1, 2, my_unused_xmm, my_unused_xmm)));
	q2_out = _mm_add_ps(q2_out, q2_out);
	q2_out = _mm_sub_ps(_mm_set_ps1(1), q2_out);

	// [0] is wx, [1] is yz, [2] is wz, [3] is xy
	qxq = _mm_mul_ps( _mm_shuffle_ps(in, in, MY_SHUFFLE(3, 1, 3, 0)),
					  _mm_shuffle_ps(in, in, MY_SHUFFLE(0, 2, 2, 1)));
	// [0] is 0+1, [2] is 2+3
	qxq = _mm_add_ps(qxq, _mm_shuffle_ps(qxq, qxq, MY_SHUFFLE(1, my_unused_xmm, 3, my_unused_xmm)));
	qxq = _mm_add_ps(qxq, qxq);
	// set value
	out[0] = std::atan2(qxq.m128_f32[0], q2_out.m128_f32[0]);
	out[2] = std::atan2(qxq.m128_f32[2], q2_out.m128_f32[1]);
	// END: get x,z

	// START: get out's y
	// [0] is w*y, [1] is z*x
	q2_out = _mm_mul_ps( _mm_shuffle_ps(in, in, MY_SHUFFLE(3, 2, my_unused_xmm, my_unused_xmm)),
						 _mm_shuffle_ps(in, in, MY_SHUFFLE(1, 0, my_unused_xmm, my_unused_xmm)));
	float sinp = 2 * (q2_out.m128_f32[0] - q2_out.m128_f32[1]);
	out[1] = std::asin(sinp);
	// END: get y 

	return _mm_load_ps(out);
}
