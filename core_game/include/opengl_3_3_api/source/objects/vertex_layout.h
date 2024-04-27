#pragma once
#include "graphics_abstraction.h"
#include "../common/opengl.h"

#include "../common/mappings.h"

namespace graphics_abstraction
{
	namespace implementations
	{
		namespace opengl_3_3_api
		{
			struct vertex_layout : public graphics_abstraction::vertex_layout
			{
				friend class opengl_3_3_api;
			public:
				vertex_layout(std::vector<graphics_abstraction::data_type>& _vertex_components, unsigned int _additional_bytes) 
				{
					vertex_components = _vertex_components;
					additional_bytes = _additional_bytes;
				};

				virtual uint32_t get_vertex_size()
				{
					int size_in_bytes = 0;
					for (auto& component : vertex_components)
					{
						switch (component)
						{
						case data_type::Bool:
							size_in_bytes += 4;
							break;
						case data_type::Int:
							size_in_bytes += 4;
							break;
						case data_type::Float:
							size_in_bytes += 4;
							break;
						case data_type::vec2:
							size_in_bytes += 8;
							break;
						case data_type::vec3:
							size_in_bytes += 12;
							break;
						case data_type::vec4:
							size_in_bytes += 16;
							break;
						}
					}
					return size_in_bytes;
				};

				virtual void set_layout_impl()
				{
				}

			protected:
				std::vector<graphics_abstraction::data_type>& get_vertex_components()
				{
					return vertex_components;
				}

				virtual void free()
				{
				}
			};

			struct vertex_layout_builder : public graphics_abstraction::vertex_layout_builder
			{
				virtual object* build_impl(graphics_abstraction::api*& api)
				{
					return new vertex_layout{vertex_components, additional_bytes};
				}
			};
		}
	}
}