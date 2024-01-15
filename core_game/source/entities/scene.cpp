#include "scene.h"

using namespace entities;

scene::~scene()
{
	for (auto& entity : entities)
		entity.reset();
}

std::weak_ptr<entity> scene::add_entity(entity* e)
{
	std::shared_ptr<entity> x{e};
	entities.push_back(x);
	return x;
}

#include <algorithm>

bool scene::remove_entity(std::shared_ptr<entity> e)
{
	auto position = std::find(entities.begin(), entities.end(), e);
	if (position != entities.end())
	{
		entities.erase(position);
		return 1;
	}
	return 0;
}