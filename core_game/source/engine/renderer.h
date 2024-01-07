#pragma once
#include "source/rendering/renderer_data_types.h"

namespace graphics_abstraction
{
	class api;
}

namespace entities
{
	class geometry_component;
}

namespace rendering
{
	struct pipeline_config;

	class renderer
	{
		struct implementation;
		implementation* impl;
	public:
		renderer();
		~renderer();
		/*
			Initialization
		*/

		void create_window();
		void create_api_instance();
		/*
			Main loop
		*/

		bool should_window_close();
		void update_window();
		/*
			Rendering
		*/

		void register_geometry_component(entities::geometry_component* comp, pipeline_config target_pipeline);
		void unregister_geometry_component(entities::geometry_component* comp, pipeline_config target_pipeline);
		void collect_geometry_data();
		void render();

		/*
			Data
		*/
		graphics_abstraction::api* get_api();
	};
}