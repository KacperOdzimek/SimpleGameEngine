#pragma once
#include <vector>
#include <string>
#include "../data/data_types.h"

namespace graphics_abstraction
{
	struct shader : public object
	{
	protected:
		virtual void bind(internal::pipeline& pipeline) final
		{
			pipeline.shader = this;
		}
		virtual void set_uniform_value_impl(std::string uniform_name, data_type uniform_data_type, void* value) = 0;
		virtual void set_uniform_value_impl(std::string uniform_name, data_type uniform_data_type, std::vector<float> values) = 0;
		virtual void set_uniform_value_impl(std::string uniform_name, data_type uniform_data_type, int value) = 0;
	public:
		virtual void set_uniform_value(std::string uniform_name, data_type uniform_data_type, void* value) final
		{
			set_uniform_value_impl(uniform_name, uniform_data_type, value);
		};
		virtual void set_uniform_value(std::string uniform_name, data_type uniform_data_type, std::vector<float> values) final
		{
			if (uniform_data_type == data_type::Int || uniform_data_type == data_type::Bool)
				owner->report_error({
						error_source::abstraction, 8, {}
					});
			else
				set_uniform_value_impl(uniform_name, uniform_data_type, values);
		}
		virtual void set_uniform_value(std::string uniform_name, data_type uniform_data_type, int value)
		{
			if (!(uniform_data_type == data_type::Int || uniform_data_type == data_type::Bool))
				owner->report_error({
						error_source::abstraction, 8, {}
					});
			else
				set_uniform_value_impl(uniform_name, uniform_data_type, value);
		}
	};

	struct shader_builder : public builder
	{
		std::vector<std::string> code;
	protected:
		virtual object* build_abs(graphics_abstraction::api*& api) final
		{
			if (code.size() < 2)
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

			return this->build_impl(api);
		}
	};
}