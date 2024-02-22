#pragma once
#include "source/entities/component.h"
#include "source/common/common.h"
#include "include/glm/vec2.hpp"

#include <iostream>

namespace entities
{
	namespace components
	{
		/*
			dynamics
			component that automates forces, for quicker development
		*/
		class dynamics : virtual public component
		{
		private:
			glm::vec2 velocity;
			glm::vec2 frame_acceleration;
		public:
			float friction_factor = 0.1;
			float mass = 1;

			dynamics(uint32_t _id) : component(_id) {};

			void add_force(glm::vec2 force)
			{
				frame_acceleration += force / mass;
			}

			void apply_forces()
			{
				velocity += frame_acceleration * float(common::delta_time);
				frame_acceleration = {0, 0};
				velocity -= velocity * friction_factor;
			}

			const glm::vec2& get_velocity() 
			{ 
				return velocity; 
			};
			
			virtual void on_attach() override
			{
			}
		};
	}
}