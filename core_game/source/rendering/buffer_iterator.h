#pragma once

namespace rendering
{
	struct vertices_buffer_iterator
	{
	private:
		void* begining;
		void* current_possition;
	public:
		vertices_buffer_iterator(void* _begining) : begining(_begining), current_possition(_begining) {};
		void put(float&& v)
		{
			*(reinterpret_cast<float*>(current_possition)) = v;
			current_possition = reinterpret_cast<float*>(current_possition) + 1;
		}
		uint32_t get_data_size()
		{
			return (uint8_t*)current_possition - (uint8_t*)begining;
		}
	};

	struct indicies_buffer_iterator
	{
	private:
		void* begining;
		void* current_possition;
		int add = 0;
	public:
		indicies_buffer_iterator(void* _begining) : begining(_begining), current_possition(_begining) {};
		void put(int&& v)
		{
			*(reinterpret_cast<int*>(current_possition)) = v + add;
			current_possition = reinterpret_cast<int*>(current_possition) + 1;
		}
		void offset(unsigned int&& offset)
		{
			add += offset;
		}
		
		uint32_t get_data_size()
		{
			return (uint8_t*)current_possition - (uint8_t*)begining;
		}
	};
}