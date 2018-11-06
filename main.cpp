#include <iostream>
#include <string>

#include "map_function.h"
#include "reduce_function.h"

#include "mapper.h"
#include "reducer.h"

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: yamr <source file> <MAP streams count> <REDUCE streams count>";
		return 1;
	}
	std::string fname = argv[1];
	size_t M = std::stoi(argv[2]);
	size_t R = std::stoi(argv[3]);	
	
	auto m = yamr::mapper<yamr::map_function>(fname, M).get_result();
	auto r = yamr::reducer<yamr::reduce_function>(m, R).get_result();
	yamr::reduce_function::result_t min_prefix = *std::min_element(r.begin(), r.end());

	std::cout << "Found min prefix : " << min_prefix;
	if (std::numeric_limits<size_t>::max() == min_prefix)
		std::cout << " (not found)";

	std::cout << std::endl;
	return 0;
}
