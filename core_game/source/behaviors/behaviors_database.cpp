#include "behaviors_database.h"
#include <unordered_map>
#include <string>

namespace behaviors
{
	enum class data_types
	{
		unspecified, Int, Float, string
	};

	struct variant
	{
		data_types data_type = data_types::unspecified;
		void* data = nullptr;

		void free_data()
		{
			if (data != nullptr && data_type != data_types::unspecified)
				switch (data_type)
				{
				case behaviors::data_types::Int:
					delete (int*)data; break;
				case behaviors::data_types::Float:
					delete (float*)data; break;
				case behaviors::data_types::string:
					delete (std::string*)data; break;
				}
		}

		void assign_data(data_types dt, void* new_data)
		{
			if (dt == data_type && data != nullptr)
				switch (data_type)
				{
				case behaviors::data_types::Int:
					*((int*)data) = *(int*)new_data;
					break;
				case behaviors::data_types::Float:
					*((float*)data) = *(float*)new_data;
					break;
				case behaviors::data_types::string:
					*((std::string*)data) = *((std::string*)new_data);
					break;
				}
			else
			{
				free_data();
				data_type = dt;
				switch (data_type)
				{
				case behaviors::data_types::Int:
					data = new int;
					*((int*)data) = *(int*)new_data;
					break;
				case behaviors::data_types::Float:
					data = new float;
					*((float*)data) = *(float*)new_data;
					break;
				case behaviors::data_types::string:
					data = new std::string;
					*((std::string*)data) = *((std::string*)new_data);
					break;
				}
			}
		}
	};

	struct database::implementation
	{
		std::unordered_map<uint32_t, variant> data;
	};

	database::database() : impl(new implementation) {}
	database::~database() { delete impl; }

#define set_func(dt, _id, _data)						\
	auto x = impl->data.find(_id);						\
	if (x == impl->data.end())							\
	{													\
		variant vr;										\
		vr.assign_data(data_types::dt, &_data);			\
		impl->data.insert({ _id, vr });					\
	}													\
	else												\
		x->second.assign_data(data_types::dt, &_data);	

#define get_func(dt, _id, _default, _ctype)				\
	auto x = impl->data.find(_id);						\
	if (x == impl->data.end())							\
		return _default;								\
	else if (x->second.data_type == data_types::dt)		\
		return *(_ctype*)x->second.data;				\
	else												\
		return _default;
		

	void database::set_float(uint32_t& id, float f)
	{
		set_func(Float, id, f)
	}

	float database::get_float(uint32_t& id)
	{
		get_func(Float, id, 0.0, float)
	}
}