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
		entity* owner = nullptr;
	public:
		virtual ~component() {};
		virtual void on_attach() = 0;
	};
}