#include "dynamics.h"

#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/physics/dynamics_manager.h"

using namespace entities;
using namespace components;

dynamics::dynamics(uint32_t _id) : component(_id)
{
	common::dynamics_manager->register_dynamics(this);
}

dynamics::~dynamics()
{
	common::dynamics_manager->unregister_dynamics(this);
}

void dynamics::apply_forces()
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

void dynamics::sweep()
{
	apply_forces();
	owner->sweep(owner->get_location() + velocity * float(common::delta_time));
}