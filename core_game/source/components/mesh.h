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
		class mesh : public entities::component
		{
			friend entities::entity;
			friend rendering::renderer;
		protected:
			rendering::render_config _config;
			void pass_transformation(rendering::transformations_buffer_iterator& tbi);
			//makes renderer reload transformations
			void mark_pipeline_dirty();
		public:
			bool visible = true;
			glm::vec2 offset = { 0, 0 };
			glm::vec2 scale = { 1, 1 };
			mesh(
				uint32_t _id,
				std::weak_ptr<assets::mesh> _mesh,
				std::weak_ptr<assets::shader> _material,
				std::vector<std::weak_ptr<assets::texture>> _textures
			);
			virtual void on_attach();
			~mesh();
		};
	}
}