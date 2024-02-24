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
			virtual void pass_transformation(rendering::transformations_buffer_iterator& tbi) override;
		public:
			virtual const rendering::render_config& get_render_config() override;
			void set_render_config(const rendering::render_config& rc);
			virtual uint32_t get_instances_amount() override;

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