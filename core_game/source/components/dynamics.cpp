#include "dynamics.h"

#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/physics/dynamics_manager.h"

#include "include/glm/glm.hpp"

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
	if (glm::length(velocity) > maximum_velocity && use_maximum_velocity)
		velocity = glm::normalize(velocity) * maximum_velocity;

	velocity -= velocity * float(drag * common::delta_time);

	if (glm::length(frame_force) == 0)
	{
		if (std::abs(velocity.x) < 0.3f)
			velocity.x = 0;

		if (std::abs(velocity.y) < 0.3f)
			velocity.y = 0;
	}

	velocity += (frame_force / mass) * float(common::delta_time);
	frame_force = { 0, 0 };
}

void dynamics::sweep()
{
	apply_forces();
	owner->sweep(owner->get_location() + velocity * float(common::delta_time));
}