#include "scene.h"

using namespace entities;

scene::~scene()
{
	for (auto& entity : entities)
		delete entity;
}

void scene::add_entity(entity* entity)
{
	entities.push_back(entity);
}

#include <algorithm>

bool scene::remove_entity(entity* entity)
{
	auto position = std::find(entities.begin(), entities.end(), entity);
	if (position != entities.end())
	{
		entities.erase(position);
		return 1;
	}
	return 0;
}