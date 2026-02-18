#pragma once
#include <d3d11.h>
#include "Base/SSE.hpp"

namespace DigitRenderer {
    typedef std::vector<BYTE> DigitTextByte;

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
        DigitRendererChar_PERCENT,
    };

    enum DigitRendererAlign_ : int {
        DigitRendererAlign_Left,
        DigitRendererAlign_Center,
		DigitRendererAlign_Right,
    };

    typedef struct DigitFontControl_t {
		int effectTime, fadeEnable; // fadeEnable only 0 or 1, when it is 1, effectTime is fade time, otherwise it's scale time.
        int renderIndex, i_fontSize;
        float f_fontSize; // yeah, float version
        int charAlignType; // use DigitRendererAlign_
		int charIndex, charAlign; // charAlign is forward movement distance
    }*PDigitFontControl;

    // constant buffer data structure
    typedef struct alignas(16) DigitConstants_t {
        float ScreenSize[2];
        float ScreenScale[2]; // since it has already been calculated, it is also written to it.
        float ScaleSpeed, FadeSpeed, pad18[2];
        __m128 BorderColor;
    }*PDigitConstants;

    DigitTextByte FormatNumberToDigitRendererChars_Damage(float number);
    DigitTextByte StringToDigitRendererChars(const std::string& str);

	int SetDigitRendererAlign(int charTotal, int alignType);
}

