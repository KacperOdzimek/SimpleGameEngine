#pragma once

#include <fstream>

namespace filesystem
{
	/*
	load_file_relative_path
	loads file from relative path
	-l-
	anvaible fomats:
	core/{path} - loads file from core package
	mod/{path} - loads file from path
	user is required to provide paths with left slash and does not type file extension
	*/
	std::fstream load_json_file_relative_path(std::string path, std::string& mod_path);
}