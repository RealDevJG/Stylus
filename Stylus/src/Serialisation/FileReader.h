#pragma once

#include <expected>
#include <filesystem>
#include <fstream>
#include <vector>

namespace Stylus {

	enum class FileError : uint8_t
	{
		FileNotGood = 0,
		FileInvalidSPIRV
	};

	class FileReader
	{
	public:
		explicit FileReader(std::filesystem::path file);
		[[nodiscard]] std::expected<std::vector<uint32_t>, FileError> ReadSpirv() noexcept;
	private:
		std::filesystem::path m_Path;
		std::ifstream m_File;
	};

}
