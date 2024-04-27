#pragma once

namespace graphics_abstraction
{
	enum class texture_type
	{
		unspecified,
		texture_1d, texture_1d_array,
		texture_2d, texture_2d_array,
		texture_3d, 
		texture_cube,
		renderbuffer
	};

	enum class texture_filtering
	{
		nearest, linear
	};

	enum class texture_wraping
	{
		repeat, mirror_repeat,
		clamp_coords, clamp_texture
	};
	
	enum class texture_internal_format
	{
		unspecified,
		depth, 
		depth_stencil,				depth24_stencil8,
		r,							r8, r16,
		rg,							rg8, rg16,
		rgb,						r3_g3_b2, rgb4, rgb5, rgb8, rgb10, rgb12, 
		rgba,						rgba2, rgba4, rgba8, rgba12, rgba16, rgba32f
	};
}