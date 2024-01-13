#pragma once

struct lua_State;

namespace behaviors
{
	void register_functions(lua_State* L);
}