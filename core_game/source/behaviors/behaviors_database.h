#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include "source/entities/entity.h"

namespace behaviors
{
	enum class database_data_types : uint8_t
	{
		unspecified, Int, Float, string, entity
	};

	struct database
	{
	private:
		struct implementation;
		implementation* impl;
	public:
		database();
		~database();
		void purge();
		database_data_types get_data_type_at(uint32_t& id);

		void set_float(uint32_t& id, float f);
		float get_float(uint32_t& id);

		void set_int(uint32_t& id, int i);
		int get_int(uint32_t& id);

		void set_string(uint32_t& id, std::string s);
		std::string get_string(uint32_t& id);

		void set_entity(uint32_t& id, std::weak_ptr<entities::entity>* e);
		std::weak_ptr<entities::entity>* get_entity(uint32_t& id);
	};
}