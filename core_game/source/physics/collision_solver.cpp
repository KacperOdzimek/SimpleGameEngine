#include "collision_solver.h"
#include "source/entities/entity.h"

#include <list>
#include <unordered_map>
#include <set>
#include <vector>

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
		collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider)
	{
		auto response = get_response_type(trace_preset, collider->preset);
		if (response == collision_response::ignore)
			return {};

		glm::vec2 near = (collider->get_world_pos() - glm::vec2{collider->extend.x, -collider->extend.y} - trace_begin) / trace_dir;
		glm::vec2 far = (collider->get_world_pos() + glm::vec2{collider->extend.x, -collider->extend.y} - trace_begin) / trace_dir;

		if (near.x > far.x) std::swap(near.x, far.x);
		if (near.y > far.y) std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x) return {};

		collision_event e;

		float hit_near = std::max(near.x, near.y);
		float hit_far = std::min(far.x, far.y);

		if (hit_far < 0) return {};

		e.location = trace_begin + hit_near * trace_dir;
		e.distance = hit_near;

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

	collision_event collision_solver::check_if_colliders_collide(
		collider* collider_1, collider* collider_2)
	{
		glm::vec2 ray = collider_2->get_world_pos() - collider_1->get_world_pos();

		auto event = check_if_ray_collide(
			collider_1->preset, collider_1->get_world_pos(), glm::normalize(ray), collider_2
		);

		glm::vec2 bounded_ray = glm::vec2{
			std::min(collider_1->extend.x, ray.x), std::min(collider_1->extend.y, ray.y)
		};

		if (event.distance <= bounded_ray.length())
			return event;
		return {};
	}

	collision_event collision_solver::check_if_collider_collide_on_move(
		entities::components::collider* moved_collider, glm::vec2 target_location, entities::components::collider* other)
	{
		other->extend += moved_collider->extend;
		auto velocity = target_location - moved_collider->get_world_pos();
		auto event = check_if_ray_collide(moved_collider->preset, moved_collider->get_world_pos(), velocity, other);
		other->extend -= moved_collider->extend;
		if (event.distance < 1)
			return event;
		return {};
	}

	sweep_move_event collision_solver::sweep_move(
		entities::components::collider* collider, glm::vec2& end_point)
	{
		std::vector<collision_event> overlap_events;
		collision_event collide_event;
		for (auto& c : impl->all_colliders)
			if (c != collider)
			{
				collision_event e = check_if_collider_collide_on_move(collider, end_point, c);
				if (e.response == collision_response::collide && e.distance < collide_event.distance)
					collide_event = e;
				else if (e.response == collision_response::overlap)
					collide_event = e;
			}

		sweep_move_event sme;
		sme.collide_event = collide_event;

		if (collide_event.other == nullptr)
			sme.overlap_events = std::move(overlap_events);
		else
			for (auto& event : overlap_events)
				if (event.distance < sme.collide_event.distance)
					sme.overlap_events.push_back(event);

		return sme;
	}
}