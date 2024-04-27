#pragma once
#include <vector>
#include <string>
#include "../objects/buffer.h"
#include "../data/data_types.h"

namespace graphics_abstraction
{
	struct vertex_layout : public object
	{
	protected:
		std::vector<graphics_abstraction::data_type> vertex_components;
		unsigned int additional_bytes = 0;

		virtual void bind(internal::pipeline& pipeline) final
		{
			pipeline.vertex_layout = this;
		}
		
		virtual void set_layout_impl() = 0;

	public:
		virtual void set_layout(std::vector<graphics_abstraction::data_type> _vertex_components) final
		{
			vertex_components = _vertex_components;
			set_layout_impl();
		}
		virtual uint32_t get_vertex_size() = 0;
	};

	struct vertex_layout_builder : public builder
	{
		std::vector<data_type> vertex_components;
		unsigned int additional_bytes = 0;
	protected:
		virtual object* build_abs(api*& api) final
		{
			if (vertex_components.size() == 0)
			{
				api->report_error(
					{
						error_source::abstraction,
						3,
						{}
					}
				);

				return nullptr;
			}
			for (auto& vc : vertex_components)
			{
				if (vc >= data_type::mat2x2)
				{
					api->report_error(
						{
							error_source::abstraction,
							3,
							{}
						}
					);

					return nullptr;
				}
			}

			return this->build_impl(api);
		}
	};
}