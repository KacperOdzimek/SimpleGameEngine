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
			class opengl_3_3_api;

			struct buffer : public graphics_abstraction::buffer
			{
				internal::GLuint id = 0;

			public:
				buffer(graphics_abstraction::buffer_type bt, uint32_t _size)
				{
					buffer_type = bt;
					size = _size;

					internal::glGenBuffers(1, &id);
					internal::glBindBuffer(internal::buffer_type_to_opengl(bt), id);
					internal::glBufferData(internal::buffer_type_to_opengl(bt), size, NULL, GL_DYNAMIC_DRAW);
					internal::glBindBuffer(internal::buffer_type_to_opengl(bt), 0);
				}

				virtual void* open_data_stream()
				{
					internal::glBindBuffer(internal::buffer_type_to_opengl(buffer_type), id);
					return internal::glMapBuffer(internal::buffer_type_to_opengl(buffer_type), GL_WRITE_ONLY);
				}

				virtual void close_data_stream()
				{
					internal::glUnmapBuffer(internal::buffer_type_to_opengl(buffer_type));
					internal::glBindBuffer(internal::buffer_type_to_opengl(buffer_type), NULL);
				}

				virtual void reallocate(uint32_t new_buffer_size)
				{
					size = new_buffer_size;
					internal::glBindBuffer(internal::buffer_type_to_opengl(buffer_type), id);
					internal::glBufferData(internal::buffer_type_to_opengl(buffer_type), new_buffer_size, NULL, GL_DYNAMIC_DRAW);
					internal::glBindBuffer(internal::buffer_type_to_opengl(buffer_type), 0);
				}

			protected:
				virtual void free()
				{
					internal::glDeleteBuffers(1, &id);
				}
			};

			struct buffer_builder : public graphics_abstraction::buffer_builder
			{
				virtual object* build_impl(graphics_abstraction::api*& api)
				{
					return new buffer{buffer_type, size};
				}
			};
		}
	}
}