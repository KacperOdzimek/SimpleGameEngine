#include "collision_solver.h"
#include "source/entities/entity.h"

namespace physics
{
	collision_event collision_solver::check_if_in_ray_dir(
		collision_present trace_present, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider)
	{
		auto response = determine_response_type(trace_present, collider->present);
		if (response == collision_response::ignore)
			return {};

		glm::vec2 near = (collider->get_world_pos() - collider->extend / 2.0f - trace_begin) / trace_dir;
		glm::vec2 far =  (collider->get_world_pos() + collider->extend / 2.0f - trace_begin) / trace_dir;

		if (near.x > far.x) std::swap(near.x, far.x);
		if (near.y > far.y) std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x) return {};

		collision_event e;
		
		e.distance = std::max(near.x, far.y);
		float hit_far = std::min(far.x, far.y);

		if (hit_far < 0) return {};

		e.location = trace_begin + e.distance * trace_dir;

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

	collision_response collision_solver::determine_response_type(const collision_present& f1, const collision_present& f2)
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