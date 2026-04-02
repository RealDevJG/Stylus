#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

namespace Stylus {

	class FileReader
	{
	public:
		FileReader(std::filesystem::path file);
		~FileReader();

		[[nodiscard]] std::vector<uint32_t> Read();
	private:
		std::filesystem::path m_Path;
		std::ifstream m_File;
	};

}
