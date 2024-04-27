#pragma once
#include <cassert>

namespace graphics_abstraction
{
	enum class draw_types
	{
		array, indexed, instanced_array, instanced_indexed
	};

	enum class primitives
	{
		point,
		line, line_strip,
		triangle, triangle_strip, triangle_fan
	};

	struct draw_args
	{
		draw_types draw_type;
		primitives primitive;

		struct array_draw_args
		{
			uint32_t first_vertex_index;
			uint32_t vertices_amount;
			array_draw_args(uint32_t _first_vertex_index, uint32_t _vertices_amount)
				: first_vertex_index(_first_vertex_index), vertices_amount(_vertices_amount) {};
		};

		struct indexed_draw_args
		{
			uint32_t indices_amount;
			indexed_draw_args(uint32_t _indices_amount)
				: indices_amount(_indices_amount) {};
		};

		struct instanced_array_draw_args
		{
			array_draw_args array_args;
			uint32_t instances_amount;
			instanced_array_draw_args(array_draw_args _array_args, uint32_t _instances_amount)
				: array_args(_array_args), instances_amount(_instances_amount) {};
		};

		struct instanced_indexed_draw_args
		{
			indexed_draw_args indexed_args;
			uint32_t instances_amount;
			instanced_indexed_draw_args(indexed_draw_args _indexed_args, uint32_t _instances_amount)
				: indexed_args(_indexed_args), instances_amount(_instances_amount) {};
		};

		union draw_args_args
		{
			array_draw_args array_draw_args;
			indexed_draw_args indexed_draw_args;
			instanced_array_draw_args instanced_array_draw_args;
			instanced_indexed_draw_args instanced_indexed_draw_args;
			draw_args_args() { array_draw_args = { 0, 0 }; };
		} args;

#define Constructor(expected_draw_type, args_type)														\
		draw_args(draw_types _draw_type, primitives _primitive, args_type _args)						\
			: draw_type(_draw_type), primitive(_primitive)												\
		{																								\
			if (_draw_type != draw_types::expected_draw_type)											\
			{																							\
				assert(true);																			\
			}																							\
			args.args_type = _args;																		\
		}

		Constructor(array,				array_draw_args)
		Constructor(indexed,			indexed_draw_args)
		Constructor(instanced_array,	instanced_array_draw_args)
		Constructor(instanced_indexed,  instanced_indexed_draw_args)

#undef Constructor
	};
}