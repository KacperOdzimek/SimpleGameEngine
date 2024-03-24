#pragma once

#include <fstream>
#include <vector>

namespace filesystem
{
	/*
	set_mod_assets_directory
	sets path that will be inserted in place of "mod/" path prefix
	*/
	void set_mod_assets_directory(std::string path);
	/*
	set_core_assets_directory
	sets path that will be inserted in place of "core/" path prefix
	*/
	void set_core_assets_directory(std::string path);
	/*
	set_active_assets_directory
	sets path that will be inserted in place of "$/" path prefix
	*/
	void set_active_assets_directory(std::string path);
	/*
	set_active_assets_directory_enabled
	sets if user can use "$/" path prefix
	*/
	void set_active_assets_directory_enabled(bool enabled);
	/*
	set_mods_directory
	sets directory from which mods_manager will load mods
	*/
	void set_mods_directory(std::string path);
	/*
	get_package
	returns first part of the path eg. 
	get_package("mod/behaviors/...") == "mod/"
	*/
	std::string get_package(std::string path);
	/*
	get_owning_folder
	returns file owning folder path eg.
	mod/folder1/file.txt -> mod/folder1/
	*/
	std::string get_owning_folder(std::string path);
	/*
	get_global_path
	returns on-drive path version of the given path
	*/
	std::string get_global_path(std::string path);
	/*
	load_file
	loads file
	*/
	std::fstream load_file(std::string relative_path_with_extension);
	/*
	get_all_subfolders
	returns full paths of directory subfolders
	*/
	std::vector<std::string> get_all_subfolders(std::string dir);
	/*
	get_main_dir
	returns system dir of main.cpp file
	*/
	std::string get_main_dir();

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
	std::unique_ptr<image_file> load_image(std::string path_with_extension);
};