#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include "glm/vec2.hpp"

namespace entities
{
	namespace components
	{
		class collider;
	}
}

namespace physics
{
	/*
		collision_preset
		32-bit flag that determines how collider should interact with other colliders
	*/
	using collision_preset = uint32_t;

	/*
		collision_preset_body_type_size
		size in bits type identificator at the begining of collision_preset
	*/
	constexpr uint8_t collision_preset_body_type_size = 4;

	/*
		collision_response
		contains types of collision events
	*/
	enum class collision_response : uint8_t
	{
		ignore = 0, overlap = 1, collide = 2
	};

	/*
		collision_event
		structure that contains informations about hit/overlap between two colliders
	*/
	struct collision_event
	{
		/*
			response
			type of event
			if response == ignore then there is no event
		*/
		collision_response response;
		/*
			other
			pointer to other collider
		*/
		entities::components::collider* other;
		/*
			distance
			distance of the event from the begining of the ray/path
		*/
		float distance;
		/*
			location
			event location in world space
		*/
		glm::vec2 location;
		/*
			normal
			other collider event normal
		*/
		glm::vec2 normal;		

		collision_event() : response(collision_response::ignore), other(nullptr), 
			distance(FLT_MAX), location({ 0,0 }), normal({ 0,0 }) {};
		collision_event(
			collision_response _resp, entities::components::collider* _other,
			float _distance, glm::vec2 _location, glm::vec2 _normal) :
			response(_resp), other(_other), distance(_distance), location(_location), normal(_normal) {};
	};

	/*
		sweep_move_event
		return structure of collision_solver::sweep_move
	*/
	struct sweep_move_event
	{
		/*
			collide_event
			closest collision
		*/
		collision_event collide_event;
		/*
			overlap_events
			all overlap events before collide_event
		*/
		std::vector<collision_event> overlap_events;
	};

	/*
		gen_flag
		generate collision_preset flag from body_type and array of responses
	*/
	collision_preset gen_flag(uint8_t body_type, std::array<collision_response, 14> responses);

	/*
		get_response_type
		returns how would two colliders interact
	*/
	collision_response get_response_type(const collision_preset& f1, const collision_preset& f2);
}