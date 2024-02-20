#pragma once
#include "source/components/mesh.h"
#include "glm/vec2.hpp"

namespace entities
{
	namespace components
	{
		/*
			static_mesh
			simple mesh implementation
		*/
		class static_mesh : virtual public entities::components::mesh
		{
		protected:
			/*
				all data required to draw the mesh
			*/
			rendering::render_config _config;

			/*
				pass_transformation
				passes mesh transformation to the buffer on the GPU
			*/
			void pass_transformation(rendering::transformations_buffer_iterator& tbi);

			/*
				mark_pipeline_dirty
				force renderer to reload transformations for the pipeline used by this instace of the mesh
			*/
			void mark_pipeline_dirty();
		public:

			/*
				entity-relative location
			*/
			glm::vec2 offset = { 0, 0 };

			/*
				mesh scale
			*/
			glm::vec2 scale = { 1, 1 };

			virtual const rendering::render_config& get_render_config() override;
			void set_render_config(const rendering::render_config& rc);

			static_mesh(
				uint32_t _id,
				std::weak_ptr<assets::mesh> _mesh,
				std::weak_ptr<assets::shader> _material,
				std::vector<std::weak_ptr<assets::texture>> _textures
			);
			static_mesh(
				uint32_t _id,
				rendering::render_config __config
			);
			~static_mesh();
		};
	}
}