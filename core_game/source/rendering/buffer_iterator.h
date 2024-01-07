#pragma once

namespace rendering
{
	struct buffer_iterator
	{
	private:
		void* begining;
		void* current_possition;
	public:
		buffer_iterator(void* _begining) : begining(_begining), current_possition(_begining) {};
		void put(float&& v)
		{
			*(reinterpret_cast<float*>(current_possition)) = v;
			current_possition = reinterpret_cast<float*>(current_possition) + 1;
		}
		void put(int&& v)
		{
			*(reinterpret_cast<int*>(current_possition)) = v;
			current_possition = reinterpret_cast<int*>(current_possition) + 1;
		}
		uint32_t get_data_size()
		{
			return (uint8_t*)current_possition - (uint8_t*)begining;
		}
	};
}