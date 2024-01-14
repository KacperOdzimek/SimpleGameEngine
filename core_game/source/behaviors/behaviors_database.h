#pragma once
#include <cstdint>

namespace behaviors
{
	struct database
	{
	private:
		struct implementation;
		implementation* impl;
	public:
		database();
		~database();
		void set_float(uint32_t& id, float f);
		float get_float(uint32_t& id);
	};
}