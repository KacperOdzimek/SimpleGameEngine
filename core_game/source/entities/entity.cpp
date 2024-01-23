#include "entity.h"

#include "source/common/common.h"

#include "source/entities/world.h"

#include "source/physics/collision_solver.h"
#include "source/components/collider.h"

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

const glm::vec2& entities::entity::get_location()
{
	return location;
}

void entities::entity::teleport(glm::vec2 new_location)
{
	int old_chunk_x = floor(location.x / physics::collision_solver::chunk_size);
	int old_chunk_y = floor(location.y / physics::collision_solver::chunk_size);

	int new_chunk_x = floor(location.x / physics::collision_solver::chunk_size);
	int new_chunk_y = floor(location.y / physics::collision_solver::chunk_size);

	location = new_location;
	if (old_chunk_x != new_chunk_x || old_chunk_y != new_chunk_y)
		for (auto& c : components)
		{
			auto ptr = dynamic_cast<components::collider*>(c);
			if (ptr != nullptr)
				ptr->on_moved_to_other_chunk();
		}
}

physics::collision_event entities::entity::sweep(glm::vec2 new_location)
{
	//TODO RUN ON COLLIDE OR OVERLAP
	physics::collision_event closest_event;
	components::collider* collider = nullptr;
	for (auto& c : components)
	{
		auto ptr = dynamic_cast<components::collider*>(c);
		if (ptr != nullptr)
		{
			auto e = common::collision_solver->sweep_move(ptr, new_location);
			if (e.distance < closest_event.distance)
			{
				collider = ptr;
				closest_event = e;
			}			
		}
	}

	if (closest_event.response == physics::collision_response::collide)
	{
		teleport(closest_event.location);
		return closest_event;
	}
	teleport(new_location);
	return {};
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