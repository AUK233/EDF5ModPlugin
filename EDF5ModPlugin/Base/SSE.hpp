#pragma once
// SSE4.2
#include <nmmintrin.h>

#define MY_SHUFFLE(fp0,fp1,fp2,fp3) (((fp3) << 6) | ((fp2) << 4) | ((fp1) << 2) | ((fp0)))
// unused xmm register location
#define my_unused_xmm 0

__declspec(align(16)) struct Matrix3D_t {
	float m0[4];
	float m1[4];
	float m2[4];
	float pos[4];
};

// 2 xyz to determine direction
struct MatrixCamera_t {
	float m0[4];
	float m1[4];
};

struct Matrix4x6_t {
	float m0[4];
	float m1[4];
	float m2[4];
	float m3[4];
	float m4[4];
	float m5[4];
};
