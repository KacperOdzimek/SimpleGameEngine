#include "crash.h"

#ifndef WIN32
#include "windows.h"
#include <codecvt>
#include <filesystem>
#endif

/*
	if true, new crashes would be ignored
	added so crash function would not make an infinite loop with 
	main functions try - catch statement
*/
static bool pending_unload = false;

void error_handling::show_crash_info(std::string text)
{
	if (pending_unload)
		return;

#ifdef WIN32
	wchar_t* title = (wchar_t*)L"GAME ERROR\0\u2210";
	std::filesystem::path a{text};
	MessageBox(NULL, (wchar_t*)a.u16string().c_str(), title, MB_ICONERROR | MB_OK);
#endif
}

void error_handling::crash(error_source source, std::string function, std::string text)
{
	if (pending_unload)
		return;

	std::string full_text;
	switch (source)
	{
	case error_handling::error_source::core:
		full_text = "[core]";
		break;;
	case error_handling::error_source::mod:
		full_text = "[mod]";
		break;
	}

	full_text.append(function.c_str());
	full_text.push_back(':');
	full_text.push_back('\n');
	full_text.append(text.c_str());

	show_crash_info(full_text);

	pending_unload = true;
	throw std::exception{"Shutdown call"};
}