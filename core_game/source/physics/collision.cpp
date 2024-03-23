#include "collision.h"

#define set_bit(source, distance_from_end, value) ((source) |= ((value) << (distance_from_end)))
#define get_bit(source, distance_from_end) (((source) >> (distance_from_end)) & 1)

namespace physics
{
	collision_preset gen_flag(uint8_t body_type, std::array<collision_response, 14> responses)
	{
		collision_preset flag = body_type;
		flag = flag << (sizeof(collision_preset) * 8 - collision_preset_body_type_size);
		for (int i = 0; i < 14; i++)
		{
			set_bit(flag, (i * 2),		get_bit(uint8_t(responses[i]), 0));
			set_bit(flag, (i * 2 + 1),	get_bit(uint8_t(responses[i]), 1));
		}

		return flag;
	}

	collision_response get_response_type(const collision_preset& f1, const collision_preset& f2)
	{
	#define channel_id_at_2 (f1 >> (sizeof(collision_preset) * 8 - collision_preset_body_type_size))
	#define channel_id_at_1 (f2 >> (sizeof(collision_preset) * 8 - collision_preset_body_type_size))

		uint8_t resp1 = 0;
		set_bit(resp1, 0, get_bit(f1, channel_id_at_1 * 2));
		set_bit(resp1, 1, get_bit(f1, channel_id_at_1 * 2 + 1));

		uint8_t resp2 = 0;
		set_bit(resp2, 0, get_bit(f2, channel_id_at_2 * 2));
		set_bit(resp2, 1, get_bit(f2, channel_id_at_2 * 2 + 1));

	#undef channel_id_at_2
	#undef channel_id_at_2

		if (resp1 + resp2 == 4)
			return collision_response::collide;
		else if (resp1 == 0 || resp2 == 0)
			return collision_response::ignore;
		return collision_response::overlap;
	}
}