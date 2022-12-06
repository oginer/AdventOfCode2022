#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <array>

#include "../utils/timer.h"


struct assignment
{
	struct
	{
		unsigned start;
		unsigned end;
	} elf1, elf2;
};

using t_input = std::vector<assignment>;

t_input parse_input(std::istream& is)
{
	t_input result;
	std::string line;

	while (std::getline(is, line))
	{
		std::stringstream ss(line);
		assignment data;
		char c;

		ss >> data.elf1.start >> c >> data.elf1.end >> c >> data.elf2.start >> c >> data.elf2.end;

		result.push_back(data);
	}

	return result;
}

t_input parse_input(std::istream&& is)
{
	return parse_input(is);
}

void part1(const t_input &input)
{
	unsigned result = std::reduce(input.begin(), input.end(), 0u,
		[](const unsigned acc, const assignment& data) -> unsigned int
		{
			// One elves fully overlaps the other one when both of his assigments are between the other elf assigments.
			return acc +
				(((data.elf1.start >= data.elf2.start && data.elf1.end <= data.elf2.end) ||
				data.elf2.start >= data.elf1.start && data.elf2.end <= data.elf1.end)
				? 1 : 0);
		}
	);

	std::cout << "Useless elves: " << result << std::endl;
}

void part2(const t_input &input)
{
	unsigned result = std::reduce(input.begin(), input.end(), 0u,
		[](const unsigned acc, const auto& data) -> unsigned
		{
			// It's easier to test when assignments don't overlap, which is when either:
			//   - both values of one elve are less than the other elve's low value or
			//   - both values of one elve are greater than the other elve's high value
			return acc +
				((data.elf1.end < data.elf2.start) || data.elf1.start > data.elf2.end ? 0 : 1);
		}
	);

	std::cout << "Mostly useless elves: " << result << std::endl;
}


int main()
{
	Timer t;

	auto input = parse_input(std::ifstream("input.txt"));
	part1(input);
	part2(input);
}
