#pragma once

namespace Stylus::Concepts {

	template<typename T>
	concept TupleLikeType = requires
	{
		typename std::tuple_size<std::remove_cvref_t<T>>::type;
	} && (std::tuple_size_v<std::remove_cvref_t<T>> == 2);

}
