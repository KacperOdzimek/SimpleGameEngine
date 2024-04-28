#include "key.h"
#include "include/glfw/glfw3.h"
#include "source/common/crash.h"
#include <cctype>

bool input::operator== (const key& lhs, const key& rhs)
{
	return lhs.key_type == rhs.key_type && lhs.id == rhs.id;
}

bool input::operator< (const input::key& lhs, const input::key& rhs)
{
	return lhs.id < rhs.id;
}

input::key input::get_key_from_key_name(const std::string& str)
{
	input::key k;
	k.id = UINT32_MAX;
	k.key_type = input::key_type::keyboard;
	//maps with ascii values
	if (str.size() == 1)
	{
		char a = std::toupper(str.at(0));
		k.id = a;
	}
	else if (str == "Space")
		k.id = GLFW_KEY_SPACE;
	else if (str == "Enter")
		k.id = GLFW_KEY_ENTER;
	else if (str == "LShift")
		k.id = GLFW_KEY_LEFT_SHIFT;
	else if (str == "RShift")
		k.id = GLFW_KEY_RIGHT_SHIFT;
	else if (str == "Tab")
		k.id = GLFW_KEY_TAB;
	else if (str == "LCtrl")
		k.id = GLFW_KEY_LEFT_CONTROL;
	else if (str == "RCtrl")
		k.id = GLFW_KEY_RIGHT_CONTROL;
	else if (str == "Backspace")
		k.id = GLFW_KEY_BACKSLASH;
	else if (str == "LPM")
	{
		k.id = GLFW_MOUSE_BUTTON_LEFT;
		k.key_type = input::key_type::mouse;
	}
	else if (str == "RPM")
	{
		k.id = GLFW_MOUSE_BUTTON_RIGHT;
		k.key_type = input::key_type::mouse;
	}
	if (k.id == UINT32_MAX)
		error_handling::crash(error_handling::error_source::core, "[input::get_key_from_key_name]",
			"Trying to use not implemented key: " + str);

	return k;
}