#pragma once
#include "sprite.h"
#include "source/assets/flipbook_asset.h"

namespace entities
{
	namespace components
	{
		class flipbook : virtual public sprite
		{
			uint32_t current_flipbook_animation;
		public:
			float playback_position = 0;
			bool looping = true;

			flipbook(uint32_t _id, std::weak_ptr<assets::flipbook> _flipbook, 
				physics::collision_preset preset, uint32_t _flipbook_animation);
			~flipbook();

			const uint32_t& get_animation() { return current_flipbook_animation; }
			void set_animation(uint32_t new_animation);
			virtual void pass_transformation(rendering::transformations_buffer_iterator& tbi);

			void move_playback(float time);
		};
	}
}