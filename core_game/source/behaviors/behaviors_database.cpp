#include "behaviors_database.h"
#include <unordered_map>
#include <string>
#include <list>

namespace behaviors
{
	template <class T>
	struct references_stack
	{
	protected:
		std::list<std::weak_ptr<T>> refs;
	public:
		void assign(std::weak_ptr<T>* new_ref)
		{
			refs.push_back(*new_ref);
		}
		std::weak_ptr<T>* get()
		{
			return &refs.back();
		}
		void purge()
		{
			auto back = std::move(refs.back());
			refs.clear();
			refs.push_back(back);
		}
	};

	using entity_ref = std::weak_ptr<::entities::entity>;
	using entity_refs_stack = references_stack<::entities::entity>;

	struct variant
	{
		database_data_types data_type = database_data_types::unspecified;
		void* data = nullptr;
		
		//remove unvalid references from references stacks
		void purge()
		{
			if (data_type == database_data_types::entity)
				((entity_refs_stack*)data)->purge();
		}

		void free_data()
		{
			if (data != nullptr && data_type != database_data_types::unspecified)
				switch (data_type)
				{
				case behaviors::database_data_types::Int:
					delete (int*)data; break;
				case behaviors::database_data_types::Float:
					delete (float*)data; break;
				case behaviors::database_data_types::Bool:
					delete (bool*)data; break;
				case behaviors::database_data_types::string:
					delete (std::string*)data; break;
				case behaviors::database_data_types::entity:
					delete (entity_refs_stack*)data; break;
				}
		}

		bool assign_data(database_data_types dt, void* new_data)
		{
			if (dt == data_type && data != nullptr)
				switch (data_type)
				{
				case behaviors::database_data_types::Int:
					*((int*)data) = *(int*)new_data;
					break;
				case behaviors::database_data_types::Float:
					*((float*)data) = *(float*)new_data;
					break;
				case behaviors::database_data_types::Bool:
					*((bool*)data) = *((bool*)new_data);
					break;
				case behaviors::database_data_types::string:
					*((std::string*)data) = *((std::string*)new_data);
					break;
				case behaviors::database_data_types::entity:
					((entity_refs_stack*)data)->assign((entity_ref*)new_data);
					return true;
				}
			else
			{
				free_data();
				data_type = dt;
				switch (data_type)
				{
				case behaviors::database_data_types::Int:
					data = new int;
					*((int*)data) = *(int*)new_data;
					break;
				case behaviors::database_data_types::Float:
					data = new float;
					*((float*)data) = *(float*)new_data;
					break;
				case behaviors::database_data_types::Bool:
					data = new bool;
					*((bool*)data) = *(bool*)new_data;
					break;
				case behaviors::database_data_types::string:
					data = new std::string;
					*((std::string*)data) = *((std::string*)new_data);
					break;
				case behaviors::database_data_types::entity:
					data = new entity_refs_stack;
					((entity_refs_stack*)data)->assign((entity_ref*)new_data);
					break;
				}
			}
			return false;
		}
	};

	struct database::implementation
	{
		std::unordered_map<uint32_t, variant> data;
		std::vector<variant*> to_be_purged;
	};

	database::database() : impl(new implementation) {}
	database::~database() 
	{ 
		for (auto& vr : impl->data)
			vr.second.free_data();
		delete impl; 
	}

	void database::purge()
	{
		for (auto& vr : impl->to_be_purged)
			vr->purge();
	}

#define set_func(dt, _id, _data)						\
	auto x = impl->data.find(_id);						\
	if (x == impl->data.end())							\
	{													\
		variant vr;										\
		vr.assign_data(database_data_types::dt, &_data);			\
		impl->data.insert({ _id, vr });					\
	}													\
	else												\
		if(x->second.assign_data(database_data_types::dt, &_data))	\
			impl->to_be_purged.push_back(&x->second);

#define get_func(dt, _id, _default, _ctype)						\
	auto x = impl->data.find(_id);								\
	if (x == impl->data.end())									\
		return _default;										\
	else if (x->second.data_type == database_data_types::dt)	\
		return *(_ctype*)x->second.data;						\
	else														\
		return _default;
		

	database_data_types database::get_data_type_at(uint32_t& id)
	{
		auto x = impl->data.find(id);
		if (x == impl->data.end())
			return database_data_types::unspecified;
		return x->second.data_type;
	}

	void database::set_float(uint32_t& id, float f)
	{
		set_func(Float, id, f)
	}

	float database::get_float(uint32_t& id)
	{
		get_func(Float, id, 0.0, float)
	}

	void database::set_int(uint32_t& id, int i)
	{
		set_func(Int, id, i)
	}

	int database::get_int(uint32_t& id)
	{
		get_func(Int, id, 0, int)
	}

	void database::set_bool(uint32_t& id, bool b)
	{
		set_func(Bool, id, b)
	}

	bool database::get_bool(uint32_t& id)
	{
		get_func(Bool, id, 0, bool)
	}

	void database::set_string(uint32_t& id, std::string s)
	{
		set_func(string, id, s)
	}

	std::string database::get_string(uint32_t& id)
	{
		get_func(string, id, "null", std::string)
	}

	void database::set_entity(uint32_t& id, std::weak_ptr<entities::entity>* e)
	{
		set_func(entity, id, *e)
	}

	std::weak_ptr<entities::entity>* database::get_entity(uint32_t& id)
	{
		auto x = impl->data.find(id);
		if (x == impl->data.end())
			return nullptr;
		else if (x->second.data_type != database_data_types::entity)
			return nullptr;
		else
			return ((entity_refs_stack*)x->second.data)->get();
	}
}