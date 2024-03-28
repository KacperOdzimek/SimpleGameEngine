#include "behaviors_manager.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/entities/world.h"

#include "source/common/common.h"
#include "source/filesystem/filesystem.h"

#include "source/assets/assets_manager.h"
#include "source/assets/behavior_asset.h"

#include "source/components/behavior.h"

#include "source/common/crash.h"
#include "register_shared.h"

#include "frame.h"

#include <unordered_map>
#include <vector>
#include <list>

constexpr int purge_triggering_dangling_pointers_amount = 20;

struct behaviors::behaviors_manager::implementation
{
    /*
        L
        global state in which all game logic takes place
        -l-
        owned by behaviors_manager
    */
    lua_State* L = nullptr;
    /*
        behaviors_id_iterator
        used to name behaviors inside the lua environement
    */
    uint64_t behaviors_id_iterator = 0;
    /*
        registered_behaviors
        set of all behavior components in the scene
        -l-
        [key]   pointer to the component        (possibly dangling!)
        [value] is component valid and enabled
        -l-
        when component is destroyed, it turns itself off by setting the 
        assigned value to false in unregister_behavior_component function
        this way we don't obscure iteration throught registered_behaviors in call_update_functions
        the set is then purged from inactive pointers
    */
    std::unordered_map<entities::components::behavior*, bool> registered_behaviors;
    /*
        dangling_pointers
        counter of dangling pointers in registered_behaviors
        when it become greater than {purge_triggering_dangling_pointers_amount}
        function {purge_registered_behaviors} is triggered
    */
    int dangling_pointers = 0;

    /*
        frames_stack
        saves states of the program in certain points of time (function calls) so they can be restored
        also holds used databases alive
    */
    std::list<frame> frames_stack;
};

/*
    purge_registered_behaviors
    remove dangling pointers from registered_behaviors
*/
inline void purge_registered_behaviors(std::unordered_map<entities::components::behavior*, bool>& _r_b)
{
    auto iter = _r_b.begin();
    while (iter != _r_b.end()) {
        if (iter->second == false) {
            iter = _r_b.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

/*
    allocate implementation and lua_State
*/
behaviors::behaviors_manager::behaviors_manager()
{
    impl = new implementation;
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    behaviors::register_shared(L);
    impl->L = L;
}

/*
    free implementation and lua_State
*/
behaviors::behaviors_manager::~behaviors_manager()
{
    lua_close(impl->L);
    delete impl;
}

int behaviors::behaviors_manager::create_database()
{
    lua_createtable(impl->L, 0, 0);
    int id = luaL_ref(impl->L, LUA_REGISTRYINDEX);
    return id;
}

void behaviors::behaviors_manager::destroy_database(int id)
{
    luaL_unref(impl->L, LUA_REGISTRYINDEX, id);
}

void behaviors::behaviors_manager::register_behavior_component(entities::components::behavior* comp)
{
    impl->registered_behaviors.insert({ comp, true });
}

void behaviors::behaviors_manager::unregister_behavior_component(entities::components::behavior* comp)
{
    impl->registered_behaviors.at(comp) = false;
    impl->dangling_pointers++;
}

void behaviors::behaviors_manager::call_update_functions()
{
    for (auto& behavior : impl->registered_behaviors)
        if (behavior.second)
            behavior.first->call_function(functions::update);
    if (impl->dangling_pointers > purge_triggering_dangling_pointers_amount)
        purge_registered_behaviors(impl->registered_behaviors);
}

std::string behaviors::behaviors_manager::create_functions_table(const std::string& file_path)
{
    auto& L = impl->L;

    std::string name = std::to_string(impl->behaviors_id_iterator);
    impl->behaviors_id_iterator++;

    std::string path = filesystem::get_global_path(file_path);

    int error;
    error = luaL_loadfile(L, path.c_str());

    if (error != LUA_OK)
        error_handling::crash(error_handling::error_source::core, "[behaviors_manager::create_behavior]", lua_tostring(L, -1));

    lua_newtable(L);
    lua_newtable(L); 
    lua_getglobal(L, "_G"); 
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);
    lua_setfield(L, LUA_REGISTRYINDEX, name.c_str());
    lua_getfield(L, LUA_REGISTRYINDEX, name.c_str());
    lua_setupvalue(L, -2, 1);

    error = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (error != LUA_OK)
        error_handling::crash(error_handling::error_source::core, "[behaviors_manager::create_behavior]", lua_tostring(L, -1));

    return name;
}

void behaviors::behaviors_manager::destroy_functions_table(const std::string& table_name)
{
    lua_pushnil(impl->L);
    lua_setfield(impl->L, LUA_REGISTRYINDEX, table_name.c_str());
}

void behaviors::behaviors_manager::pass_pointer_arg(void* arg)
{
    lua_pushinteger(impl->L, (uint64_t)arg);
    luaL_ref(impl->L, LUA_REGISTRYINDEX);
}

void behaviors::behaviors_manager::pass_entity_arg(std::weak_ptr<entities::entity>* entity)
{
    auto* data = (std::weak_ptr<::entities::entity>*)(lua_newuserdata(impl->L, sizeof(entity)));
    new(data) std::weak_ptr<::entities::entity>(*entity);
    luaL_getmetatable(impl->L, "entity");
    lua_setmetatable(impl->L, -2);
}

void behaviors::behaviors_manager::pass_float_arg(float arg)
{
    lua_pushnumber(impl->L, arg);
}

bool behaviors::behaviors_manager::prepare_behavior_function_call(behaviors::functions func, assets::behavior* bhv)
{
    lua_getfield(impl->L, LUA_REGISTRYINDEX, bhv->name.c_str());
    switch (func)
    {
    case behaviors::functions::init:
        lua_getfield(impl->L, -1, "on_init"); break;
    case behaviors::functions::update:
        lua_getfield(impl->L, -1, "on_update"); break;
    case behaviors::functions::destroy:
        lua_getfield(impl->L, -1, "on_destroy"); break;
    case behaviors::functions::on_collide:
        lua_getfield(impl->L, -1, "on_collide"); break;
    case behaviors::functions::on_overlap:
        lua_getfield(impl->L, -1, "on_overlap"); break;
    }
    lua_remove(impl->L, -2);
    if (lua_isnil(impl->L, -1))
    {
        lua_pop(impl->L, -1);
        return false;
    }
    return true;
}

bool behaviors::behaviors_manager::prepare_custom_behavior_function_call(const std::string& func_name, assets::behavior* bhv)
{
    lua_getfield(impl->L, LUA_REGISTRYINDEX, bhv->name.c_str());
    lua_getfield(impl->L, -1, func_name.c_str());
    lua_insert(impl->L, -lua_gettop(impl->L));
    lua_remove(impl->L, -1);
    if (lua_isnil(impl->L, 1))
    {
        lua_remove(impl->L, 1);
        return false;
    }
    return true;
}

bool behaviors::behaviors_manager::prepare_scene_function_call(behaviors::functions func, assets::scene* sc)
{
    lua_getfield(impl->L, LUA_REGISTRYINDEX, sc->name.c_str());
    switch (func)
    {
    case behaviors::functions::init:
        lua_getfield(impl->L, -1, "on_init"); break;
    case behaviors::functions::update:
        lua_getfield(impl->L, -1, "on_update"); break;
    case behaviors::functions::destroy:
        lua_getfield(impl->L, -1, "on_destroy"); break;
    }
    lua_remove(impl->L, -2);
    if (lua_isnil(impl->L, -1))
    {
        lua_pop(impl->L, -1);
        return false;
    }
    return true;
}

void behaviors::behaviors_manager::call(int args_amount)
{
    if (impl->frames_stack.back().target_object_database.get() == nullptr)
    {
        lua_pushnil(impl->L);
        lua_setglobal(impl->L, "self");
    }
    else
    {
        lua_rawgeti(impl->L, LUA_REGISTRYINDEX, impl->frames_stack.back().target_object_database->table_ref);
        lua_setglobal(impl->L, "self");
    }

    auto err = lua_pcall(impl->L, args_amount, 0, 0);
    if (err != LUA_OK)
        error_handling::crash(error_handling::error_source::core, "[behaviors_manager::call]", lua_tostring(impl->L, -1));
}

void behaviors::behaviors_manager::abort()
{
    luaL_dostring(impl->L, "do return end");
}

void behaviors::behaviors_manager::create_frame(std::shared_ptr<database> database, entities::scene* scene_context)
{
    impl->frames_stack.push_back({});
    impl->frames_stack.back().scene_context = scene_context;
    impl->frames_stack.back().target_object_database = database;
}

void behaviors::behaviors_manager::pop_frame()
{
    impl->frames_stack.pop_back();
    if (impl->frames_stack.size() == 0 || impl->frames_stack.back().target_object_database == nullptr)
    {
        lua_pushnil(impl->L);
        lua_setglobal(impl->L, "self");
    }
    else
    {
        lua_rawgeti(impl->L, LUA_REGISTRYINDEX, impl->frames_stack.back().target_object_database->table_ref);
        lua_setglobal(impl->L, "self");
    }
}

const behaviors::frame* behaviors::behaviors_manager::get_current_frame()
{
    if (impl->frames_stack.size() == 0)
        return nullptr;
    return &impl->frames_stack.back();
}
