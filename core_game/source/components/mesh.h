#pragma once
#include "source/entities/component.h"

#include "source/assets/mesh_asset.h"
#include "source/assets/shader_asset.h"

#include "source/rendering/transformations_buffer_iterator.h"
#include "source/rendering/render_config.h"

#include "glm/vec2.hpp"

namespace rendering
{
	class renderer;
}

namespace entities
{
	namespace components
	{
		/*
			mesh
			component that links the rendering subsystem with game logic
			mesh adds rendered geometry to the entity
		*/
		class mesh : virtual public entities::component
		{
			friend entities::entity;
			friend rendering::renderer;
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
				determine if mesh should be rendered
				change does not cause tranformation buffer update
			*/
			bool visible = true;

			/*
				entity-relative location
			*/
			glm::vec2 offset = { 0, 0 };

			/*
				mesh scale
			*/
			glm::vec2 scale = { 1, 1 };

			const rendering::render_config& get_render_config();
			void set_render_config(const rendering::render_config& rc);

			mesh(
				uint32_t _id,
				std::weak_ptr<assets::mesh> _mesh,
				std::weak_ptr<assets::shader> _material,
				std::vector<std::weak_ptr<assets::texture>> _textures
			);
			mesh(
				uint32_t _id,
				rendering::render_config __config
			);
			virtual void on_attach();
			~mesh();
		};
	}
}