#include "scene.h"

using namespace entities;

scene::~scene()
{
	for (auto& e : entities)
		if (!e.expired())
			e.lock()->kill();
}