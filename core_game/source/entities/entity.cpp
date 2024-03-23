#include "entity.h"

#include "source/common/common.h"

#include "source/entities/world.h"

#include "source/physics/collision_solver.h"
#include "source/components/collider.h"
#include "source/components/behavior.h"
#include "source/components/mesh.h"

#include <set>

entities::entity::entity()
{
	self = std::shared_ptr<entity>{ this };
	common::world->get_dynamic_scene()->entities.push_back(self);
}

entities::entity::entity(scene* parent_scene)
{
	self = std::shared_ptr<entity>{ this };
	parent_scene->entities.push_back(self);
}

void entities::entity::attach_component(component* comp)
{
	components.push_back(comp);
	comp->owner = this;
	comp->on_attach();
}

void entities::entity::call_on_collide(std::weak_ptr<entities::entity> other)
{
	for (auto& comp : components)
	{
		auto behavior = dynamic_cast<components::behavior*>(comp);
		if (behavior != nullptr)
			behavior->call_function(behaviors::functions::on_collide, other);
	}
}

void entities::entity::call_on_overlap(std::set<std::shared_ptr<entities::entity>*>& overlaping_entities)
{
	for (auto& comp : components)
	{
		auto behavior = dynamic_cast<components::behavior*>(comp);
		if (behavior != nullptr)
			for (auto& e : overlaping_entities)
				if (e->get() != self.get())
					behavior->call_function(behaviors::functions::on_overlap, *e);
	}
}

void entities::entity::call_event(const char* name)
{
	std::string event_name = "event_";
	event_name += name;
	for (auto& comp : components)
	{
		auto behavior = dynamic_cast<components::behavior*>(comp);
		if (behavior != nullptr)
			behavior->call_custom_function(event_name);
	}
}

const glm::vec2& entities::entity::get_location()
{
	return location;
}

void entities::entity::teleport(glm::vec2 new_location)
{
	location = new_location;
	for (auto& c : components)
	{
		auto m_ptr = dynamic_cast<components::mesh*>(c);
		if (m_ptr != nullptr)
		{
			m_ptr->mark_pipeline_dirty();
			continue;
		}
	};
}

physics::collision_event entities::entity::sweep(glm::vec2 new_location)
{
	if (new_location.x == location.x && new_location.y == location.y)
		return {};

	std::vector<physics::sweep_move_event> events;
	int closest_event_id = -1;
	components::collider* collider = nullptr;

	for (auto& c : components)
	{
		auto c_ptr = dynamic_cast<components::collider*>(c);
		if (c_ptr != nullptr)
		{
			events.push_back(common::collision_solver->sweep_move(c_ptr, new_location));
			if (
				(
					closest_event_id == -1 
					|| events.back().collide_event.distance < events.at(closest_event_id).collide_event.distance
				) 
				&& events.back().collide_event.other != nullptr)
				closest_event_id = static_cast<int>(events.size() - 1);
		}
		auto m_ptr = dynamic_cast<components::mesh*>(c);
		if (m_ptr != nullptr)
		{
			m_ptr->mark_pipeline_dirty();
		}
	}

	if (closest_event_id == -1)
	{
		std::set<std::shared_ptr<entities::entity>*> overlaping_entities;

		for (auto& sweep : events)
			for (auto& ovr : sweep.overlap_events)
				overlaping_entities.insert(&(ovr.other->get_owner_weak().lock().get()->self));

		call_on_overlap(overlaping_entities);
		overlaping_entities.insert(&self);

		for (auto& e : overlaping_entities)
			if (e->get() != this)
				(*e)->call_on_overlap(overlaping_entities);

		location = new_location;
		return {};
	}
	else
	{
		physics::collision_event& collide_event = events.at(closest_event_id).collide_event;

		std::set<std::shared_ptr<entities::entity>*> overlaping_entities;

		for (auto& sweep : events)
			for (auto& ovr : sweep.overlap_events)
				if (ovr.distance < collide_event.distance)		//Check if overlap is closer than collide event
					overlaping_entities.insert(&(ovr.other->get_owner_weak().lock().get()->self));

		call_on_overlap(overlaping_entities);
		overlaping_entities.insert(&self);

		for (auto& e : overlaping_entities)
			if (e->get() != this)
				(*e)->call_on_overlap(overlaping_entities);

		location += events.at(closest_event_id).velocity;

		call_on_collide(collide_event.other->get_owner_weak());
		collide_event.other->get_owner_weak().lock()->call_on_collide(this->self);

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

std::weak_ptr<entities::entity> entities::entity::get_weak()
{
	return self;
}

entities::entity::~entity()
{
}