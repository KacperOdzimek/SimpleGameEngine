#pragma once
#include "graphics_abstraction.h"
#include "opengl.h"

namespace graphics_abstraction
{
	namespace implementations
	{
		namespace opengl_3_3_api
		{
			namespace internal
			{
				inline int primitive_to_opengl(graphics_abstraction::primitives primitive)
				{
					switch (primitive)
					{
					case graphics_abstraction::primitives::point:
						return GL_POINTS;
					case graphics_abstraction::primitives::line:
						return GL_LINES;
					case graphics_abstraction::primitives::line_strip:
						return GL_LINE_STRIP;
					case graphics_abstraction::primitives::triangle:
						return GL_TRIANGLES;
					case graphics_abstraction::primitives::triangle_strip:
						return GL_TRIANGLE_STRIP;
					case graphics_abstraction::primitives::triangle_fan:
						return GL_TRIANGLE_FAN;
					default:
						return GL_POINTS;
					}
				}

				inline int data_type_to_opengl(graphics_abstraction::data_type data_type)
				{
					switch (data_type)
					{
					case graphics_abstraction::data_type::Int:
						return GL_INT;
					case graphics_abstraction::data_type::Float:
						return GL_FLOAT;
					case graphics_abstraction::data_type::Bool:
						return GL_BOOL;
					case graphics_abstraction::data_type::Byte:
						return GL_BYTE;
					case graphics_abstraction::data_type::vec2:
						return GL_FLOAT_VEC2;
					case graphics_abstraction::data_type::vec3:
						return GL_FLOAT_VEC3;
					case graphics_abstraction::data_type::vec4:
						return GL_FLOAT_VEC4;
					default:
						return 0;
					}
				}

				inline int buffer_type_to_opengl(graphics_abstraction::buffer_type bt)
				{
					switch (bt)
					{
					case graphics_abstraction::buffer_type::vertex:
						return GL_ARRAY_BUFFER;
					case graphics_abstraction::buffer_type::indicies:
						return GL_ELEMENT_ARRAY_BUFFER;
					case graphics_abstraction::buffer_type::instanced:
						return GL_ARRAY_BUFFER;
					case graphics_abstraction::buffer_type::other:
						return GL_ARRAY_BUFFER;
					default:
						return GL_ARRAY_BUFFER;
					}
				}

				inline int texture_type_type_to_opengl(graphics_abstraction::texture_type tt)
				{
					switch (tt)
					{
					case graphics_abstraction::texture_type::texture_1d:
						return GL_TEXTURE_1D;
					case graphics_abstraction::texture_type::texture_1d_array:
						return GL_TEXTURE_1D_ARRAY;
					case graphics_abstraction::texture_type::texture_2d:
						return GL_TEXTURE_2D;
					case graphics_abstraction::texture_type::texture_2d_array:
						return GL_TEXTURE_2D_ARRAY;
					case graphics_abstraction::texture_type::texture_3d:
						return GL_TEXTURE_3D;
					case graphics_abstraction::texture_type::texture_cube:
						return GL_TEXTURE_CUBE_MAP;
					case graphics_abstraction::texture_type::renderbuffer:
						return GL_RENDERBUFFER;
					default:
						return GL_TEXTURE_2D_ARRAY;
					}
				}

				inline int texture_wraping_type_to_opengl(graphics_abstraction::texture_wraping tw)
				{
					switch (tw)
					{
					case graphics_abstraction::texture_wraping::repeat:
						return GL_REPEAT;
					case graphics_abstraction::texture_wraping::mirror_repeat:
						return GL_MIRRORED_REPEAT;
					case graphics_abstraction::texture_wraping::clamp_coords:
						return GL_CLAMP_TO_EDGE;
					case graphics_abstraction::texture_wraping::clamp_texture:
						return GL_CLAMP_TO_BORDER;
					default:
						return GL_REPEAT;
					}
				}

				inline int texture_filtering_type_to_opengl(bool using_mipmap,
					graphics_abstraction::texture_filtering filter,
					graphics_abstraction::texture_filtering mipmap = graphics_abstraction::texture_filtering::linear)
				{
					if (!using_mipmap)
					{
						if (filter == graphics_abstraction::texture_filtering::linear)
							return GL_LINEAR;
						return GL_NEAREST;
					}
					else
					{
						bool mag_linear = filter == graphics_abstraction::texture_filtering::linear;
						bool mipmap_linear = mipmap == graphics_abstraction::texture_filtering::linear;

						if (mag_linear && mipmap_linear)
							return GL_LINEAR_MIPMAP_LINEAR;
						else if (!mag_linear && !mipmap_linear)
							return GL_NEAREST_MIPMAP_NEAREST;
						else if (mag_linear && !mipmap_linear)
							return GL_LINEAR_MIPMAP_NEAREST;
						else if (!mag_linear && mipmap_linear)
							return GL_NEAREST_MIPMAP_LINEAR;
					}
					return 0;
				}

				inline int texture_internal_format_to_opengl(graphics_abstraction::texture_internal_format tif)
				{
					switch (tif)
					{
					case graphics_abstraction::texture_internal_format::unspecified:
						return GL_RGB;
					case graphics_abstraction::texture_internal_format::depth:
						return GL_DEPTH_COMPONENT;
					case graphics_abstraction::texture_internal_format::depth_stencil:
						return GL_DEPTH_STENCIL;
					case graphics_abstraction::texture_internal_format::depth24_stencil8:
						return GL_DEPTH24_STENCIL8;
					case graphics_abstraction::texture_internal_format::r:
						return GL_RED;
					case graphics_abstraction::texture_internal_format::r8:
						return GL_R8;
					case graphics_abstraction::texture_internal_format::r16:
						return GL_R16;
					case graphics_abstraction::texture_internal_format::rg:
						return GL_RG;
					case graphics_abstraction::texture_internal_format::rg8:
						return GL_RG8;
					case graphics_abstraction::texture_internal_format::rg16:
						return GL_RG16;
					case graphics_abstraction::texture_internal_format::rgb:
						return GL_RGB;
					case graphics_abstraction::texture_internal_format::r3_g3_b2:
						return GL_R3_G3_B2;
					case graphics_abstraction::texture_internal_format::rgb4:
						return GL_RGB4;
					case graphics_abstraction::texture_internal_format::rgb5:
						return GL_RGB5;
					case graphics_abstraction::texture_internal_format::rgb8:
						return GL_RGB8;
					case graphics_abstraction::texture_internal_format::rgb10:
						return GL_RGB10;
					case graphics_abstraction::texture_internal_format::rgb12:
						return GL_RGB12;
					case graphics_abstraction::texture_internal_format::rgba:
						return GL_RGBA;
					case graphics_abstraction::texture_internal_format::rgba2:
						return GL_RGBA2;
					case graphics_abstraction::texture_internal_format::rgba4:
						return GL_RGBA4;
					case graphics_abstraction::texture_internal_format::rgba8:
						return GL_RGBA8;
					case graphics_abstraction::texture_internal_format::rgba12:
						return GL_RGBA12;
					case graphics_abstraction::texture_internal_format::rgba16:
						return GL_RGBA16;
					case graphics_abstraction::texture_internal_format::rgba32f:
						return GL_RGBA32F;
					default:
						return GL_RGB;
					}
				}

				inline int input_data_type_to_opengl(graphics_abstraction::input_data_type idt)
				{
					switch (idt)
					{
					case graphics_abstraction::input_data_type::unspecified:
						return GL_UNSIGNED_BYTE;
					case graphics_abstraction::input_data_type::byte:
						return GL_BYTE;
					case graphics_abstraction::input_data_type::unsigned_byte:
						return GL_UNSIGNED_BYTE;
					case graphics_abstraction::input_data_type::usingned_byte_r3_g3_b2:
						return GL_UNSIGNED_BYTE_3_3_2;
					case graphics_abstraction::input_data_type::usigned_short:
						return GL_UNSIGNED_SHORT;
					case graphics_abstraction::input_data_type::Short:
						return GL_SHORT;
					case graphics_abstraction::input_data_type::usingned_short_r5_g6_b5:
						return GL_UNSIGNED_SHORT_5_6_5;
					case graphics_abstraction::input_data_type::usingned_short_r4_g4_b4_a4:
						return GL_UNSIGNED_SHORT_4_4_4_4;
					case graphics_abstraction::input_data_type::unsingned_int:
						return GL_UNSIGNED_INT;
					case graphics_abstraction::input_data_type::Int:
						return GL_INT;
					case graphics_abstraction::input_data_type::unsigned_int_r8_g8_r8_a8:
						return GL_UNSIGNED_INT_8_8_8_8;
					case graphics_abstraction::input_data_type::unsigned_int_r10_g10_b10_a2:
						return GL_UNSIGNED_INT_10_10_10_2;
					case graphics_abstraction::input_data_type::half_float:
						return GL_HALF_FLOAT;
					case graphics_abstraction::input_data_type::Float:
						return GL_FLOAT;
					default:
						return GL_UNSIGNED_BYTE;
					}
				}
			}
		}
	}
}