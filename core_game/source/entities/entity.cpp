#include "entity.h"

#include "source/common/common.h"
#include "source/entities/world.h"

void entities::entity::attach_component(component* comp)
{
	components.push_back(comp);
	comp->owner = this;
	comp->on_attach();
}

void entities::entity::kill()
{
	common::world->remove_entity_pointer(this);
	delete this;
}

entities::entity::~entity()
{
	for (auto& comp : components)
		delete comp;
}