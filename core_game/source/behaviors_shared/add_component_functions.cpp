#include "add_component_functions.h"

#include "utilities.h"

#include "source/common/common.h"

#include "source/assets/assets_manager.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/collision_config_asset.h"

#include "source/components/behavior.h"
#include "source/components/camera.h"
#include "source/components/collider.h"
#include "source/components/static_mesh.h"
#include "source/components/sprite.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace add_component
		{
			int _e_add_behavior(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_add_behavior]");
				uint32_t id = load_id(L, 2, "[_e_add_behavior]", "Component");
				auto bhv = load_asset_path(L, 3, "[_e_add_behavior]");

				e->attach_component(
					new ::entities::components::behavior{
						id,
						::assets::cast_asset<::assets::behavior>(::common::assets_manager->safe_get_asset(bhv))
					}
				);

				return 0;
			}

			int _e_add_camera(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_add_camera]");
				uint32_t id = load_id(L, 2, "[_e_add_camera]", "Component");
				float width = lua_tonumber(L, 3);
				
				e->attach_component(
					new ::entities::components::camera{
						id, width
					}
				);

				return 0;
			}

			int _e_add_static_mesh(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_add_static_mesh]");
				uint32_t id = load_id(L, 2, "[_e_add_static_mesh]", "Component");
				rendering::render_config rc = load_render_config(L, 3, "[_e_add_static_mesh]");

				e->attach_component(
					new ::entities::components::static_mesh{
						id, rc
					}
				);

				return 0;
			}

			int _e_add_sprite(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_add_sprite]");
				uint32_t id = load_id(L, 2, "[_e_add_sprite]", "Component");
				auto texture = load_asset_path(L, 3, "[_e_add_sprite]");
				int sprite_id = lua_tointeger(L, 4);
				auto preset_name = lua_tostring(L, 5);

				physics::collision_preset preset;
				{
					auto config = ::assets::cast_asset<::assets::collision_config>(::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config")));
					preset = config.lock()->get_preset(utilities::hash_string(preset_name));
				}

				auto sprite = new ::entities::components::sprite{
						id,
						::assets::cast_asset<::assets::texture>(::common::assets_manager->safe_get_asset(texture)),
						preset
				};

				e->attach_component(sprite);
				sprite->set_sprite_id(sprite_id);

				return 0;
			}

			int _e_add_collider(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_add_collider]");
				uint32_t id = load_id(L, 2, "[_e_add_collider]", "Component");
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
				lua_register(L, "_e_add_static_mesh", _e_add_static_mesh);
				lua_register(L, "_e_add_sprite", _e_add_sprite);
				lua_register(L, "_e_add_collider", _e_add_collider);
			}
		}
	}
}