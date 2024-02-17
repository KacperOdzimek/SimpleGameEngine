#include "add_component_functions.h"

#include "utilities.h"

#include "source/common/common.h"

#include "source/assets/assets_manager.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/collision_config_asset.h"

#include "source/components/behavior.h"
#include "source/components/camera.h"
#include "source/components/collider.h"
#include "source/components/mesh.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace add_component
		{
			int _e_add_behavior(lua_State* L)
			{
				auto e = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				uint32_t bhv = load_id(L, 3);

				e->attach_component(
					new ::entities::components::behavior{
						id,
						::assets::cast_asset<::assets::behavior>(::common::assets_manager->get_asset(bhv))
					}
				);

				return 0;
			}

			int _e_add_camera(lua_State* L)
			{
				auto e = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				float width = lua_tonumber(L, 3);
				
				e->attach_component(
					new ::entities::components::camera{
						id, width
					}
				);

				return 0;
			}

			int _e_add_mesh(lua_State* L)
			{
				auto e = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				rendering::render_config rc = load_render_config(L, 3);

				e->attach_component(
					new ::entities::components::mesh{
						id, rc
					}
				);

				return 0;
			}

			int _e_add_collider(lua_State* L)
			{
				auto e = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				auto preset_name = lua_tostring(L, 3);
				physics::collision_preset preset;

				{
					auto config = ::assets::cast_asset<::assets::collision_config>(::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config")));
					preset = config.lock()->get_preset(utilities::hash_string(preset_name));
				}

				glm::vec2 extend;
				extend.x = lua_tonumber(L, 4);
				extend.y = lua_tonumber(L, 5);

				e->attach_component(
					new ::entities::components::collider{
						id, preset, extend
					}
				);

				return 0;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_e_add_behavior", _e_add_behavior);
				lua_register(L, "_e_add_camera", _e_add_camera);
				lua_register(L, "_e_add_mesh", _e_add_mesh);
				lua_register(L, "_e_add_collider", _e_add_collider);
			}
		}
	}
}