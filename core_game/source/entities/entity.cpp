#include "entity.h"

#include "source/common/common.h"

#include "source/entities/world.h"

#include "source/physics/collision_solver.h"
#include "source/components/collider.h"

#include <set>

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
	location = new_location;
}

physics::collision_event entities::entity::sweep(glm::vec2 new_location)
{
	std::vector<physics::sweep_move_event> events;
	int closest_event_id = -1;
	components::collider* collider = nullptr;
	for (auto& c : components)
	{
		auto ptr = dynamic_cast<components::collider*>(c);
		if (ptr != nullptr)
		{
			events.push_back(common::collision_solver->sweep_move(ptr, new_location));
			if ((closest_event_id == -1 || events.back().collide_event.distance < events.at(closest_event_id).collide_event.distance) 
				&& events.back().collide_event.other != nullptr)
				closest_event_id = events.size() - 1;		
		}
	}

	if (closest_event_id == -1)
	{
		std::set<entities::entity*> call_on_overlap;
		for (auto& sweep : events)
			for (auto& ovr : sweep.overlap_events)
				call_on_overlap.insert(ovr.other->get_owner_weak().lock().get());

		for (auto& e : call_on_overlap)
		{
			//Overlap
		}

		teleport(new_location);
		return {};
	}
	else
	{
		physics::collision_event& collide_event = events.at(closest_event_id).collide_event;

		std::set<entities::entity*> call_on_overlap;
		for (auto& sweep : events)
			for (auto& ovr : sweep.overlap_events)
			{
				if (ovr.distance <= collide_event.distance)
					call_on_overlap.insert(ovr.other->get_owner_weak().lock().get());
			}	

		for (auto& e : call_on_overlap)
		{
			//Overlap
		}

		teleport(collide_event.location);
		//Trigger on collide

		return collide_event;
	}
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