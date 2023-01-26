#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#include <iostream>
#include <locale>
#include <locale.h>

# include <io.h>
# include <fcntl.h>

int main()
{
    char cp_utf16le[] = ".1200";
    setlocale(LC_ALL, cp_utf16le);
    int shisjfi = _setmode(_fileno(stdout), _O_WTEXT);

    float n0 = 0;
    int n1 = 0;
    int n3 = 0;
    float n4 = 0;
    float n5 = 0;

    float v10 = 1.0f;

    std::wcout << "Please input values (node in ptr):\n--->Node 0 (Base Value): ";
    std::wcin >> n0;

    std::wcout << "--->Node 1: ";
    std::wcin >> n1;

    std::wcout << "--->Node 4: ";
    std::wcin >> n4;

    std::wcout << "--->Node 5: ";
    std::wcin >> n5;

    std::wcout << "--->Need for Star Rating: ";
    std::wcin >> n3;

    if (n0 < 0.0)
    {
        n0 = -n0;
        v10 = -1.0;
    }

    float v12 = n0 * n4;
    float v14 = powf(n3 / 5.0f, n5) * v12;

    float v7 = 0.0f;
    if ((n1 & 3) != 0) {
       if ((n1 & 3) == 1) {
            v7 = fmaxf(0.0f, (v12 + n0) - v14);
       }
    }
    else {
        v7 = fmaxf(0.0f, (n0 - v12) + v14);
    }

    float v16 = v7 * v10;
    int i16 = (int)(v16 + 0.5f);

    std::wcout << "\n--->Result: ";
    std::wcout << std::to_wstring(v16);
    std::wcout << "\n--->Result (int): ";
    std::wcout << std::to_wstring(i16);
    std::wcout << "\n";

    system("pause");

    return 0;
}