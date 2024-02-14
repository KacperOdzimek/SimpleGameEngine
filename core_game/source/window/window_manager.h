#pragma once 
#include "source/assets/input_config_asset.h"

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

		void change_frame();
		bool should_close();

		std::pair<int, int> get_size();

		void set_resize_callback(std::function<void(void)> callback);
	};
}