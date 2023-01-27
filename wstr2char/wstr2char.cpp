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

    std::wstring wstr;
    std::vector<unsigned char> buffer(256,0);
    std::wcout << "input string: ";
    std::wcin >> wstr;

    int wstrSize = wstr.size() * 2;
    memcpy(&buffer[0], wstr.c_str(), wstrSize);

    std::wcout << "\nout byte: ";
    for (int i = 0; i < wstrSize; i++) {
        std::wcout << std::to_wstring((int)buffer[i]);
       
        if (i+1 == wstrSize)
            std::wcout << L",0,0";
        else {
            std::wcout << L",";
        }
    }
    std::wcout << "\n";

    system("pause");

    return 0;
}