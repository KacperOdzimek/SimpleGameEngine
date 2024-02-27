#include "scene.h"
#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"

using namespace entities;

constexpr int purge_triggering_dangling_pointers_amount = 30 * 60;  //Purge every 30 seconds

scene::scene()
{
}

scene::scene(std::weak_ptr<assets::scene> __scene)
{
    _scene = __scene.lock();
    if (_scene.get() != nullptr && 
        !common::behaviors_manager->prepare_scene_function_call(behaviors::functions::init, _scene.get()))
        return;

    common::behaviors_manager->create_frame(nullptr, this);
    common::behaviors_manager->call(0);
    common::behaviors_manager->pop_frame();

}

scene::~scene()
{
    if (_scene.get() == nullptr || 
        !common::behaviors_manager->prepare_scene_function_call(behaviors::functions::destroy, _scene.get()))
        return;

    common::behaviors_manager->create_frame(nullptr, this);
    common::behaviors_manager->call(0);
    common::behaviors_manager->pop_frame();

	for (auto& e : entities)
		if (!e.expired())
			e.lock()->kill();
}

void scene::update()
{
    if (_scene.get() == nullptr || 
        !common::behaviors_manager->prepare_scene_function_call(behaviors::functions::destroy, _scene.get()))
        return;

    common::behaviors_manager->create_frame(nullptr, this);
    common::behaviors_manager->pass_float_arg(common::delta_time);
    common::behaviors_manager->call(1);
    common::behaviors_manager->pop_frame();

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