#pragma once
#include <vector>

namespace graphics_abstraction
{
	enum class error_source
	{
		abstraction, implementation
	};

	struct error
	{
		error_source source = error_source::abstraction;
		uint16_t code = 0;
		std::vector<uint8_t> additional_info = {};
		error() {};
		error(error_source _source, uint16_t _code, std::vector<uint8_t> _additional_info) :
			source(_source), code(_code), additional_info(_additional_info)  {};
	};
}