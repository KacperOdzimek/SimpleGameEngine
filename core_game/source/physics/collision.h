#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include "glm/glm.hpp"

namespace entities
{
	namespace components
	{
		class collider;
	}
}

namespace physics
{
	using collision_preset = uint32_t;
	constexpr uint8_t collision_preset_body_type_size = 4;

	enum class collision_response : uint8_t
	{
		ignore = 0, overlap = 1, collide = 2
	};

	struct collision_event
	{
		collision_response response;
		entities::components::collider* other;
		float distance;;
		glm::vec2 location;
		glm::vec2 normal;		

		collision_event() : response(collision_response::ignore), other(nullptr), 
			distance(FLT_MAX), location({ 0,0 }), normal({ 0,0 }) {};
		collision_event(
			collision_response _resp, entities::components::collider* _other,
			float _distance, glm::vec2 _location, glm::vec2 _normal) :
			response(_resp), other(_other), distance(_distance), location(_location), normal(_normal) {};
	};

	struct sweep_move_event
	{
		collision_event collide_event;
		std::vector<collision_event> overlap_events;
	};

	collision_preset gen_flag(uint8_t body_type, std::array<collision_response, 14> responses);
	collision_response get_response_type(const collision_preset& f1, const collision_preset& f2);
}