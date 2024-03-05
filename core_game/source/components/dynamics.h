#pragma once
#include "source/entities/component.h"
#include "include/glm/vec2.hpp"

#include <iostream>

namespace entities
{
	namespace components
	{
		/*
			dynamics
			component that automates forces, for quicker development
			TODO
		*/
		class dynamics : virtual public component
		{
		private:
			glm::vec2 velocity{};
			bool use_maximum_velocity = true;
			float maximum_velocity = 2;
			glm::vec2 frame_force{};
			void apply_forces();
		public:
			float friction_factor = 0.1;
			float mass = 1;

			dynamics(uint32_t _id);
			~dynamics();

			void add_force(glm::vec2 force)
			{
				frame_force += force;
			}

			void sweep();

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