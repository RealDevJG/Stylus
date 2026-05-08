#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

namespace Stylus {

	class FileReader
	{
	public:
		explicit FileReader(std::filesystem::path file);

		[[nodiscard]] std::vector<uint32_t> Read() noexcept;
	private:
		std::filesystem::path m_Path;
		std::ifstream m_File;
	};

}
