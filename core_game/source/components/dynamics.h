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
			WIP
		*/
		class dynamics : virtual public component
		{
		private:
			glm::vec2 velocity{};
			bool use_maximum_velocity = true;
			float maximum_velocity = 2;
			glm::vec2 frame_force{};
		public:
			float friction_factor = 0.1;
			float mass = 1;

			dynamics(uint32_t _id) : component(_id) {};

			void add_force(glm::vec2 force)
			{
				frame_force += force;
			}

			void apply_forces()
			{
				//Constrain Velocity
				if (velocity.length() > maximum_velocity && use_maximum_velocity)
					velocity = glm::normalize(velocity) * maximum_velocity;

				//Accelerating
				if (!(frame_force.x == 0 && frame_force.y == 0))
				{
					velocity += (frame_force / mass) * float(common::delta_time);
					frame_force = { 0, 0 };
				}
				//Breaking
				else
				{
					velocity -= velocity * friction_factor;
				}
			}

			void sweep()
			{
				apply_forces();
				owner->sweep(owner->get_location() + velocity * float(common::delta_time));
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