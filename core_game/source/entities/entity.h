#pragma once
#include "component.h"
#include "source/physics/collision.h"

#include "include/glm/vec2.hpp"

#include <memory>
#include <list>

namespace entities
{
	class component;

	class entity
	{
	friend std::shared_ptr<entities::entity>;
	friend class scene;
	friend component;
	protected:
		std::list<component*> components;
		std::shared_ptr<entity> self;
		glm::vec2 location{ 0.0f, 0.0f };
	public:
		uint8_t layer = 0;
		entity();
		entity(scene* parent_scene);
		/*
			kill
			destroys the entity and all of its components
		*/
		void kill();

		/*
			get_location
			returns entity get_location
		*/
		const glm::vec2& get_location();

		/*
			teleport
			moves entity to new location without checking for collision events
		*/
		void teleport(glm::vec2 new_location);
		
		/*
			sweep
			simulates entity move to the new_location while checking for collision along the way
		*/
		physics::collision_event sweep(glm::vec2 new_location);

		/*
			attach_component
			adds component to the entity
		*/
		void attach_component(component* comp);

		/*
			get_component
			returns component of given id
		*/
		component* get_component(uint32_t id);

		/*
			kill_component
			destroys component of given id
		*/
		void kill_component(uint32_t id);

		/*
			get_weak
			returns weak pointer connected with entity::self
		*/
		std::weak_ptr<entities::entity> get_weak();

		inline const std::list<component*> get_components() { return components; };

		~entity();
	};
}