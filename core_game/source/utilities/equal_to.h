#pragma once

namespace utilities
{
	template<class T>
	struct equal_to
	{
		constexpr bool operator()(const T& lhs, const T& rhs) const
		{
			return lhs == rhs;
		}
	};
}