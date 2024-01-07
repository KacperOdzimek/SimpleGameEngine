#pragma once

#include "graphics_abstraction/graphics_abstraction.h"
#include "source/utilities/equal_to.h"

#include "source/assets/shader_asset.h"

#include <set>
#include <unordered_map>

namespace entities
{
	class geometry_component;
}

namespace rendering
{
	struct pipeline_config
	{
		assets::shader* shader = nullptr;
		std::set<graphics_abstraction::functionalities> enabled_functionalities;

		bool operator== (const pipeline_config& rhs) const
		{
			return this->shader == rhs.shader && this->enabled_functionalities == rhs.enabled_functionalities;
		}
	};

	struct pipeline_buffers
	{
		graphics_abstraction::buffer* vertices;
		graphics_abstraction::buffer* indicies;
		uint32_t valid_indicies;
	};
}

template <>
struct std::hash<rendering::pipeline_config>
{
public:
	size_t operator()(const rendering::pipeline_config& config) const
	{
		return (uint64_t)config.shader;
	}
};

namespace rendering
{
	using pipelines = std::unordered_map<pipeline_config, pipeline_buffers>;
	using geometry_sources = std::unordered_map<pipeline_config, std::vector<entities::geometry_component*>>;
}