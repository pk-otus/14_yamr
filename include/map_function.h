#pragma once
#include <vector>
#include <string>

namespace yamr
{
	class block_reader;

	class map_function
	{
	public:
		using thread_in_t = const block_reader;
		using result_t = std::vector<std::string>;

		result_t operator()(const std::string& data_in) const
		{
			result_t result;

			for (size_t i = 1; i <= data_in.size(); ++i)
			{
				result.emplace_back(data_in.substr(0, i));
			}

			return result;
		}
	};
}