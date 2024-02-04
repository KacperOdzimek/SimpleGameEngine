#include "collider.h"
#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/physics/collision_solver.h"

glm::vec2 entities::components::collider::get_world_pos()
{
	return get_owner_location() + entity_offset;
}

void entities::components::collider::on_attach()
{
	common::collision_solver->register_collider(this);
}

entities::components::collider::~collider()
{
	common::collision_solver->unregister_collider(this);
}