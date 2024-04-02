#pragma once
#include "source/entities/component.h"
#include "include/glm/vec2.hpp"

namespace physics
{
	class dynamics_manager;
}

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
			friend entities::entity;
			friend physics::dynamics_manager;

			glm::vec2 velocity{};
			glm::vec2 frame_force{};
			bool grouned = false;
			void apply_forces();
			void collide_event(glm::vec2 normal);
		public:
			bool use_maximum_velocity = false;
			float maximum_velocity = 0.0f;
			float drag = 0.7f;
			float mass = 1.0f;

			dynamics(uint32_t _id);
			~dynamics();

			const bool& get_grounded() { return grouned; }

			void add_force(glm::vec2 force)
			{
				frame_force += force;
			}

			void sweep();

			const void set_velocity(glm::vec2 vel)
			{
				velocity = vel;
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