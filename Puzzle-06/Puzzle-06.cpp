#include <iostream>
#include <string>
#include <fstream>
#include <ranges>
#include <algorithm>

#include "../utils/timer.h"


size_t find_first_unique_chunk(const std::string &input, unsigned n)
{
	for (size_t i = 0; i < input.length()-n-1; ++i)
	{
		std::string substr = input.substr(i, n);
		std::ranges::sort(substr);
		if (std::ranges::adjacent_find(substr) == substr.end()) return i+n;
	}
	return input.length();
}

int main()
{
	Timer t;

	std::string input;
	std::getline(std::ifstream("input.txt"), input);

	std::cout << "Start of packet marker : " << find_first_unique_chunk(input, 4) << std::endl;
	std::cout << "Start of message marker: " << find_first_unique_chunk(input, 14) << std::endl;
}
