#pragma once
#include <string>
#include <memory>

namespace assets
{
	enum class asset_type
	{
		behavior, texture, flipbook, shader, sound, cutscene, level
	};

	struct asset
	{
		std::string package_name;
		virtual ~asset() {};
	};

	template <class derived>
	std::weak_ptr<derived> cast_asset(std::weak_ptr<asset> asset)
	{
		return std::dynamic_pointer_cast<derived>(asset.lock());
	}
}