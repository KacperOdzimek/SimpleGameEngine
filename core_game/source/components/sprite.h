#pragma once
#include "mesh.h"
#include "collider.h"

namespace entities
{
	namespace components
	{
		class sprite : public mesh, public collider
		{
		private:
			std::shared_ptr<assets::texture> texture;
		public:
			sprite(uint32_t _id, std::weak_ptr<assets::texture> _texture, physics::collision_preset preset);
			virtual void on_attach() override;
			~sprite();
		};
	}
}