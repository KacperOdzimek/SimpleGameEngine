#pragma once

namespace entities
{
	namespace components
	{
		class flipbook;
	}
}

namespace rendering
{
	class flipbooks_manager
	{
	private:
		struct implementaion;
		implementaion* impl;
	public:
		flipbooks_manager();
		~flipbooks_manager();
		void register_flipbook(entities::components::flipbook* flipbook);
		void unregister_flipbook(entities::components::flipbook* flipbook);
		void update();
	};
}