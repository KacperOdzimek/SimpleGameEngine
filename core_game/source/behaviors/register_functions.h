#pragma once
class lua_State;

namespace behaviors
{
	class database;
	namespace internal
	{
		extern database* active_database;
	}
	void register_functions(lua_State* L);
}