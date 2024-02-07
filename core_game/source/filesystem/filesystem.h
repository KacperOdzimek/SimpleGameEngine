#pragma once

#include <fstream>

struct filesystem
{
	/*
	set_mod_asset_path
	sets path that will be inserted in place of "mod/" path prefix
	*/
	static void set_mod_asset_path(std::string path);
	/*
	set_core_asset_path
	sets path that will be inserted in place of "core/" path prefix
	*/
	static void set_core_asset_path(std::string path);
	/*
	get_package
	returns first part of the path eg. 
	get_package("mod/behaviors/...") == "mod/"
	*/
	static std::string get_package(std::string path);
	/*
	get_global_path
	returns on-drive path version of the given path
	*/
	static std::string get_global_path(std::string path);
	/*
	load_file
	loads file
	*/
	static std::fstream load_file(std::string path_with_extension);

	struct image_file
	{
		unsigned char* image_source_pointer = nullptr;
		int width, height, color_channels = 0;
		~image_file();
	};
	/*
	load_image
	loads image
	*/
	static std::unique_ptr<image_file> load_image(std::string path_with_extension);
};