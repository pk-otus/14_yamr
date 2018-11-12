#pragma once
#include <fstream>
#include <algorithm>
#include <future>
#include <thread>
#include <iostream>
#include <exception>

#include "block_reader.h"
#include "map_function.h"
#include "reduce_function.h"

namespace yamr
{
	template<class F>
	class threaded_converter
	{
	public:
		threaded_converter(size_t num) :
			thread_count(num)
		{}

		virtual ~threaded_converter() = default;

		std::vector<typename F::result_t> get_result()
		{
			using task_t = std::packaged_task<typename F::result_t(typename F::thread_in_t&, size_t)>;

			for (auto i = 0; i < thread_count; ++i)
			{
				auto task = task_t(thread_action);

				v_fut.push_back(task.get_future());
				std::thread(std::move(task), std::ref(get_data(i)), i).detach();
			}

			std::vector<typename F::result_t> result;
			for (auto& f : v_fut)
			{
				result.emplace_back(f.get());
			}
			return result;
		}
	private:
		static typename F::result_t thread_action(typename F::thread_in_t& data_in, size_t num);

		virtual typename F::thread_in_t& get_data(size_t num) = 0;

		const size_t thread_count;
		std::vector<std::future<typename F::result_t>> v_fut;
	};

	template <>
	inline map_function::result_t
		threaded_converter<map_function>::thread_action(map_function::thread_in_t& data_in, size_t num)
	{
		map_function::result_t result;
		try 
		{
			map_function m;
			auto strings = data_in.read_strings(num);
						
			for (const std::string& s : strings)
			{
				auto r = m(s);
				result.insert(result.end(), r.begin(), r.end());
			}
			std::sort(result.begin(), result.end());			
		}
		catch (std::exception& e)
		{
			std::cout << "mapper error: " << e.what() << '\n';
		}
		return result;
	}

	template <>
	inline reduce_function::result_t
		threaded_converter<reduce_function>::thread_action(reduce_function::thread_in_t& data_in, size_t num)
	{
		auto result = std::numeric_limits<reduce_function::result_t>::max();
		try
		{
			std::sort(data_in.begin(), data_in.end(),
				[](const auto& lhs, const auto& rhs)
			{
				int64_t diff = lhs.size() - rhs.size();
				return 0 != diff ? diff < 0 : lhs < rhs;
			});

			result = reduce_function()(data_in);
			auto fout = std::ofstream("reduce_" + std::to_string(num));
			fout << result;
			fout.close();			
		}
		catch (std::exception& e)
		{
			std::cout << "reducer error: " << e.what() << '\n';
		}
		return result;
	}
}
