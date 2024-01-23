#pragma once
#include <cstdint>
#include <memory>
#include "include/glm/glm.hpp"

namespace rendering
{
	class renderer;
	struct vertices_buffer_iterator;
}

namespace entities
{
	class entity;

	class component
	{
		friend entities::entity;
	protected:
		uint32_t id;
		entity* owner;
		std::weak_ptr<entity> get_owner_weak();
		glm::vec2& get_owner_location();
	public:
		component(uint32_t _id) : id(_id) {};
		virtual ~component() {};
		virtual void on_attach() = 0;
	};
}