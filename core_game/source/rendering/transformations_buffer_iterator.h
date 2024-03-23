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
		inline void put(float&& v)
		{
			*(reinterpret_cast<float*>(current_position)) = v;
			current_position = reinterpret_cast<float*>(current_position) + 1;
		}
		inline void put(float& v)
		{
			*(reinterpret_cast<float*>(current_position)) = v;
			current_position = reinterpret_cast<float*>(current_position) + 1;
		}
		inline void put(int&& v)
		{
			*(reinterpret_cast<int*>(current_position)) = v;
			current_position = reinterpret_cast<int*>(current_position) + 1;
		}
		inline void put(int& v)
		{
			*(reinterpret_cast<int*>(current_position)) = v;
			current_position = reinterpret_cast<int*>(current_position) + 1;
		}
		uint32_t get_data_size()
		{
			return static_cast<uint32_t>((uint8_t*)current_position - (uint8_t*)begining);
		}
	};
}