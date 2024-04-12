#pragma once 
#include "source/input/key.h"

#include <functional>

namespace window
{
	class window_manager
	{
	private:
		struct implementation;
		implementation* impl;
	public:
		window_manager();
		~window_manager();

		void create_window(std::string title, int width, int height, bool fullscreen);

		void update();
		bool should_close();

		std::pair<int, int> get_size();

		void set_mouse_visible(bool visible);
		void set_resize_callback(std::function<void(void)> callback);

		void vsync();

		input::key_state get_key_state(input::key key);
	};
}