#pragma once 
#include <string>

namespace input
{
	enum class key_type
	{
		mouse, keyboard, gamepad
	};

	struct key
	{
		key_type key_type;
		uint32_t id;
		float axis_value = 0;
	};

	bool operator== (const key& lhs, const key& rhs);
	bool operator<  (const key & lhs, const key & rhs);

	struct key_state
	{
		key key;
		float state;
	};

	key get_key_from_key_name(const std::string& str);
}