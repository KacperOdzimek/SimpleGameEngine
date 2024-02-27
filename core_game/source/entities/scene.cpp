#include "scene.h"

using namespace entities;

constexpr int purge_triggering_dangling_pointers_amount = 30 * 60;  //Purge every 30 seconds

scene::~scene()
{
	for (auto& e : entities)
		if (!e.expired())
			e.lock()->kill();
}

void scene::update()
{
    frames_since_purged++;
	if (frames_since_purged > purge_triggering_dangling_pointers_amount)
	{
        frames_since_purged = 0;
        auto iter = entities.begin();
        while (iter != entities.end()) {
            if (iter->expired()) {
                iter = entities.erase(iter);
            }
            else {
                ++iter;
            }
        }
	}
}