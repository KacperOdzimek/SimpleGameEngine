#pragma once
#include "render_config.h"
#include <functional>

namespace graphics_abstraction
{
	class api;
}

namespace entities
{
	namespace components
	{
		class mesh;
		class camera;
	}
}

namespace rendering
{
	class renderer
	{
		struct implementation;
		implementation* impl;
		friend implementation;
		void create_main_renderbuffer();
	public:
		renderer();
		~renderer();
		/*
			Initialization
		*/

		void initialize();
		/*
			Rendering
		*/

		void load_config();

		void mark_pipeline_dirty(const render_config& pipeline);
		void register_mesh_component(entities::components::mesh* mesh);
		void unregister_mesh_component(entities::components::mesh* mesh);
		void update_transformations();
		void render();

		void set_active_camera(entities::components::camera* camera);
		entities::components::camera* get_active_camera();

		/*
			Data
		*/
		graphics_abstraction::api* get_api();
		std::function<void(void)> get_resize_function();
		uint32_t get_new_uid();
	};
}