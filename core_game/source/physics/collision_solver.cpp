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
		collision_present trace_present, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider)
	{
		auto response = get_response_type(trace_present, collider->present);
		if (response == collision_response::ignore)
			return {};

		glm::vec2 near = (collider->get_world_pos() - glm::vec2{collider->extend.x, -collider->extend.y} - trace_begin) / trace_dir;
		glm::vec2 far =  (collider->get_world_pos() + glm::vec2{collider->extend.x, -collider->extend.y} - trace_begin) / trace_dir;

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
			collider_1->present, collider_1->get_world_pos(), glm::normalize(ray), collider_2
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
		auto event = check_if_ray_collide(moved_collider->present, moved_collider->get_world_pos(), velocity, other);
		other->extend -= moved_collider->extend;
		if (event.distance < 1)	
			return event;
		return {};
	}

	collision_event collision_solver::sweep_move(
		entities::components::collider* collider, glm::vec2& end_point)
	{
		collision_event closest_event;
		for (auto& c : impl->all_colliders)
			if (c != collider)
			{
				collision_event e = check_if_collider_collide_on_move(collider, end_point, c);
				if (e.distance < closest_event.distance)
					closest_event = e;
			}
		
		return closest_event;
	}

#define set_bit(source, distance_from_end, value) (source |= value << distance_from_end)
#define get_bit(source, distance_from_end) ((source >> distance_from_end) & 1)

	collision_present collision_solver::gen_flag(uint8_t body_type, std::array<collision_response, 14> responses)
	{
		collision_present flag = body_type;
		flag = flag << (sizeof(collision_present) * 8 - collision_present_body_type_size);
		for (int i = 0; i < 14; i++)
		{
			set_bit(flag, i * 2, get_bit(uint8_t(responses[i]), 0));
			set_bit(flag, i * 2 + 1, get_bit(uint8_t(responses[i]), 1));
		}

		return flag;
	}

	collision_response collision_solver::get_response_type(const collision_present& f1, const collision_present& f2)
	{
#define channel_id_at_2 (f1 >> (sizeof(collision_present) * 8 - collision_present_body_type_size))
#define channel_id_at_1 (f2 >> (sizeof(collision_present) * 8 - collision_present_body_type_size))

		uint8_t resp1 = 0;
		set_bit(resp1, 0, get_bit(f1, channel_id_at_1 * 2));
		set_bit(resp1, 1, get_bit(f1, channel_id_at_1 * 2 + 1));

		uint8_t resp2 = 0;
		set_bit(resp2, 0, get_bit(f2, channel_id_at_2 * 2));
		set_bit(resp2, 1, get_bit(f2, channel_id_at_2 * 2 + 1));

#undef channel_id_at_2
#undef channel_id_at_2

		if (resp1 + resp2 == 0)
			return collision_response::ignore;
		else if (resp1 + resp2 == 4)
			return collision_response::collide;
		return collision_response::overlap;
	}
}