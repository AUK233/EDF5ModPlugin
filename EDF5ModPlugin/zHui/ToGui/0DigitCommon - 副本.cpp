#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "0DigitCommon.h"

namespace DigitRenderer {
	DigitTextByte FormatNumberToDigitRendererChars_Damage(float number)
	{
		std::string str;
		if (number >= 100.0f) {
			str = std::format("{:.0f}", number);
		} else if (number >= 10.0f) {
			str = std::format("{:.1f}", number);
		} else {
			str = std::format("{:.2f}", number);
		}

		auto out = StringToDigitRendererChars(str);
		return out;
	}

	DigitTextByte StringToDigitRendererChars(const std::string& str)
	{
		DigitTextByte out;
		out.reserve(str.size());
		for (char c : str) {
			if (c == '.') {
				out.push_back(DigitRendererChar_DOT);
				continue;
			}

			BYTE number = c - '0';
			// 0x3A ':' as '%'
			if (number > DigitRendererChar_PERCENT) number = 0;
			out.push_back(number);
		}
		return out;
	}

	int SetDigitRendererAlign(int charTotal, int alignType)
	{
		if (alignType < DigitRendererAlign_Center) return 0;

		if (alignType == DigitRendererAlign_Center) return charTotal / 2;

		if (alignType > DigitRendererAlign_Center) return charTotal;
	}
}
