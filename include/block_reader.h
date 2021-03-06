#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <exception>

namespace yamr
{
	class block_reader
	{
	public:
		block_reader(const std::string& nm, size_t block_count) :
			fname(nm)
		{
			offset_vector_t string_begins;

			std::ifstream in(fname, std::ios::binary);
			if (in.fail())
				throw std::invalid_argument("cannot open data file");

			std::string unused;
			do
			{
				string_begins.push_back(in.tellg());
			} 
			while (std::getline(in, unused) && !in.eof());

			single_block_size = string_begins.size() / block_count + 1;

			for (size_t i = 0; i < string_begins.size(); i += single_block_size)
			{
				offsets.push_back(string_begins[i]);
			}
			in.close();
		}
		std::vector<std::string> read_strings(size_t block_number) const
		{
			std::vector<std::string> result;
			if (offsets.size() > block_number)
			{
				std::ifstream in(fname, std::ios::binary);
				std::string line;
				in.seekg(offsets[block_number]);
				while (block_number + 1 == offsets.size() || in.tellg() < offsets[block_number + 1])
				{
					if (in.eof()) break;
					std::getline(in, line);
					line.erase(std::remove_if(line.begin(), line.end(), 
							[](const char c) { return '\n' == c || '\r' == c; }), line.end());
					std::transform(line.begin(), line.end(), line.begin(), tolower);
					result.push_back(line);
				}
				in.close();
			}
			return result;
		}
	private:
		using offset_vector_t = std::vector<std::streamoff>;

		const std::string fname;
		size_t single_block_size;
		offset_vector_t offsets;
	};
}
