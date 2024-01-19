#include "collision_solver.h"
#include "source/entities/entity.h"

#include <list>
#include <unordered_map>

using collider = entities::components::collider;
using space_chunk = std::list<collider*>;
struct chunks_positions_hash;

using space_chunks = std::unordered_map<std::pair<int, int>, space_chunk, chunks_positions_hash>;

struct chunks_positions_hash {
	std::size_t operator () (const std::pair<int, int>& p) const {
		auto h1 = std::hash<int>{}(p.first);
		auto h2 = std::hash<int>{}(p.second);
		if (h1 != h2)
			return h1 ^ h2;
		return h1;
	}
};

//Chunk size in world units
constexpr float chunk_size = 4;

namespace physics
{
	struct collision_solver::implementation
	{
		space_chunks chunks;
		void add_collider_to_chunk(collider* c, int chunk_x, int chunk_y)
		{
			auto itr = chunks.find({ chunk_x, chunk_y });
			if (itr == chunks.end())
				chunks.insert({ {chunk_x, chunk_y }, {c} });
			else
				chunks.at({ chunk_x, chunk_y }).push_back(c);
		}
		void remove_collider_from_chunk(collider* c, int chunk_x, int chunk_y)
		{
			auto itr = chunks.find({ chunk_x, chunk_y });
			if (itr != chunks.end())
				chunks.erase(itr);
		}
	};

	collision_solver::collision_solver() :
		impl(new implementation) {};

	collision_solver::~collision_solver()
	{
		delete impl;
	}

	void collision_solver::register_collider(collider* c)
	{
		int local_grid_center_x = floor(c->get_world_pos().x / chunk_size);
		int local_grid_center_y = floor(c->get_world_pos().y / chunk_size);

		auto extend = (c->extend / 2.0f);

		//I
		auto ver_pos = (c->get_world_pos() + extend) / chunk_size;
		if (ver_pos.x > local_grid_center_x && ver_pos.y > local_grid_center_y)
			impl->add_collider_to_chunk(c, local_grid_center_x, local_grid_center_y);
		//II
		ver_pos = (c->get_world_pos() + glm::vec2{-extend.x, extend.y}) / chunk_size;
		if (ver_pos.x < local_grid_center_x && ver_pos.y > local_grid_center_y)
			impl->add_collider_to_chunk(c, local_grid_center_x - 1, local_grid_center_y);
		//III
		ver_pos = (c->get_world_pos() + glm::vec2{-extend.x, -extend.y}) / chunk_size;
		if (ver_pos.x < local_grid_center_x && ver_pos.y < local_grid_center_y)
			impl->add_collider_to_chunk(c, local_grid_center_x - 1, local_grid_center_y - 1);
		//IV
		ver_pos = (c->get_world_pos() + glm::vec2{extend.x, -extend.y}) / chunk_size;
		if (ver_pos.x > local_grid_center_x && ver_pos.y < local_grid_center_y)
			impl->add_collider_to_chunk(c, local_grid_center_x, local_grid_center_y - 1);
	}

	void collision_solver::unregister_collider(entities::components::collider* c)
	{
		int local_grid_center_x = floor(c->get_world_pos().x / chunk_size);
		int local_grid_center_y = floor(c->get_world_pos().y / chunk_size);

		auto extend = (c->extend / 2.0f);

		//I
		auto ver_pos = (c->get_world_pos() + extend) / chunk_size;
		if (ver_pos.x > local_grid_center_x && ver_pos.y > local_grid_center_y)
			impl->remove_collider_from_chunk(c, local_grid_center_x, local_grid_center_y);
		//II
		ver_pos = (c->get_world_pos() + glm::vec2{-extend.x, extend.y}) / chunk_size;
		if (ver_pos.x < local_grid_center_x && ver_pos.y > local_grid_center_y)
			impl->remove_collider_from_chunk(c, local_grid_center_x - 1, local_grid_center_y);
		//III
		ver_pos = (c->get_world_pos() + glm::vec2{-extend.x, -extend.y}) / chunk_size;
		if (ver_pos.x < local_grid_center_x && ver_pos.y < local_grid_center_y)
			impl->remove_collider_from_chunk(c, local_grid_center_x - 1, local_grid_center_y - 1);
		//IV
		ver_pos = (c->get_world_pos() + glm::vec2{extend.x, -extend.y}) / chunk_size;
		if (ver_pos.x > local_grid_center_x && ver_pos.y < local_grid_center_y)
			impl->remove_collider_from_chunk(c, local_grid_center_x, local_grid_center_y - 1);
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