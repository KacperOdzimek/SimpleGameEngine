#pragma once
#include "entity.h"

#include "source/assets/scene_asset.h"

#include <vector>
#include <memory>

namespace entities
{
	class entity;
	class world;

	class scene
	{
		friend entity;
		friend world;
	public:
		/*
			entities
			vector of entities owned by the scene
			when scene is deleted it will call entity::kill on every owned entity
			may contain expired pointers
		*/
		std::vector<std::weak_ptr<entity>> entities;
		const uint32_t name;
	protected:
		/*
			frames_since_purged
			as entities can be destroyed due to game logic
			scene::entities can contains expired pointers which can slow down the game
			frames_since_purged counts frames since the last scene::entities purging from expired pointers
			and triggers purge when it reaches value definied in .cpp file
		*/
		int frames_since_purged = 0;
		std::shared_ptr<assets::scene> _scene;
	public:
		scene(uint32_t _name);
		scene(uint32_t _name, std::weak_ptr<assets::scene> __scene);
		void update();
		~scene();
	};
}