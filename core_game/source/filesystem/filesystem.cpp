#include "filesystem.h"
#include "source/common/abort.h"


std::fstream filesystem::load_json_file_relative_path(std::string path, std::string& mod_path)
{
	int split_index = path.find('/');
	std::string package = path.substr(0, split_index);
	if (package == "mod")
	{
		std::string global_path = mod_path + path.substr(split_index, path.size()) + ".json";
		std::fstream file;
		file.open(global_path);
		return file;
	}
	else if (package == "game")
	{
		//TODO
	}
	else
		abort("No such package: " + package);
}