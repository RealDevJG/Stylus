#pragma once

namespace Stylus::Concepts {

	template<typename Enum>
	concept EnumType = std::is_enum_v<Enum>;

}
