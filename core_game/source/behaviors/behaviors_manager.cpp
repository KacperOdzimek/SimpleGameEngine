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
        set of all behavior components in the world
        bool indicates if behavior should not be deleted
    */
    std::list<std::pair<entities::components::behavior*, bool>> registered_behaviors;
    /*
        frames_stack
        saves states of the program in certain points of time (function calls) so they can be restored
        also holds used databases alive
    */
    std::list<frame> frames_stack;
    /*
        loaded_modules
        modules loaded via require_module
        key is module relative path
        value is lua registry address of the module
    */
    std::unordered_map<std::string, int> loaded_modules;
};


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
    for (auto& module : impl->loaded_modules)
        luaL_unref(impl->L, LUA_REGISTRYINDEX, module.second);

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
    impl->registered_behaviors.push_back({ comp, true });
}

void behaviors::behaviors_manager::unregister_behavior_component(entities::components::behavior* comp)
{
    auto i = std::find(
        impl->registered_behaviors.begin(),
        impl->registered_behaviors.end(),
        std::pair<entities::components::behavior*, bool>{ comp, true }
    );

    if (impl->registered_behaviors.end() == i) return;

    i->second = false;
}

void behaviors::behaviors_manager::require_module(const std::string& relative_path)
{
    auto itr = impl->loaded_modules.find(relative_path);   
    if (itr == impl->loaded_modules.end())
    {
        std::string global_path = filesystem::get_global_path(relative_path) + ".lua";
        if (luaL_dofile(impl->L, global_path.c_str())) 
        { 
            throw std::exception{(std::string{"Unable to load lua module: " + global_path}).c_str()};
        }
        int module_index = luaL_ref(impl->L, LUA_REGISTRYINDEX);
        impl->loaded_modules.insert({ relative_path, module_index });
    }
    int module_index = impl->loaded_modules.at(relative_path);
    lua_rawgeti(impl->L, LUA_REGISTRYINDEX, module_index);
}

void behaviors::behaviors_manager::call_update_functions()
{
    auto itr = impl->registered_behaviors.begin();
    while (itr != impl->registered_behaviors.end())
    {
        if (!itr->second)
        { 
            itr = impl->registered_behaviors.erase(itr);
            continue;
        }
        itr->first->call_function(functions::update);
        if (!itr->second)
        {
            itr = impl->registered_behaviors.erase(itr);
            continue;
        }
        ++itr;
    }
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

void behaviors::behaviors_manager::pass_nil()
{
    lua_pushnil(impl->L);
}

void behaviors::behaviors_manager::pass_custom_function_args(const int& args_registry_id)
{
    lua_rawgeti(impl->L, LUA_REGISTRYINDEX, args_registry_id);
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

bool behaviors::behaviors_manager::prepare_custom_behavior_function_call(const std::string& func_name, assets::behavior* bhv, const int& args_registry_id)
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

int behaviors::behaviors_manager::call(int args_amount, int results)
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

    int stack_size = lua_gettop(impl->L);

    auto err = lua_pcall(impl->L, args_amount, results, 0);
    if (err != LUA_OK)
        error_handling::crash(error_handling::error_source::core, "[behaviors_manager::call]", lua_tostring(impl->L, -1));

    int actuall_results = lua_gettop(impl->L) - stack_size + args_amount + 1;

    while (actuall_results > results) 
    {
        lua_pop(impl->L, 1);
        results++;
    };

    return std::min(actuall_results, results);
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
