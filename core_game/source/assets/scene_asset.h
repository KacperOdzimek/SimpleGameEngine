#pragma once
#include "asset.h"

namespace behaviors
{
	class behaviors_manager;
}

namespace assets
{
	struct scene : public asset
	{
		friend behaviors::behaviors_manager;
	protected:
		std::string name;
	public:
		scene(std::string& lua_file_path);
	};
}