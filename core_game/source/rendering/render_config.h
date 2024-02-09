#pragma once
#include <memory>
#include <vector>

namespace assets
{
	struct mesh;
	struct shader;
	struct texture;
}

namespace rendering
{
	struct render_config
	{
		std::shared_ptr<assets::mesh> mesh;
		std::shared_ptr<assets::shader> material;
		std::vector<std::shared_ptr<assets::texture>> textures;

		bool operator==(const render_config& other) const
		{
			return mesh.get() == other.mesh.get() && material.get() == other.material.get()
				&& textures == other.textures;
		}
	};
}

template <>
struct std::hash<rendering::render_config>
{
public:
	size_t operator()(const rendering::render_config& config) const
	{
		//TODO
		return (uint64_t)config.material.get();
	}
};