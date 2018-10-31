#pragma once
#include "threaded_converter.h"

namespace yamr
{
	template<class F>
	class reducer : public threaded_converter<F>
	{
	public:
		reducer(const std::vector<typename F::thread_in_t>& map_result, size_t N) :
			threaded_converter<F>(N),
			reducer_data(N)
		{
			for (auto& r : map_result)
			{
				for (const auto& str : r)
				{
					reducer_data[str.size() % N].emplace_back(str);
				}
			}
		}
	private:
		typename F::thread_in_t& get_data(size_t num) override { return reducer_data[num]; }

		std::vector<typename F::thread_in_t> reducer_data;
	};
}