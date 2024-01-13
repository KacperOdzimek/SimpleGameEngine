#include "behaviors_manager.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/common/common.h"

#include "source/assets/assets_manager.h"
#include "source/assets/behavior_asset.h"

#include "source/components/behavior.h"

#include "source/common/abort.h"
#include "register_functions.h"

#include <vector>

struct behaviors::behaviors_manager::implementation
{
    lua_State* L = nullptr;
    int args_counter = 0;
    uint64_t behaviors_id_iterator = 0;

    std::vector<entities::components::behavior*> registered_behaviors;

    void pcall(int r)
    {
        if (r != LUA_OK)
            abort(lua_tostring(L, -1));
    }
};

behaviors::behaviors_manager::behaviors_manager()
{
    impl = new implementation;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    register_functions(L);
    impl->L = L;
}

behaviors::behaviors_manager::~behaviors_manager()
{
    lua_close(impl->L);
}

void behaviors::behaviors_manager::register_behavior_component(entities::components::behavior* comp)
{
    impl->registered_behaviors.push_back(comp);
    comp->call_function(functions::init);
}

void behaviors::behaviors_manager::unregister_behavior_component(entities::components::behavior* comp)
{
    comp->call_function(functions::destroy);
    #define target impl->registered_behaviors
    target.erase(std::remove(target.begin(), target.end(), comp), target.end());
    #undef target
}

void behaviors::behaviors_manager::call_update_functions()
{
    for (auto& comp : impl->registered_behaviors)
        comp->call_function(functions::update);
}

std::string behaviors::behaviors_manager::create_behavior(const std::string& file_path)
{
    std::string name = std::to_string(impl->behaviors_id_iterator);
    impl->behaviors_id_iterator++;

    auto& L = impl->L;
    std::string file = common::assets_manager->get_assets_path() + file_path.c_str();

    impl->pcall(luaL_loadfile(L, file.c_str()));
    lua_newtable(L);
    lua_newtable(L);
    lua_getglobal(L, "_G");
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);
    lua_setfield(L, LUA_REGISTRYINDEX, name.c_str());
    lua_getfield(L, LUA_REGISTRYINDEX, name.c_str());
    lua_setupvalue(L, 1, 1);
    impl->pcall(lua_pcall(L, 0, LUA_MULTRET, 0));

    return name;
}

void behaviors::behaviors_manager::pass_pointer_arg(void* arg)
{
    ++impl->args_counter;
    lua_pushinteger(impl->L, (uint64_t)arg);
}

void behaviors::behaviors_manager::pass_int_arg(uint64_t arg)
{
    ++impl->args_counter;
    lua_pushinteger(impl->L, arg);
}

void behaviors::behaviors_manager::pass_float_arg(float arg)
{
    ++impl->args_counter;
    lua_pushnumber(impl->L, arg);
}

void behaviors::behaviors_manager::prepare_call(behaviors::functions func, assets::behavior* bhv)
{
    impl->args_counter = 0;
    lua_getfield(impl->L, LUA_REGISTRYINDEX, bhv->name.c_str());
    switch (func)
    {
    case behaviors::functions::init:
        lua_getfield(impl->L, -1, "on_init"); break;
    case behaviors::functions::update:
        lua_getfield(impl->L, -1, "on_update"); break;
    case behaviors::functions::destroy:
        lua_getfield(impl->L, -1, "on_destroy"); break;
    }
}

void behaviors::behaviors_manager::call()
{
    auto err = lua_pcall(impl->L, impl->args_counter, 0, 0);
    if (err != LUA_OK)
        abort(lua_tostring(impl->L, -1) + '\n');
}