#pragma once
#include "asset.h"
#include "include/nlohmann/json.hpp"

#include <memory>

namespace assets
{
	struct custom_data : public asset
	{
	protected:
		std::shared_ptr<nlohmann::json> _data;
	public:
		custom_data(std::unique_ptr<nlohmann::json>& data) : _data(std::move(data)) {};
		std::shared_ptr<nlohmann::json> access_data() { return _data; };
	};
}