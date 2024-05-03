#include "entity.h"

#include "source/common/common.h"

#include "source/entities/world.h"

#include "source/physics/collision_solver.h"
#include "source/components/collider.h"
#include "source/components/behavior.h"
#include "source/components/mesh.h"
#include "source/components/dynamics.h"

#include "source/behaviors/behaviors_manager.h"
#include "source/behaviors/frame.h"

#include <vector>

entities::entity::entity()
{
	self = std::shared_ptr<entity>{ this };
	common::world->get_persistent_scene()->entities.push_back(self);
}

entities::entity::entity(scene* parent_scene)
{
	self = std::shared_ptr<entity>{ this };
	parent_scene->entities.push_back(self);

	auto f = common::behaviors_manager->get_current_frame();
	if (f->scene_context != common::world->get_persistent_scene())
		location += f->scene_context->world_offset;
}

void entities::entity::attach_component(component* comp)
{
	components.push_back(comp);
	comp->owner = this;
	comp->on_attach();
}

void call_on_collide(std::weak_ptr<entities::entity> entity, std::weak_ptr<entities::entity> other)
{
	for (auto& comp : entity.lock()->get_components())
	{
		auto behavior = dynamic_cast<entities::components::behavior*>(comp);
		if (behavior != nullptr)
			behavior->call_function(behaviors::functions::on_collide, other);
	}

	for (auto& comp : other.lock()->get_components())
	{
		auto behavior = dynamic_cast<entities::components::behavior*>(comp);
		if (behavior != nullptr)
			behavior->call_function(behaviors::functions::on_collide, entity);
	}
}

void call_on_overlap(std::vector<std::weak_ptr<entities::entity>>& overlaping_entities)
{
	if (overlaping_entities.size() == 1) return;

	for (auto& entity : overlaping_entities)
		for (auto& other : overlaping_entities)
		{
			if (other.expired()) continue;
			if (&entity == &other) continue;
			for (auto& comp : entity.lock()->get_components())
			{
				auto bhv = dynamic_cast<entities::components::behavior*>(comp);
				if (bhv == nullptr) continue;
				bhv->call_function(behaviors::functions::on_overlap, other);
				if (entity.expired()) break;
			}
			if (entity.expired()) break;
		}
}

const glm::vec2& entities::entity::get_location()
{
	return location;
}

void entities::entity::teleport(glm::vec2 new_location)
{
	auto f = common::behaviors_manager->get_current_frame();
	if (f->scene_context != common::world->get_persistent_scene())
		new_location += f->scene_context->world_offset;

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
	if (location == new_location) return {};

	std::vector<physics::sweep_move_event*> events;
	int closest_event_id = -1;

	std::vector<components::dynamics*> dynamics_components;

	for (auto& c : components)
	{
		auto c_ptr = dynamic_cast<components::collider*>(c);
		if (c_ptr != nullptr)
		{
			auto current_event = common::collision_solver->sweep_move(c_ptr, new_location);
			
			if (current_event == nullptr) continue;

			events.push_back(current_event);
			
			if (
				events.back()->collide_event != nullptr &&														//If there was a collide
				(closest_event_id == -1 ||																		//And there was no hit before
				events.back()->collide_event->distance < events.at(closest_event_id)->collide_event->distance)	//Or this hit is closer than previous
			)	
				closest_event_id = static_cast<int>(events.size() - 1);
		}
		auto m_ptr = dynamic_cast<components::mesh*>(c);
		if (m_ptr != nullptr)
		{
			m_ptr->mark_pipeline_dirty();
			continue;
		}
		auto d_ptr = dynamic_cast<components::dynamics*>(c);
		if (d_ptr != nullptr)
		{
			dynamics_components.push_back(d_ptr);
			continue;
		}
	}

	std::vector<std::weak_ptr<entities::entity>> overlaping_entities;

	if (closest_event_id == -1)
		for (auto& sweep : events)
			for (auto& ovr : sweep->overlap_events)
				overlaping_entities.push_back(ovr->other->get_owner_weak());
	else
	{
		physics::collision_event* collide_event = events.at(closest_event_id)->collide_event;
		for (auto& sweep : events)
			for (auto& ovr : sweep->overlap_events)
				if (ovr->distance < collide_event->distance)		//Check if overlap is closer than collide event
					overlaping_entities.push_back(ovr->other->get_owner_weak());
	}

	overlaping_entities.push_back(get_weak());
	if (overlaping_entities.size() != 0)
		call_on_overlap(overlaping_entities);

	physics::collision_event result_collide;

	if (closest_event_id == -1)
	{
		location = new_location;
		result_collide = {};
	}
	else
	{
		location += events.at(closest_event_id)->velocity;
		result_collide = *events.at(closest_event_id)->collide_event;

		for (auto& d : dynamics_components)
			d->collide_event(result_collide.normal);

		call_on_collide(get_weak(), result_collide.other->get_owner_weak());
	}

	for (auto& e : events)
		delete e;

	return result_collide;
}

entities::component* entities::entity::get_component(uint32_t id)
{
	for (auto& comp : components)
		if (comp->id == id)
			return comp;
	return nullptr;
}

void entities::entity::kill_component(uint32_t id)
{;
	for (auto itr = components.begin(); itr != components.end(); itr++)
		if ((*itr)->id == id)
		{
			delete (*itr);
			components.erase(itr);
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