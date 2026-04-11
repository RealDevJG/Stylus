#include "FileReader.h"

#include <iostream>
#include <cassert>

namespace Stylus {

	FileReader::FileReader(std::filesystem::path path)
		: m_Path(path)
	{
		m_File = std::ifstream(path, std::ios::binary | std::ios::ate);
	}

	FileReader::~FileReader()
	{
		m_File.close();
	}

	std::vector<uint32_t> FileReader::Read()
	{
		if (!m_File.good())
		{
			std::cout << "File " << m_Path << " is no good, wrong path maybe?\n";
			return std::vector<uint32_t>();
		}

		const size_t fileSize = static_cast<size_t>(m_File.tellg());

		// File is not valid SPIR-V if not 4-byte aligned
		assert(fileSize % 4 == 0);

		// division to translate bytes from std::ios::binary into uint32_t
		std::vector<uint32_t> buffer;
		buffer.resize(fileSize / 4);

		m_File.seekg(0);
		m_File.read(reinterpret_cast<char*>(buffer.data()), fileSize);

		return buffer;
	}

}
