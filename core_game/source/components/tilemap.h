#pragma once
#include "source/components/mesh.h"
#include "source/assets/tilemap_asset.h"
#include "source/assets/tileset_asset.h"
#include "source/physics/collision.h"

namespace entities
{
	namespace components
	{
		class collider;

		/*
			tilemap
			renders tilemap with given tileset
		*/
		class tilemap : virtual public entities::components::mesh
		{
		protected:
			std::vector<collider*> owned_colliders;
			std::shared_ptr<assets::tilemap> tilemap_asset;
			std::shared_ptr<assets::tileset> tileset_asset;
			rendering::render_config _config;
			physics::collision_preset preset = 2;
			virtual void pass_transformation(rendering::transformations_buffer_iterator& tbi) override;
			void build_colliders();
		public:
			virtual void on_attach() override;
			virtual const rendering::render_config& get_render_config() override;
			virtual uint32_t get_instances_amount() override;
			tilemap(
				uint32_t _id,
				std::weak_ptr<assets::tilemap> _tilemap,
				std::weak_ptr<assets::tileset> _tileset
			);
			~tilemap();
		};
	}
}