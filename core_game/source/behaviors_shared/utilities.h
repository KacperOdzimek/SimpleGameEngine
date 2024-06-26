#pragma once
#include "source/common/common.h"
#include "source/common/crash.h"

#include "source/assets/asset.h"
#include "source/assets/assets_manager.h"

#include "source/assets/mesh_asset.h"
#include "source/assets/shader_asset.h"
#include "source/assets/texture_asset.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/entities/entity.h"
#include "source/entities/component.h"

#include "source/utilities/hash_string.h"

#include "source/rendering/render_config.h"

inline void push_entity(lua_State* L, std::weak_ptr<::entities::entity>& entity)
{
	auto* data = (std::weak_ptr<::entities::entity>*)(lua_newuserdata(L, sizeof(entity)));
	new(data) std::weak_ptr<::entities::entity>(entity);
	luaL_getmetatable(L, "entity");
	lua_setmetatable(L, -2);
}

inline std::shared_ptr<::entities::entity> load_entity(lua_State* L, int arg_id, const std::string parent_function)
{
	if (!lua_isuserdata(L, arg_id))
		error_handling::crash(error_handling::error_source::mod, parent_function,
			"Entity reference should be std::weak_ptr<::entities::entity>* userdata");

	auto* ptr = reinterpret_cast<std::weak_ptr<::entities::entity>*>(luaL_checkudata(L, arg_id, "entity"));
	luaL_argcheck(L, ptr != nullptr, arg_id, "Entity reference expected");

	if (ptr->expired())
		error_handling::crash(error_handling::error_source::mod, parent_function, "Trying to perform operations on dead entity.");
	return ptr->lock();
}

inline uint32_t load_id(lua_State* L, int arg_id, std::string parent_function, const std::string id_of_what)
{
	if (lua_isnumber(L, arg_id) || lua_isinteger(L, arg_id))
		return static_cast<uint32_t>(lua_tointeger(L, arg_id));
	else if (lua_isstring(L, arg_id))
		return ::utilities::hash_string(lua_tostring(L, arg_id));
	error_handling::crash(error_handling::error_source::mod, parent_function, id_of_what + " id should be integer or string");
	return 0;
}

inline std::string load_asset_path(lua_State* L, int arg_id, std::string parent_function)
{
	if (lua_isstring(L, arg_id))
		return lua_tostring(L, arg_id);
	error_handling::crash(error_handling::error_source::mod, parent_function, "Asset path should be string");
	return "";
}

template<class comp_class>
inline comp_class* load_component(lua_State* L, const std::string parent_function, int entity_ptr_pos = 1, int component_id_pos = 2)
{
	auto e = load_entity(L, entity_ptr_pos, parent_function);
	uint32_t comp = load_id(L, component_id_pos, parent_function, "Component");
	auto component = e->get_component(comp);
	if (component == nullptr)
		error_handling::crash(error_handling::error_source::mod, parent_function, 
			"Entity does not contain component with tag: " + std::to_string(comp));
	auto casted = dynamic_cast<comp_class*>(component);
	if (casted == nullptr)
		error_handling::crash(error_handling::error_source::mod, parent_function, 
			"Component: " + std::to_string(comp) + " is not of the type you try to operate on");
	return casted;
}

inline rendering::render_config load_render_config(lua_State* L, int arg_id, const std::string parent_function)
{
	rendering::render_config rc{};

	lua_pushnil(L);
	if (lua_istable(L, arg_id))
	{
		while (lua_next(L, arg_id) != 0)
		{
			if (lua_isstring(L, -2))
			{
				std::string key = lua_tostring(L, -2);
				if (key == "shader")
				{
					rc.material = assets::cast_asset<assets::shader>(common::assets_manager->safe_get_asset(
						load_asset_path(L, -1, parent_function)
					)).lock();
				}
				else if (key == "mesh")
				{
					rc.mesh = assets::cast_asset<assets::mesh>(common::assets_manager->safe_get_asset(
						load_asset_path(L, -1, parent_function)
					)).lock();
				}
				else if (key == "textures")
				{
					if (lua_istable(L, arg_id + 2))
					{
						lua_pushnil(L);
						while (lua_next(L, arg_id + 2) != 0)
						{
							rc.textures.push_back(
								assets::cast_asset<assets::texture>(common::assets_manager->safe_get_asset(
									load_asset_path(L, -1, parent_function)
								)).lock()
							);
							lua_pop(L, 1);
						}
					}
				}
			}

			lua_pop(L, 1);
		}
	}

	if (rc.material == nullptr)
		error_handling::crash(error_handling::error_source::mod, parent_function,
			"Render config is missing shader");
	else if (rc.mesh == nullptr)
		error_handling::crash(error_handling::error_source::mod, parent_function,
			"Render config is missing mesh");

	return rc;
}

#include "include/nlohmann/json.hpp"

inline bool is_string_int(const std::string& s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

inline void dump_json_to_table(lua_State* L, nlohmann::json* object, const std::string& parent_func_name)
{
	lua_newtable(L);
	int counter = 1;
	for (auto element = object->begin(); element != object->end(); ++element)
	{
		if (object->is_array())
			lua_pushinteger(L, counter);
		else if (is_string_int(element.key()))
			lua_pushinteger(L, std::stoi(element.key().c_str()));
		else
			lua_pushstring(L, element.key().c_str());

		auto& value = element.value();
		switch (element.value().type())
		{
		case nlohmann::json::value_t::string:
			lua_pushstring(L, std::string(value).c_str());
			break;
		case nlohmann::json::value_t::number_integer:
			lua_pushinteger(L, int(value));
			break;
		case nlohmann::json::value_t::number_float:
			lua_pushnumber(L, float(value));
			break;
		case nlohmann::json::value_t::number_unsigned:
			lua_pushinteger(L, int(value));
			break;
		case nlohmann::json::value_t::boolean:
			lua_pushboolean(L, bool(value));
			break;
		case nlohmann::json::value_t::object:
			dump_json_to_table(L, &value, parent_func_name);
			break;
		case nlohmann::json::value_t::array:
			dump_json_to_table(L, &value, parent_func_name);
			break;
		default:
			error_handling::crash(error_handling::error_source::core, parent_func_name,
				"Unsuported data type: " + std::string(value.type_name()));
		}
		lua_settable(L, -3);
		counter++;
	}
}

inline void dump_table_to_json(lua_State* L, nlohmann::json* object, int table_stack_id, const std::string& parent_func_name)
{
	lua_pushvalue(L, table_stack_id);
	lua_pushnil(L);

	while (lua_next(L, -2))
	{
		lua_pushvalue(L, -2);
		auto key = std::string(lua_tostring(L, -1));

		switch (lua_type(L, -2))
		{
		case LUA_TNUMBER:
			(*object)[key] = static_cast<float>(lua_tonumber(L, -2));
			break;
		case LUA_TBOOLEAN:
			(*object)[(key)] = static_cast<bool>(lua_toboolean(L, -2));
			break;
		case LUA_TSTRING:
			(*object)[key] = std::string(lua_tostring(L, -2));
			break;
		case LUA_TTABLE:
			auto nested_object = nlohmann::json::object();
			dump_table_to_json(L, &nested_object, -2, parent_func_name);
			(*object)[key] = nested_object;
			break;
		}
		lua_pop(L, 2);
	}
	lua_pop(L, 1);
}

inline void push_nil_to_table(lua_State* L, const char* name)
{
	lua_pushstring(L, name);
	lua_pushnil(L);
	lua_settable(L, -3);
}

inline void push_number_to_table(lua_State* L, const char* name, float value)
{
	lua_pushstring(L, name);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}

inline void push_string_to_table(lua_State* L, const char* name, const char* value)
{
	lua_pushstring(L, name);
	lua_pushstring(L, value);
	lua_settable(L, -3);
}

inline void push_entity_to_table(lua_State* L, const char* name, std::weak_ptr<::entities::entity> value)
{
	lua_pushstring(L, name);
	push_entity(L, value);
	lua_settable(L, -3);
}