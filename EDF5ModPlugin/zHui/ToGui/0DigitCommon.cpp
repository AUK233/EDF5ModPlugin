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

	DigitTextByte FormatNumberToDigitRendererChars_Percentage(float number)
	{
		auto curNumber = std::fmaxf(number, 0);
		curNumber = std::fminf(10000.0f, curNumber); // max is 10000.00%

		// 48 + 12 = 60 = 0x3C
		auto str = std::format("{:.2f}\x3c", curNumber);

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
			}
			else if (c == '-') {
				out.push_back(DigitRendererChar_SUB);
			}
			else {
				BYTE number = c - '0';
				// 48+12 = '<' as '%'
				if (number > DigitRendererChar_LAST) number = DigitRendererChar_LAST;
				out.push_back(number);
			}
			// end
		}
		return out;
	}
}
