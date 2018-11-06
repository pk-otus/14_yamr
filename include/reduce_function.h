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
			result_t result = std::numeric_limits<size_t>::max();
			auto it = data_in.begin();
			while (data_in.end() != it)
			{
				auto temp = (*it).size();
				it = std::adjacent_find(it, data_in.end());
				
				if (data_in.end() == it)
					result = temp;
				else
				{					
					it = std::find_if(it, data_in.end(),
						[&it](const auto& e) { return e.size() > (*it).size(); });
				}
			}
			return result;
		}
	};
}