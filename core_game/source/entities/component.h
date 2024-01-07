#pragma once
#include "common/data_types.h"

namespace rendering
{
	class renderer;
	struct buffer_iterator;
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