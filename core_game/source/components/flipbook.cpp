#include "flipbook.h"

#include "source/entities/entity.h"

#include "source/common/crash.h"

#include "source/common/common.h"
#include "source/rendering/flipbooks_manager.h"

using namespace entities;
using namespace components;

flipbook::flipbook(uint32_t _id, std::weak_ptr<assets::flipbook> _flipbook,
	physics::collision_preset preset, uint32_t _flipbook_animation)
	: sprite(_id, _flipbook, preset), component(_id), mesh(_id), collider(_id, preset, sprite_extend / 2.0f),
	current_flipbook_animation(_flipbook_animation)
{
	extend = sprite_extend / 2.0f;
	common::flipbooks_manager->register_flipbook(this);
}

flipbook::~flipbook()
{
	common::flipbooks_manager->unregister_flipbook(this);
}

void flipbook::set_animation(uint32_t new_animation)
{
	auto asset = reinterpret_cast<assets::flipbook*>(rc.textures.at(0).get());

	if (asset->animations.find(new_animation) == asset->animations.end())
		error_handling::crash(error_handling::error_source::core, 
			"[flipbook::set_animation]", "Trying to use non-existent animation.");

	if (new_animation != current_flipbook_animation)
		playback_position = 0.0f;

	current_flipbook_animation = new_animation;

	mark_pipeline_dirty();
}

void flipbook::pass_transformation(rendering::transformations_buffer_iterator& tbi)
{
	if (visible)
	{
		glm::vec2 location = owner->get_location();

		tbi.put(std::move(location.x));
		tbi.put(std::move(location.y));

		tbi.put(sprite_extend.x);
		tbi.put(sprite_extend.y);

		tbi.put(owner->layer);
		tbi.put(reinterpret_cast<assets::flipbook*>(rc.textures.at(0).get())
			->get_sprite_id_at_position(current_flipbook_animation, playback_position));
	}
}

void flipbook::move_playback(float time)
{
#define anim reinterpret_cast<assets::flipbook*>(rc.textures.at(0).get())->animations.at(current_flipbook_animation)

	playback_position += time;
	if (playback_position < 0)
		playback_position = 0;

	else if (playback_position >= (anim.frames.size() / anim.frames_per_second))
		if (looping)
			playback_position = 0;
		else
			playback_position = (anim.frames.size() / anim.frames_per_second) - (0.5 / anim.frames_per_second);

	set_sprite_id(reinterpret_cast<assets::flipbook*>(rc.textures.at(0).get())->get_sprite_id_at_position(
		current_flipbook_animation, playback_position));

	mark_pipeline_dirty();

#undef anim
}