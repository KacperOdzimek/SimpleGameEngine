#include "collision_solver.h"
#include "source/entities/entity.h"

#include "include/glm/glm.hpp"

#include <vector>
#include <algorithm>

using collider = entities::components::collider;

namespace physics
{
	struct collision_solver::implementation
	{
		std::vector<collider*> all_colliders;
	};

	collision_solver::collision_solver() :
		impl(new implementation) {};

	collision_solver::~collision_solver()
	{
		delete impl;
	}

	void collision_solver::register_collider(collider* c)
	{
		impl->all_colliders.push_back(c);
	}

	void collision_solver::unregister_collider(entities::components::collider* c)
	{
#define target impl->all_colliders
		target.erase(std::remove(target.begin(), target.end(), c), target.end());
#undef target
	}

	collision_event* collision_solver::check_if_ray_collide(
		collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir)
	{
		for (auto& c : impl->all_colliders)
		{
			auto event = check_if_ray_collide(trace_preset, trace_begin, trace_dir, c);

			if (event == nullptr)
				continue;

			if (event->response == collision_response::collide)
				return event;

			delete event;
		}

		return nullptr;
	}

	collision_event* collision_solver::check_if_ray_collide(
		collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider)
	{
		glm::vec2 near = (collider->get_world_pos() - (glm::vec2{collider->extend.x, -collider->extend.y} / 4.0f) - trace_begin) / trace_dir;
		glm::vec2 far = (collider->get_world_pos() + (glm::vec2{collider->extend.x, -collider->extend.y} / 4.0f) - trace_begin) / trace_dir;

		if (near.x > far.x) std::swap(near.x, far.x);
		if (near.y > far.y) std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x) return nullptr;

		float hit_near = std::max(near.x, near.y);
		float hit_far = std::min(far.x, far.y);

		if (hit_far < 0) return nullptr;
		if (hit_near != hit_near) hit_near = hit_far;

		collision_event* e = new collision_event;

		e->location = trace_begin + hit_near * trace_dir;
		e->distance = glm::distance(trace_begin, e->location);

		if (near.x > near.y)
			if (trace_dir.x < 0)
				e->normal = { 1, 0 };
			else
				e->normal = { -1, 0 };
		else if (near.x < near.y)
			if (trace_dir.y < 0)
				e->normal = { 0, 1 };
			else
				e->normal = { 0, -1 };

		e->other = collider;
		e->response = get_response_type(trace_preset, collider->preset);

		return e;
	}

	collision_event* collision_solver::check_if_collider_collide_on_move(
		entities::components::collider* moved_collider, const glm::vec2& velocity, entities::components::collider* other)
	{
		auto response = get_response_type(moved_collider->preset, other->preset);

		if (
			response == collision_response::ignore ||
			moved_collider->get_layer() != other->get_layer() ||
			velocity.x == 0 && velocity.y == 0		
		)
			return nullptr;

		other->extend += moved_collider->extend;
		auto e = check_if_ray_collide(moved_collider->preset, moved_collider->get_world_pos(), velocity, other);
		other->extend -= moved_collider->extend;

		if (e == nullptr)
			return nullptr;
		else if (e->distance < glm::length(velocity))
			return e;
		delete e;
		return nullptr;
	}

	sweep_move_event* collision_solver::sweep_move(
		entities::components::collider* col, const glm::vec2& end_point)
	{
		glm::vec2 velocity = end_point - col->get_world_pos();

		if (col->preset == 0 || velocity.x == 0 && velocity.y == 0)
			return nullptr;

		collision_event* collide_event = nullptr;
		std::vector<collision_event*> overlap_events;

		std::vector<collider*> potential_collisions;

		for (auto& c : impl->all_colliders)
			if (glm::distance(c->get_world_pos(), col->get_world_pos()) - glm::length(c->extend + col->extend) / 2.0f <= glm::length(velocity))
				potential_collisions.push_back(c);

		std::sort(potential_collisions.begin(), potential_collisions.end(), [&](
			entities::components::collider* a, 
			entities::components::collider* b)
			{
				return
					glm::length(a->get_world_pos() - col->get_world_pos()) <
					glm::length(b->get_world_pos() - col->get_world_pos());
			});

		for (auto& c : potential_collisions)
		{
			if (c == col) continue;

			if (glm::distance(c->get_world_pos(), col->get_world_pos()) > glm::length(c->extend + col->extend))
				continue;

			collision_event* e = check_if_collider_collide_on_move(col, velocity, c);
			if (e == nullptr)
				continue;
			else if (e->response == collision_response::collide)
			{
				velocity *= (glm::vec2(1, 1) - glm::vec2(std::abs(e->normal.x), std::abs(e->normal.y)));
				if (collide_event == nullptr || e->distance < collide_event->distance)
					collide_event = e;
				else
					delete e;
			}
			else if (e->response == collision_response::overlap)
				overlap_events.push_back(std::move(e));
		}

		sweep_move_event* sme = new sweep_move_event;
		sme->velocity = std::move(velocity);
		sme->collide_event = std::move(collide_event);

		if (sme->collide_event == nullptr)
			sme->overlap_events = std::move(overlap_events);
		else
			for (auto& event : overlap_events)
				if (event->distance < sme->collide_event->distance)
					sme->overlap_events.push_back(std::move(event));

		return sme;
	}
}