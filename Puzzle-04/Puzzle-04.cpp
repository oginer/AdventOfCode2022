#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <array>

using t_assigment = std::array<std::array<unsigned, 2>, 2 > ;
using t_input = std::vector<t_assigment>;

t_input parse_input(std::istream& is)
{
	t_input result;
	std::string line;

	while (std::getline(is, line))
	{
		std::stringstream ss(line);
		t_assigment data;
		char c;

		ss >> data[0][0] >> c >> data[0][1] >> c >> data[1][0] >> c >> data[1][1];

		result.push_back(data);
	}

	return result;
}

t_input parse_input(std::istream&& is)
{
	return parse_input(is);
}

void part1(t_input input)
{
	unsigned result = std::reduce(input.begin(), input.end(), 0u,
		[](const unsigned acc, const auto& data) -> unsigned
		{
			// One elves fully overlaps the other one both his assigments are between the other elf assigments.
			if ((data[0][0] >= data[1][0] && data[0][1] <= data[1][1]) ||
			data[1][0] >= data[0][0] && data[1][1] <= data[0][1])
				return acc + 1;
			else
				return acc;
		}
	);

	std::cout << "Useless elves: " << result << std::endl;
}

void part2(t_input input)
{
	unsigned result = std::reduce(input.begin(), input.end(), 0u,
		[](const unsigned acc, const auto& data) -> unsigned
		{
			// It's easier to test when assignments don't overlap: when either
			//   - both values are less than the other elve low value or
			//   - both values are greater than the other elve high value
			if ((data[0][0] < data[1][0] && data[0][1] < data[1][0]) ||
			data[0][0] > data[1][1] && data[0][1] > data[1][1])
				return acc;
			else
				return acc + 1;
		}
	);

	std::cout << "Mostly useless elves: " << result << std::endl;
}


int main()
{
	auto input = parse_input(std::ifstream("input.txt"));
	part1(input);
	part2(input);
}
