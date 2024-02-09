#pragma once
#include <cstdint>

namespace rendering
{
	struct transformations_buffer_iterator
	{
	private:
		void* begining;
		void* current_position;
	public:
		transformations_buffer_iterator(void* _begining) : begining(_begining), current_position(_begining) {};
		void put(float&& v)
		{
			*(reinterpret_cast<float*>(current_position)) = std::move(v);
			current_position = reinterpret_cast<float*>(current_position) + 1;
		}
		void put(float& v)
		{
			*(reinterpret_cast<float*>(current_position)) = v;
			current_position = reinterpret_cast<float*>(current_position) + 1;
		}
		uint32_t get_data_size()
		{
			return (uint8_t*)current_position - (uint8_t*)begining;
		}
	};
}