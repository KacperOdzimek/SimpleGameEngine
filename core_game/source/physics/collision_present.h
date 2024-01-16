#pragma once
#include <cstdint>
#include <array>

namespace entities
{
	namespace components
	{
		class collider;
	}
}

namespace physics
{
	using collision_present = uint32_t;
	constexpr uint8_t collision_present_body_type_size = 4;

	enum class collision_response : uint8_t
	{
		ignore = 0, overlap = 1, collide = 2
	};

	struct collision_event
	{
		collision_response response;
		entities::components::collider* other;
		float distance;
		glm::vec2 location;
		glm::vec2 normal;		

		collision_event() : response(collision_response::ignore), other(nullptr), 
			distance(0), location({ 0,0 }), normal({ 0,0 }) {};
		collision_event(
			collision_response _resp, entities::components::collider* _other,
			float _distance, glm::vec2 _location, glm::vec2 _normal) :
			response(_resp), other(_other), distance(_distance), location(_location), normal(_normal) {};
	};
}