#pragma once

namespace entities
{
	namespace components
	{
		class dynamics;
	}
}

namespace physics
{
	class dynamics_manager
	{
	private:
		struct implementaion;
		implementaion* impl;
	public:
		dynamics_manager();
		~dynamics_manager();
		void register_dynamics(entities::components::dynamics* dynamics);
		void unregister_dynamics(entities::components::dynamics* dynamics);
		void update();
	};
}