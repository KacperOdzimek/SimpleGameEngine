#pragma once
#include "../data/textures_properties.h"

namespace graphics_abstraction
{
	struct texture : public object
	{
		texture(
			texture_type _texture_type,
			texture_internal_format _internal_format,
			unsigned int _width, unsigned int _height,
			bool _using_mipmaps,
			texture_filtering _min_filter, texture_filtering _min_mipmap_filter,
			texture_filtering _mag_filter,
			texture_wraping _x_axis_wraping, texture_wraping _y_axis_wraping,
			texture_wraping _z_axis_wraping
		) : texture_type(_texture_type), internal_format(_internal_format), 
			width(_width), height(_height),
			using_mipmaps(_using_mipmaps),
			min_filter(_min_filter), min_mipmap_filter(_min_mipmap_filter),
			mag_filter(_mag_filter),
			x_axis_wraping(_x_axis_wraping),
			y_axis_wraping(_y_axis_wraping),
			z_axis_wraping(_z_axis_wraping)
		{};

		const texture_type texture_type = texture_type::unspecified;
		const texture_internal_format internal_format = texture_internal_format::unspecified;

		unsigned int width;
		unsigned int height;

		const bool using_mipmaps = true;

		const texture_filtering min_filter = texture_filtering::linear;
		const texture_filtering min_mipmap_filter = texture_filtering::linear;
		const texture_filtering mag_filter = texture_filtering::linear;

		const texture_wraping x_axis_wraping = texture_wraping::repeat;
		const texture_wraping y_axis_wraping = texture_wraping::repeat;
		const texture_wraping z_axis_wraping = texture_wraping::repeat;

	protected:
		virtual void bind(internal::pipeline& pipeline) final
		{
		}

	public:
		virtual void write(
			unsigned int width,
			unsigned int height,
			void* source_texture = nullptr,
			texture_internal_format source_format = texture_internal_format::unspecified,
			input_data_type source_data_type = input_data_type::unspecified
		) = 0;
		virtual void resize(unsigned int width, unsigned int height) = 0;
	};

	struct texture_builder : public builder
	{
		texture_type texture_type = texture_type::unspecified;
		texture_internal_format internal_format = texture_internal_format::unspecified;

		unsigned int width = 0;
		unsigned int height = 0;

		bool generate_mipmaps = true;

		texture_internal_format source_format = texture_internal_format::unspecified;
		input_data_type source_data_type = input_data_type::unspecified;
		void* source_texture = nullptr;

		texture_filtering min_filter		= texture_filtering::linear;
		texture_filtering min_mipmap_filter = texture_filtering::linear;
		texture_filtering mag_filter		= texture_filtering::linear;

		texture_wraping x_axis_wraping = texture_wraping::repeat;
		texture_wraping y_axis_wraping = texture_wraping::repeat;
		texture_wraping z_axis_wraping = texture_wraping::repeat;
	protected:
		virtual object* build_abs(api*& api) final
		{
			if (internal_format == texture_internal_format::unspecified)
				goto report_error;

			if (
				(source_format == texture_internal_format::unspecified || source_data_type == input_data_type::unspecified) 
				&& !(source_texture == nullptr)
			)
				goto report_error;

			switch (texture_type)
			{
			case graphics_abstraction::texture_type::unspecified:
				goto report_error;
			case graphics_abstraction::texture_type::texture_1d:
				if (width == 0)
					goto report_error;
				break;
			case graphics_abstraction::texture_type::texture_1d_array:		//TODO
				break;
			case graphics_abstraction::texture_type::texture_2d:
				if (width == 0 || height == 0)
					goto report_error;
				break;
			case graphics_abstraction::texture_type::texture_2d_array:		//TODO
				break;
			case graphics_abstraction::texture_type::texture_3d:			//TODO
				break;
			case graphics_abstraction::texture_type::texture_cube:			//TODO
				break;
			default:
				break;
			}

			return this->build_impl(api);

		report_error:
			api->report_error(
				{
					error_source::abstraction,
					3,
					{}
				}
			);

			return nullptr;
		}
	};
}