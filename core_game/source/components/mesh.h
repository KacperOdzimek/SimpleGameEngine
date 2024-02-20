#pragma once
#include "source/entities/component.h"

#include "source/assets/mesh_asset.h"
#include "source/assets/shader_asset.h"

#include "source/rendering/transformations_buffer_iterator.h"
#include "source/rendering/render_config.h"

#include <cstdint>

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
			virtual void pass_transformation(rendering::transformations_buffer_iterator& tbi) = 0;
			void mark_pipeline_dirty();
		public:
			bool visible = true;

			virtual const rendering::render_config& get_render_config() = 0;
			virtual void on_attach() override;

			mesh(uint32_t _id);
		};
	}
}