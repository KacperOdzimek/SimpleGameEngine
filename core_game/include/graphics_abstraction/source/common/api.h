#pragma once
#include "../objects/object.h"
#include "../data/all_types.h"
#include "../private/pipeline.h"

namespace graphics_abstraction
{
	struct framebuffer_builder;
	struct buffer_builder;
	struct shader_builder;
	struct vertex_layout_builder;
	struct texture_builder;
	struct textures_set_builder;

	class api
	{
	friend struct object;
	friend struct builder;

	protected:
		internal::pipeline current_pipeline;
		internal::pipeline modified_pipeline;

		//If dirty, api won't perform any drawing
		bool dirty = true;

		virtual bool apply_bindings_impl() = 0;

	public:
		/*
			creating resources
		*/

		virtual object* build(builder* builder, bool destroy_builder = true) final
		{
			auto obj = builder->build(this);
			if (destroy_builder)
				delete builder;
			return obj;
		}

		virtual bool free(object* obj) final
		{
			if (internal::is_binded(current_pipeline, obj))
				dirty = true;
			internal::unbind(modified_pipeline, obj);
			obj->free();
			delete obj;
			return true;
		}

		/*
			error handling
		*/

	protected:
		std::vector<error> errors_queue;

	public:
		void report_error(error _error)
		{
			errors_queue.push_back(_error);
		}

	protected:
		virtual void get_errors_impl() = 0;
	public:
		const std::vector<error>& get_errors()
		{
			get_errors_impl();
			return errors_queue;
		}

		void clear_errors()
		{
			errors_queue.resize(0);
			errors_queue.shrink_to_fit();
		}

		virtual framebuffer_builder* create_framebuffer_builder() = 0;
		virtual buffer_builder*	create_buffer_builder() = 0;
		virtual shader_builder*	create_shader_builder() = 0;
		virtual vertex_layout_builder* create_vertex_layout_builder() = 0;
		virtual texture_builder* create_texture_builder() = 0;
		virtual textures_set_builder* create_textures_set_builder() = 0;

		/*
			drawing
		*/
		virtual bool bind(object* obj) final
		{
			if (obj == nullptr)
			{
				errors_queue.push_back({ error_source::abstraction, 6, {} });
				return false;
			}
			obj->bind(modified_pipeline);
			dirty = true;
			return true;
		}

		virtual bool bind(buffer* instanced_buffer, vertex_layout* layout) final
		{
			if (instanced_buffer == nullptr || layout == nullptr)
			{
				errors_queue.push_back({ error_source::abstraction, 6, {} });
				return false;
			}
			modified_pipeline.instanced_buffer = instanced_buffer;
			modified_pipeline.instanced_buffer_layout = layout;
			dirty = true;
			return true;
		}

		virtual bool apply_bindings() final
		{
			current_pipeline = modified_pipeline;
			dirty = !apply_bindings_impl();
			return !dirty;
		};
	protected:		
		virtual bool draw_impl(draw_args& draw_args) = 0;
	public:
		virtual bool draw(draw_args draw_args) final
		{
			if (current_pipeline.indices_buffer == nullptr
				&& (
					draw_args.draw_type == draw_types::instanced_indexed
					|| 
					draw_args.draw_type == draw_types::indexed
				))
			{
				report_error(
					{
						error_source::abstraction, 7, {}
					}
				);

				return false;
			}		

			if (!dirty)
				return draw_impl(draw_args);
			return false;
		}

		virtual bool set_screen_size(int width, int height) = 0;
		
		virtual framebuffer* get_default_framebuffer() = 0;
		virtual bool set_enabled(functionalities functionality, bool enabled) = 0;
	};
};