#include "entity.h"

#include "source/common/common.h"
#include "source/entities/world.h"

void entities::entity::attach_component(component* comp)
{
	components.push_back(comp);
	comp->owner = this;
	comp->on_attach();
}

entities::component* entities::entity::get_component(uint32_t id)
{
	for (auto& comp : components)
		if (comp->id == id)
			return comp;
	return nullptr;
}

void entities::entity::kill_component(uint32_t id)
{
	for (int i = 0; i < components.size(); i++)
		if (components[i]->id == id)
		{
			delete components[i];
			components.erase(components.begin() + i);
		}
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