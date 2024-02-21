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
			bool visible = true;
			glm::vec2 offset = { 0, 0 };
			glm::vec2 scale = { 1, 1 };
		public:
			void set_visible(bool visiblity) { if (visible != visiblity) { visible = visiblity; mark_pipeline_dirty(); } }
			bool get_visible() { return visible; }

			void set_offset(glm::vec2 new_offset) 
			{ 
				if (offset.x != new_offset.x || offset.y != new_offset.y) 
				{ 
					offset = new_offset;
					mark_pipeline_dirty(); 
				} 
			}
			const glm::vec2& get_offset() { return offset; }

			void set_scale(glm::vec2 new_scale)
			{
				if (scale.x != new_scale.x || scale.y != new_scale.y)
				{
					scale = new_scale;
					mark_pipeline_dirty();
				}
			}
			const glm::vec2& get_scale() { return scale; }

			virtual const rendering::render_config& get_render_config() = 0;
			virtual void on_attach() override;

			mesh(uint32_t _id);
		};
	}
}