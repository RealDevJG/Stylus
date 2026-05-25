#include "FileReader.h"

#include <expected>
#include <cassert>
#include <iostream>

namespace Stylus {

	FileReader::FileReader(std::filesystem::path path)
		: m_Path(path)
	{
		m_File = std::ifstream(path, std::ios::binary | std::ios::ate);
	}

	std::expected<std::vector<uint32_t>, FileError> FileReader::ReadSpirv() noexcept
	{
		if (!m_File.good())
		{
			std::cerr << "File " << m_Path << " is no good, wrong path maybe?\n";
			return std::unexpected{ FileError::FileNotGood };
		}

		const size_t fileSize = static_cast<size_t>(m_File.tellg());

		if (fileSize % 4 != 0)
		{
			std::cerr << "File " << m_Path << " is not 4-byte aligned; it is invalid SPIR-V\n";
			return std::unexpected{ FileError::FileInvalidSPIRV };
		}

		// Division to translate bytes from std::ios::binary into uint32_t
		std::vector<uint32_t> buffer(fileSize / 4);
		m_File.seekg(0);
		m_File.read(reinterpret_cast<char*>(buffer.data()), fileSize);

		return buffer;
	}

}
