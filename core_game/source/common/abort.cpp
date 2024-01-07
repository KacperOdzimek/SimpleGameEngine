#include "abort.h"

#ifndef WIN32
#include "windows.h"
#include <codecvt>
#endif

void abort(std::string text)
{
#ifdef WIN32
	wchar_t* title = (wchar_t*)L"GAME ERROR\0\u2210";
	std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> converter;
	std::u16string text16 = converter.from_bytes(text);
	MessageBox(NULL, (wchar_t*)text16.c_str(), title, MB_ICONERROR | MB_OK);
	abort();
#else
	abort();
#endif
}