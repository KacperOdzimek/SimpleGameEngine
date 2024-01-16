#include "entity.h"

#include "source/common/common.h"
#include "source/entities/world.h"

entities::entity::entity()
{
	self = std::shared_ptr<entity>{ this };
	common::world->active_scene->entities.push_back(self);
}

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
	for (auto& comp : components)
		delete comp;
	self.reset();
}

entities::entity::~entity()
{
}