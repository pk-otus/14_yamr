#pragma once
#include "threaded_converter.h"

namespace yamr
{
	template<class F>
	class mapper : public threaded_converter<F>
	{
	public:
		mapper(const std::string& fname, size_t N) :
			threaded_converter<F>(N),
			blocks(fname, N)
		{

		}
	private:
		typename F::thread_in_t& get_data(size_t num) override { return blocks; }

		const block_reader blocks;
	};
}