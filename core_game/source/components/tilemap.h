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
			physics::collision_preset preset = 0;
			unsigned int layers_stride = 1;
			virtual void pass_transformation(rendering::transformations_buffer_stream& tbi) override;
			void build_colliders();
		public:
			unsigned int get_layers_stride() { return layers_stride; };
			void set_layers_stride(unsigned int new_stride) { layers_stride = new_stride; mark_pipeline_dirty(); };
			virtual void on_attach() override;
			virtual const rendering::render_config& get_render_config() override;
			virtual uint32_t get_instances_amount() override;
			tilemap(
				uint32_t _id,
				std::weak_ptr<assets::tilemap> _tilemap,
				std::weak_ptr<assets::tileset> _tileset,
				physics::collision_preset _preset
			);
			~tilemap();
		};
	}
}