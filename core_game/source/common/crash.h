#pragma once
#include <string>

namespace error_handling
{
	enum class error_source
	{
		core, mod
	};

	/*
		crash
		creates window contating error message and closes the game 
	*/
	void crash(error_source source, std::string function, std::string text);
	void show_crash_info(std::string text);
}