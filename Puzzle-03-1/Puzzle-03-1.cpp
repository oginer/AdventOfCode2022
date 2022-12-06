#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <span>
#include <numeric>
#include <ranges>
#include <array>

#include "../utils/timer.h"


unsigned short priority(char c)
{
	if (c <= 'Z')  // is c uppercase?
		return c - 'A' + 27;
	else
		return c - 'a' + 1;
}

template <class T>
std::set<T> intersection(const std::set<T> & items1, const std::set<T> & items2)
{
	std::set<T> repeats;
	std::ranges::set_intersection(items1, items2, std::inserter(repeats, repeats.begin()));
	return repeats;
}

std::vector<std::string> read_input(std::istream& is)
{
	std::vector<std::string> result;
	std::string line;
	while (std::getline(is, line))
		result.push_back(line);
	return result;
}

using t_input = std::vector<std::string>;

t_input read_input(std::istream&& is)
{
	return read_input(is);
}

void part1(const t_input& input)
{
	//std::vector<char> repeats;
	unsigned int result = std::transform_reduce(input.begin(), input.end(), 0u,
		[](const int acc, const char c) -> unsigned int
		{
			return acc + priority(c);
		},
		[](const std::string& s) -> char
		{
			std::set<char> first(s.begin(), s.begin() + s.length() / 2);
			std::set<char> second(s.begin() + s.length() / 2, s.end());
			return *(intersection(first, second).begin());
		}
	);

	std::cout << "Sum of repeats: " << result << std::endl;
}

void part2(const t_input& input)
{
	unsigned sum_badges = 0;

	std::vector<std::array<std::set<char>,3>> group_backsack;

	for (auto it = input.begin(); it != input.end();)
	{
		std::array<std::set<char>, 3> backsack_chunk;

		for (int i = 0; i < 3; ++i)
		{
			backsack_chunk[i] = std::set<char>{ it->begin(), it->end() };
			it++;
		}

		group_backsack.push_back(backsack_chunk);
	}

	sum_badges = std::reduce(group_backsack.begin(), group_backsack.end(), 0u,
		[](const unsigned acc, const std::array<std::set<char>, 3> backsacks) -> unsigned int
		{
			char badge = *(std::reduce(backsacks.begin()+1, backsacks.end(), backsacks[0], intersection<char>).begin());
			return acc + priority(badge);
		}
	);
	std::cout << "Sum of badges = " << sum_badges << std::endl;
}

int main()
{
	Timer t;

	auto input = read_input(std::ifstream("input.txt"));

	part1(input);
	part2(input);
}
