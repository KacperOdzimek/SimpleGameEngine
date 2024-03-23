#pragma once
#include "asset.h"
#include <vector>

namespace rendering
{
	class renderer;
}

namespace assets
{
	class assets_manager;
}

namespace graphics_abstraction
{
	struct shader;
	struct vertex_layout;
}

namespace assets
{
	struct shader : public asset
	{
		friend rendering::renderer;
		friend assets::assets_manager;
	private:
		graphics_abstraction::shader* _shader;
		graphics_abstraction::vertex_layout* vertex_layout;
	public:
		shader(std::string& vertex_shader, std::string& pixel_shader,
			std::vector<uint32_t>& hashed_layout);
		~shader();
	};
}