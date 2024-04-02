#include "dynamics_manager.h"
#include "source/components/dynamics.h"

#include "source/common/common.h"
#include <vector>

using dynamics = entities::components::dynamics;

struct physics::dynamics_manager::implementaion
{
	std::vector<dynamics*> all_dynamics;
};

physics::dynamics_manager::dynamics_manager()
{
	impl = new implementaion;
}

physics::dynamics_manager::~dynamics_manager()
{
	delete impl;
}

void physics::dynamics_manager::register_dynamics(entities::components::dynamics* dyn)
{
	impl->all_dynamics.push_back(dyn);
}

void physics::dynamics_manager::unregister_dynamics(entities::components::dynamics* dyn)
{
	auto itr = std::find(impl->all_dynamics.begin(), impl->all_dynamics.end(), dyn);
	impl->all_dynamics.erase(itr);
}

void physics::dynamics_manager::update()
{
	for (auto& d : impl->all_dynamics)
	{
		d->grouned = false;
		d->sweep();
	}
}