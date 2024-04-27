#pragma once

namespace graphics_abstraction
{
	struct object;
	struct framebuffer;
	struct buffer;
	struct shader;
	struct vertex_layout;
	struct textures_set;

	namespace internal
	{
		struct pipeline
		{
			framebuffer*		framebuffer				= nullptr;
			shader*				shader					= nullptr;
			buffer*				vertex_buffer			= nullptr;
			buffer*				instanced_buffer		= nullptr;
			vertex_layout*		instanced_buffer_layout = nullptr;
			vertex_layout*		vertex_layout			= nullptr;
			buffer*				indices_buffer			= nullptr;
			textures_set*		textures_set 			= nullptr;
		};

		inline bool is_binded(pipeline& pipeline, object* obj)
		{
			return (
				(void*)pipeline.framebuffer				== (void*)obj ||
				(void*)pipeline.shader					== (void*)obj ||
				(void*)pipeline.vertex_buffer			== (void*)obj ||
				(void*)pipeline.vertex_layout			== (void*)obj ||
				(void*)pipeline.instanced_buffer_layout == (void*)obj ||
				(void*)pipeline.indices_buffer			== (void*)obj ||
				(void*)pipeline.instanced_buffer		== (void*)obj ||
				(void*)pipeline.textures_set			== (void*)obj
			);
		}

		inline void unbind(pipeline& pipeline, object* obj)
		{
			if ((void*)pipeline.framebuffer == (void*)obj)
				pipeline.framebuffer = nullptr;

			else if ((void*)pipeline.shader == (void*)obj)
				pipeline.shader = nullptr;

			else if ((void*)pipeline.vertex_buffer == (void*)obj)
				pipeline.vertex_buffer = nullptr;

			else if ((void*)pipeline.vertex_layout == (void*)obj)
				pipeline.vertex_layout = nullptr;

			else if ((void*)pipeline.instanced_buffer_layout == (void*)obj)
				pipeline.instanced_buffer_layout = nullptr;

			else if ((void*)pipeline.indices_buffer == (void*)obj)
				pipeline.indices_buffer = nullptr;

			else if ((void*)pipeline.instanced_buffer == (void*)obj)
				pipeline.instanced_buffer = nullptr;

			else if ((void*)pipeline.textures_set == (void*)obj)
				pipeline.textures_set = nullptr;
		}
	}
}