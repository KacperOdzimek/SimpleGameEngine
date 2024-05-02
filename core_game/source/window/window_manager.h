#pragma once 
#include "source/input/key.h"

#include "include/glm/vec2.hpp"

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
		//returns mouse position in world space
		glm::vec2 get_mouse_position();

		void set_resize_callback(std::function<void(void)> callback);

		void vsync(double frame_processing_time);

		input::key_state get_key_state(input::key key);
	};
}