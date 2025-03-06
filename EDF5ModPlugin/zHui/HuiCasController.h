#pragma once
#include <nmmintrin.h>

typedef struct CanmAnimationData_t {
	INT16 type, keyframeCount;
	float initial[3], delta[3];
	int keyframeOffset;
} *PCanmAnimationData;

typedef struct CanmInputCoordinate_t {
	__m128 base;
	__m128 delta;
} *PCanmInputCoordinate;

__declspec(align(16)) typedef struct CanmInputQuaternion_t {
	float x, y, z, w;
} *PCanmInputQuaternion;

void module_CasControllerInitialization(PBYTE hmodEXE);

void __fastcall module_CanmGetPlayAnimationCoordinate(void*, __m128* outF4, const PCanmAnimationData pData, const PCanmInputCoordinate inV2, int index, float factor);

__m128 __vectorcall vector_QuaternionSlerp(__m128 q1, __m128 q2, float factor);
__m128 __vectorcall vector_QuaternionToEuler(__m128 in);

