#include "sound_asset.h"

#include "source/filesystem/filesystem.h"

assets::sound::sound(std::string _file_path)
	: file_path(filesystem::get_global_path(std::move(_file_path)))
{
}

assets::sound::~sound()
{
}

float assets::sound::get_length()
{
	return 0;
}