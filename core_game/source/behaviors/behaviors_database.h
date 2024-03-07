#pragma once

namespace behaviors
{
	class behaviors_manager;

	struct database
	{
	friend behaviors::behaviors_manager;
		database();
		~database();
	private:
		int table_ref;
	};
}