#include "collider.h"
#include "source/entities/entity.h"

glm::vec2 entities::components::collider::get_world_pos()
{
	return owner->position + entity_offset;
}