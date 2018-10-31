#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace yamr
{
	class reduce_function
	{
	public:
		using thread_in_t = std::vector<std::string>;
		using result_t = size_t;

		result_t operator()(const thread_in_t& data_in) const
		{
			result_t result = 0;
			thread_in_t temp;
			auto it = data_in.begin();
			while (data_in.end() != it)
			{
				result = (*it).size();

				it = std::adjacent_find(it, data_in.end());
				if (data_in.end() != it)
				{
					it = std::find_if(it, data_in.end(),
						[&it](const auto& e) { return e.size() > (*it).size(); });
				}
			}
			return result;
		}
	};
}