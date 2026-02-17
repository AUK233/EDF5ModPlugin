#pragma once
#include <d3d11.h>

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

    typedef struct DigitFontControl_t {
		int effectTime, fadeEnable; // fadeEnable only 0 or 1, when it is 1, effectTime is fade time, otherwise it's scale time.
        int renderIndex, charIndex;
        int fontSize; // yeah, need to float.
    }*PDigitFontControl;

    // constant buffer data structure
    typedef struct alignas(16) DigitConstants_t {
        float ScreenSize[2];
        float OutPos[2];
        float Color[4];
        float FontSize;
    }*PDigitConstants;

    // structure to be rendered
    typedef struct alignas(16) DigitText_t {
        DigitTextByte textData;
        DigitConstants_t cbData;
    }*PDigitText;

    DigitTextByte FormatNumberToDigitRendererChars_Damage(float number);
    DigitTextByte StringToDigitRendererChars(const std::string& str);
}

