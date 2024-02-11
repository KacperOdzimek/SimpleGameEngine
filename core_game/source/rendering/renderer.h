#pragma once
#include "render_config.h"

namespace graphics_abstraction
{
	class api;
}

namespace entities
{
	namespace components
	{
		struct mesh;
		struct camera;
	}
}

namespace rendering
{
	struct pipeline_config;

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

		void create_window();
		void initialize();
		/*
			Main loop
		*/

		bool should_window_close();
		void update_window();
		/*
			Rendering
		*/

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
	};
}