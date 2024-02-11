#pragma once
#include "entity.h"

#include <vector>
#include <memory>

namespace entities
{
	class entity;

	class scene
	{
		friend entity;
	protected:
		/*
			entities
			vector of entities owned by the scene
			when scene is deleted it will call entity::kill on every owned entity
			may contain expired pointers
		*/
		std::vector<std::weak_ptr<entity>> entities;
		/*
			frames_since_purged
			as entities can be destroyed due to game logic
			scene::entities can contains expired pointers which can slow down the game
			frames_since_purged counts frames since the last scene::entities purging from expired pointers
			and triggers purge when it reaches value definied in .cpp file
		*/
		int frames_since_purged = 0;
	public:
		void update();
		~scene();
	};
}