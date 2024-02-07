#pragma once
#include "asset.h"
#include <vector>

namespace rendering
{
	class renderer;
}

namespace graphics_abstraction
{
	struct buffer;
}

namespace assets
{
	struct mesh : public asset
	{
		friend rendering::renderer;
	private:
		enum class draw_type 
		{
			indexed_triangles, triangle_strip
		};
		draw_type draw_type;
		graphics_abstraction::buffer* vertices;
		graphics_abstraction::buffer* indicies;
	public:
		mesh(const std::vector<float>& vertices, const std::vector<int>& indicies);
		mesh(const std::vector<float>& vertices);
		~mesh();
	};
}