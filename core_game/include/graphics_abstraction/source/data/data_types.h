#pragma once

namespace graphics_abstraction
{
	enum class data_type
	{
		Int, Float, Bool, Byte,
		vec2, vec3, vec4,
		mat2x2, mat2x3, mat2x4,
		mat3x2, mat3x3, mat3x4,
		mat4x2, mat4x3, mat4x4
	};

	enum class input_data_type
	{
		unspecified,
		byte, unsigned_byte,  usingned_byte_r3_g3_b2,
		usigned_short, Short, usingned_short_r5_g6_b5,  usingned_short_r4_g4_b4_a4,
		unsingned_int, Int,	  unsigned_int_r8_g8_r8_a8, unsigned_int_r10_g10_b10_a2,
		half_float, Float,
	};
}