#include "flipbooks_manager.h"
#include "source/components/flipbook.h"

#include "source/common/common.h"
#include <vector>

using flipbook = entities::components::flipbook;

struct rendering::flipbooks_manager::implementaion
{
	std::vector<flipbook*> flipbooks;
};

rendering::flipbooks_manager::flipbooks_manager()
{
	impl = new implementaion;
}

rendering::flipbooks_manager::~flipbooks_manager()
{
	delete impl;
}

void rendering::flipbooks_manager::register_flipbook(entities::components::flipbook* flipbook)
{
	impl->flipbooks.push_back(flipbook);
}

void rendering::flipbooks_manager::unregister_flipbook(entities::components::flipbook* flipbook)
{
	auto itr = std::find(impl->flipbooks.begin(), impl->flipbooks.end(), flipbook);
	impl->flipbooks.erase(itr);
}

void rendering::flipbooks_manager::update()
{
	for (auto& flip : impl->flipbooks)
		flip->move_playback(static_cast<float>(common::delta_time));
}