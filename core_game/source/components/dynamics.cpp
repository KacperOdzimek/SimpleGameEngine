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

	if (common::top_down)
		velocity -= velocity * float(drag * common::delta_time) * common::physics_time_dilation_mod;
	else
		velocity.x -= velocity.x * float(drag * common::delta_time) * common::physics_time_dilation_mod;

	if (glm::length(frame_force) == 0)
	{
		if (std::abs(velocity.x) < 0.3f)
			velocity.x = 0;

		if (std::abs(velocity.y) < 0.3f)
			velocity.y = 0;
	}

	velocity += (frame_force / mass) * float(common::delta_time) * common::physics_time_dilation_mod;
	frame_force = { 0, 0 };
}

void dynamics::collide_event(glm::vec2 normal)
{
	if (velocity.x > 0 && normal.x < 0) velocity.x = 0;
	else if (velocity.x < 0 && normal.x > 0) velocity.x = 0;

	if (velocity.y > 0 && normal.y < 0) velocity.y = 0;
	else if (velocity.y < 0 && normal.y > 0)
	{
		velocity.y = 0;
		grouned = true;
	}
}

void dynamics::sweep()
{
	apply_forces();
	owner->sweep(owner->get_location() + velocity * float(common::delta_time) * common::physics_time_dilation_mod);
}