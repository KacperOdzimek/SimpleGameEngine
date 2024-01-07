#include "scene.h"

using namespace entities;

void scene::add_entity(entity* entity)
{
	entities.push_back(entity);
}

scene::~scene()
{
	for (auto& entity : entities)
		delete entity;
}