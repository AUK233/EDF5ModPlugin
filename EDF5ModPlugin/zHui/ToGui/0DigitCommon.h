#pragma once
#include <d3d11.h>
#include "Base/SSE.hpp"

namespace DigitRenderer {
	typedef std::vector<BYTE> DigitTextByte;

	// used to configure our solid-colour textures
	enum DigitRendererColor_ : int {
		DigitRendererColor_Red, // 255, 0, 0, 255
		DigitRendererColor_Green, // 0, 255, 0, 255
		DigitRendererColor_GreenHalfA, // 0, 255, 0, 127
		DigitRendererColor_Yellow, // 255, 255, 0, 255
		DigitRendererColor_Blue, // 0, 0, 255, 255
		DigitRendererColor_Cyan, // 0, 255, 255, 255
		DigitRendererColor_White, // 255, 255, 255, 255
		DigitRendererColor_WhiteA190, // 255, 255, 255, 190
		DigitRendererColor_ALL,
	};

	// character index for shader, should be same as shader code.
	enum DigitRendererChar_ : int {
		DigitRendererChar_0,
		DigitRendererChar_1,
		DigitRendererChar_2,
		DigitRendererChar_3,
		DigitRendererChar_4,
		DigitRendererChar_5,
		DigitRendererChar_6,
		DigitRendererChar_7,
		DigitRendererChar_8,
		DigitRendererChar_9,
		DigitRendererChar_DOT,
		// second line
		DigitRendererChar_SUB,
		DigitRendererChar_PERCENT,
		DigitRendererChar_LAST = 21 // last
	};

	typedef struct DigitFontControl_t {
		int renderIndex; // use DigitRendererChar_
		int charIndex, charTotal, i_fontSize;
		int scaleFactor, fadeFactor;
		float time, f_fontSize;
		float charAlignFactor; // 0 is left, 0.5 is center, 1 is right
	}*PDigitFontControl;

	// constant buffer data structure
	typedef struct alignas(16) DigitConstants_t {
		float ScreenSize[2];
		float ScreenScale[2]; // since it has already been calculated, it is also written to it.
		float ScaleSpeed, FadeSpeed, pad18[2];
		__m128 BorderColor;
	}*PDigitConstants;

	// =============================================================
	union BaseDigitData_u {
		float fp32;
		int s32;
	};

	typedef struct alignas(16) DigitData_Damage_t {
		BaseDigitData_u value;
		float time, effectTime;
		int scaleFactor, fadeFactor;
	}*PDigitData_Damage;

	typedef struct DigitData_DamageInHit_t {
		__m128 pos;
		BaseDigitData_u value;
	}*PDigitData_DamageInHit;


	enum DigitRendererWeaponStatus_ : int {
		DigitRendererWeaponStatus_Normal,
		DigitRendererWeaponStatus_Reload, // in reloading
		DigitRendererWeaponStatus_Charge, // in charging
		DigitRendererWeaponStatus_Credit, // in waiting for credit
	};

	typedef struct alignas(16) DigitData_Weapon_t {
		BaseDigitData_u value;
		int pad4;
		void* pWeapon;
		int effectTime, timeIncrement;
		int weaponAlignType, weaponStatus;
	}*PDigitData_Weapon;

	typedef struct alignas(16) DigitData_Weapon_Render_t {
		int isEnabled, scaleTime;
		DigitTextByte text;
		__m128 pos;
		int colorIndex;
	}*PDigitData_Weapon_Render;

	DigitTextByte FormatNumberToDigitRendererChars_Ammo(int number);
	DigitTextByte FormatNumberToDigitRendererChars_Damage(float number);
	DigitTextByte FormatNumberToDigitRendererChars_Percentage(float number);
	DigitTextByte FormatNumberToDigitRendererChars_Second(float number);
	DigitTextByte StringToDigitRendererChars(const std::string& str);
	std::string UnicodeToUTF8(LPCWSTR wstr, int length = -1);
}

