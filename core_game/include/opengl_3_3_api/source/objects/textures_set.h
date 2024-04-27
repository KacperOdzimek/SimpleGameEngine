#pragma once
#include "graphics_abstraction.h"
#include "../common/opengl.h"
#include "texture.h"

namespace graphics_abstraction
{
	namespace implementations
	{
		namespace opengl_3_3_api
		{
			struct textures_set : public graphics_abstraction::textures_set
			{
				friend class opengl_3_3_api;
				friend struct textures_set_builder;

				textures_set() {};

			protected:
				virtual void free()
				{
				}

				//TODO: textures types
				void bind_textures()
				{
					int offset = 0;
					for (auto& txt : textures)
					{
						internal::glActiveTexture(GL_TEXTURE0 + offset);
						internal::glBindTexture(
							GL_TEXTURE_2D, 
							static_cast<texture*>(txt)->id);
						offset++;
					}
				}
			};

			struct textures_set_builder : public graphics_abstraction::textures_set_builder
			{
				virtual object* build_impl(graphics_abstraction::api*& api)
				{
					graphics_abstraction::textures_set* ts = new textures_set{};
					ts->set_selection(textures);
					return ts;
				}
			};
		}
	}
}