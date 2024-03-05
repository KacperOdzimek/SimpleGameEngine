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
		*/
		class dynamics : virtual public component
		{
		private:
			glm::vec2 velocity{};
			glm::vec2 frame_force{};
			void apply_forces();
		public:
			bool use_maximum_velocity = false;
			float maximum_velocity = 0;
			float drag = 0.7;
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