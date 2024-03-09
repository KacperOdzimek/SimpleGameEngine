#include "behaviors_database.h"
#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"

namespace behaviors
{
	database::database()
	{
		table_ref = common::behaviors_manager->create_database();
	}

	database::~database() 
	{ 
		common::behaviors_manager->destroy_database(table_ref);
	}
}