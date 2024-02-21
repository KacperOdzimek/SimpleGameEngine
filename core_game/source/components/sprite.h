#pragma once
#include "mesh.h"
#include "collider.h"

namespace entities
{
	namespace components
	{
		class sprite : virtual public mesh, virtual public collider
		{
		private:
			rendering::render_config rc;
			glm::vec2 sprite_extend;
		public:
			sprite(uint32_t _id, std::weak_ptr<assets::texture> _texture, physics::collision_preset preset);
			~sprite();

			int sprite_id = 0;

			virtual void pass_transformation(rendering::transformations_buffer_iterator& tbi) override;
			virtual void on_attach() override;
			virtual const rendering::render_config& get_render_config() override;
		};
	}
}