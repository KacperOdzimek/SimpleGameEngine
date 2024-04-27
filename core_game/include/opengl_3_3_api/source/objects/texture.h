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
			struct texture : public graphics_abstraction::texture
			{
				internal::GLuint id;

			public:
				virtual void write(
					unsigned int _width,
					unsigned int _height,
					void* source_texture = nullptr,
					texture_internal_format source_format = texture_internal_format::unspecified,
					input_data_type source_data_type = input_data_type::unspecified
				)
				{
					width = _width;
					height = _height;

					switch (texture_type)
					{
					case texture_type::texture_1d:
						internal::glTexImage1D(
							internal::texture_type_type_to_opengl(texture_type),
							0,
							internal::texture_internal_format_to_opengl(internal_format),
							width, 0,
							internal::texture_internal_format_to_opengl(source_format),
							internal::input_data_type_to_opengl(source_data_type),
							source_texture
						);
						break;
					case texture_type::texture_1d_array:					//TODO
						break;
					case texture_type::texture_2d:
						internal::glTexImage2D(
							internal::texture_type_type_to_opengl(texture_type),
							0,
							internal::texture_internal_format_to_opengl(internal_format),
							width, height, 0,
							internal::texture_internal_format_to_opengl(source_format),
							internal::input_data_type_to_opengl(source_data_type),
							source_texture
						);
						break;
					case texture_type::texture_2d_array:					//TODO
						break;
					case texture_type::texture_3d:							//TODO
						break;
					case texture_type::texture_cube:						//TODO
						break;
					case texture_type::renderbuffer:
						internal::glGenRenderbuffers(1, &id);
						internal::glBindRenderbuffer(GL_RENDERBUFFER, id);
						internal::glRenderbufferStorage(GL_RENDERBUFFER,
							internal::texture_internal_format_to_opengl(internal_format),
							width, height);
						break;
					}
				};

				virtual void resize(unsigned int width, unsigned int height)
				{
					write(width, height, nullptr, internal_format, input_data_type::unsigned_byte);
				}

				texture(const texture_builder& tb) :
					graphics_abstraction::texture(tb.texture_type,tb.internal_format, tb.width, tb.height,
						tb.generate_mipmaps, tb.min_filter, tb.min_mipmap_filter, tb.mag_filter, tb.x_axis_wraping,
						tb.y_axis_wraping, tb.z_axis_wraping)
				{
					int txt_type = internal::texture_type_type_to_opengl(tb.texture_type);

					if (texture_type == texture_type::renderbuffer)
					{
						internal::glGenRenderbuffers(1, &id);
						internal::glBindRenderbuffer(txt_type, id);
						internal::glRenderbufferStorage(txt_type,
							internal::texture_internal_format_to_opengl(internal_format),
							width, height);
					}
					else
					{
						internal::glGenTextures(1, &id);
						internal::glBindTexture(txt_type, id);

						internal::glTexParameteri(txt_type, GL_TEXTURE_WRAP_S, internal::texture_wraping_type_to_opengl(tb.x_axis_wraping));
						internal::glTexParameteri(txt_type, GL_TEXTURE_WRAP_T, internal::texture_wraping_type_to_opengl(tb.y_axis_wraping));
						internal::glTexParameteri(txt_type, GL_TEXTURE_WRAP_R, internal::texture_wraping_type_to_opengl(tb.z_axis_wraping));

						internal::glTexParameteri(txt_type, GL_TEXTURE_MIN_FILTER,
							internal::texture_filtering_type_to_opengl(tb.generate_mipmaps, tb.min_filter, tb.min_mipmap_filter));
						internal::glTexParameteri(txt_type, GL_TEXTURE_MAG_FILTER,
							internal::texture_filtering_type_to_opengl(0, tb.mag_filter, tb.mag_filter));

						write(width, height, tb.source_texture, tb.source_format, tb.source_data_type);

						if (tb.generate_mipmaps)
							internal::glGenerateMipmap(internal::texture_type_type_to_opengl(texture_type));
					}
				}

			protected:
				virtual void free()
				{
					internal::glDeleteTextures(1, &id);
				}
			};

			struct texture_builder : public graphics_abstraction::texture_builder
			{
				virtual object* build_impl(graphics_abstraction::api*& api)
				{
					return new texture{*this};
				}
			};
		}
	}
}