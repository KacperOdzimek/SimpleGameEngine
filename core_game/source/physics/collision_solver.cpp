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

	collision_event collision_solver::check_if_ray_collide(
		collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir)
	{
		for (auto& c : impl->all_colliders)
		{
			auto event = check_if_ray_collide(trace_preset, trace_begin, trace_dir, c);
			if (event.response == collision_response::collide)
				return event;
		}

		return {};
	}

	collision_event collision_solver::check_if_ray_collide(
		collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider)
	{
		auto response = get_response_type(trace_preset, collider->preset);
		if (response == collision_response::ignore)
			return {};

		collider->extend /= 4;

		glm::vec2 near = (collider->get_world_pos() - glm::vec2{collider->extend.x, -collider->extend.y} - trace_begin) / trace_dir;
		glm::vec2 far = (collider->get_world_pos() + glm::vec2{collider->extend.x, -collider->extend.y} - trace_begin) / trace_dir;

		collider->extend *= 4;

		if (near.x > far.x) std::swap(near.x, far.x);
		if (near.y > far.y) std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x) return {};

		float hit_near = std::max(near.x, near.y);
		float hit_far = std::min(far.x, far.y);

		if (hit_far < 0) return {};
		if (hit_near != hit_near) hit_near = hit_far;

		collision_event e;

		e.location = trace_begin + hit_near * trace_dir;
		e.distance = glm::distance(trace_begin, e.location);

		if (near.x > near.y)
			if (trace_dir.x < 0)
				e.normal = { 1, 0 };
			else
				e.normal = { -1, 0 };
		else if (near.x < near.y)
			if (trace_dir.y < 0)
				e.normal = { 0, 1 };
			else
				e.normal = { 0, -1 };

		e.other = collider;
		e.response = response;

		return e;
	}

	collision_event collision_solver::check_if_collider_collide_on_move(
		entities::components::collider* moved_collider, const glm::vec2& velocity, entities::components::collider* other)
	{
		if (moved_collider->get_layer() != other->get_layer())
			return {};

		if (velocity.x == 0 && velocity.y == 0)
			return {};

		other->extend += moved_collider->extend;
		auto event = check_if_ray_collide(moved_collider->preset, moved_collider->get_world_pos(), velocity, other);
		other->extend -= moved_collider->extend;

		if (event.distance < glm::length(velocity))
			return event;
		return {};
	}

	sweep_move_event collision_solver::sweep_move(
		entities::components::collider* collider, const glm::vec2& end_point)
	{
		if (collider->preset == 0)
			return {};

		glm::vec2 velocity = end_point - collider->get_world_pos();

		if (velocity.x == 0 && velocity.y == 0)
			return {};

		collision_event collide_event;
		std::vector<collision_event> overlap_events;

		std::sort(impl->all_colliders.begin(), impl->all_colliders.end(), [&](
			entities::components::collider* a, 
			entities::components::collider* b)
			{
				return
					glm::length(a->get_world_pos() - collider->get_world_pos()) <
					glm::length(b->get_world_pos() - collider->get_world_pos());
			});

		for (auto& c : impl->all_colliders)
			if (c != collider)
			{
				collision_event e = check_if_collider_collide_on_move(collider, velocity, c);
				if (e.response == collision_response::collide)
				{
					if (e.response == collision_response::collide && e.distance < collide_event.distance)
						collide_event = e;
					velocity *= (glm::vec2(1, 1) - glm::vec2(std::abs(e.normal.x), std::abs(e.normal.y)));
				}
				else if (e.response == collision_response::overlap)
					overlap_events.push_back(std::move(e));
			}

		sweep_move_event sme;
		sme.velocity = std::move(velocity);
		sme.collide_event = std::move(collide_event);

		if (collide_event.other == nullptr)
			sme.overlap_events = std::move(overlap_events);
		else
			for (auto& event : overlap_events)
				if (event.distance < sme.collide_event.distance)
					sme.overlap_events.push_back(std::move(event));

		return sme;
	}
}