#pragma once
#include "asset.h"

namespace audio
{
	class audio_manager;
}

namespace assets
{
	struct sound : public asset
	{
	friend audio::audio_manager;
	protected:
		std::string file_path;
	public:
		sound(std::string file_path);
		~sound();
		float get_length();
	};
}