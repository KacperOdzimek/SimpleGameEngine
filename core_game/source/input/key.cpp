#include "key.h"
#include "include/glfw/glfw3.h"
#include "source/common/abort.h"

uint32_t map_button(std::string _button)
{
	//ascii letters ids are the same as the glfw mapping for them eg.
	//'Q' == 81 && GLFW_KEY_Q == 81
	if (_button.size() == 1 && _button.at(0) >= 'A' && _button.at(0) <= 'Z')
		return _button.at(0);

#define implement(name, binding) else if (_button == #name) return binding;
	implement("Enter", GLFW_KEY_ENTER)
	implement("Left Shift", GLFW_KEY_LEFT_SHIFT)
	implement("Right Shift", GLFW_KEY_RIGHT_SHIFT)

#undef implement;

	abort("Error while loading input config: Key not implemented: " + _button);
	return 0;
}

input::key::key_type_enum map_key_type(std::string _button)
{
	if (_button == "Left Mouse Button")
		return input::key::key_type_enum::mouse_left;

	else if (_button == "Right Mouse Button")
		return input::key::key_type_enum::mouse_right;

	return input::key::key_type_enum::keyboard;
}

input::key::key(std::string _button, float _value)
	: 
	key_type(map_key_type(_button)),
	keyboard_button(map_key_type(_button) == key_type_enum::keyboard ? map_button(_button) : 0),
	value(_value)
{
}

bool input::operator== (const key& lhs, const key& rhs)
{
	return lhs.keyboard_button == rhs.keyboard_button;
}