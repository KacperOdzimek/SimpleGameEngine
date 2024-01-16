#include "scene.h"

using namespace entities;

scene::~scene()
{
	for (auto& entity : entities)
		entity.reset();
}