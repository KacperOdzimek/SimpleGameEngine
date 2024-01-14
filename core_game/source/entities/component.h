#pragma once
#include "source/common/data_types.h"

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
		entity* owner = nullptr;
	public:
		component(uint32_t _id) : id(_id) {};
		virtual ~component() {};
		virtual void on_attach() = 0;
	};
}