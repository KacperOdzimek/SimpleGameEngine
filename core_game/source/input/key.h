#pragma once 
#include <string>

namespace input
{
	struct key
	{
		enum class key_type_enum
		{
			mouse_left, mouse_right, keyboard
		};

		const key_type_enum key_type;
		const uint32_t keyboard_button;
		const float value = 1;
		key(std::string _button, float _value);
		friend bool operator==(const key& lhs, const key& rhs);
	};

	bool operator== (const key& lhs, const key& rhs);
}