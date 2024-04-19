#include "collision_functions.h"

#include "utilities.h"

#include "source/common/common.h"
#include "source/physics/collision_solver.h"

#include "source/assets/collision_config_asset.h"

#include "include/glm/glm.hpp"

namespace behaviors
{
	namespace lua_shared
	{
		namespace collision
		{
			int _cl_trace(lua_State* L)
			{
				physics::collision_preset trace_preset;
				{
					std::string trace_preset_name = lua_tostring(L, 1);
					auto config = ::assets::cast_asset<::assets::collision_config>(
						::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config")));
					trace_preset = config.lock()->get_preset(utilities::hash_string(trace_preset_name));
				}

				float start_x = static_cast<float>(lua_tonumber(L, 2));
				float start_y = static_cast<float>(lua_tonumber(L, 3));
				glm::vec2 start = { start_x, start_y };

				float end_x = static_cast<float>(lua_tonumber(L, 4));
				float end_y = static_cast<float>(lua_tonumber(L, 5));
				glm::vec2 end = { end_x, end_y };

				auto event = common::collision_solver->check_if_ray_collide(
					trace_preset,
					start,
					end - start
				);

				lua_newtable(L);

				if (event != nullptr && event->distance < glm::distance(start, end))
				{
					push_entity_to_table(L, "entity", event->other->get_owner_weak());
					push_number_to_table(L, "distance", event->distance);
				}
				else
				{
					push_nil_to_table(L, "entity");
					push_nil_to_table(L, "distance");
				}

				delete event;

				return 1;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_cl_trace", _cl_trace);
			}
		}
	}
}