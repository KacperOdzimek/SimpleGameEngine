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
		entity* owner;
	public:
		virtual ~component() {};
	};
}