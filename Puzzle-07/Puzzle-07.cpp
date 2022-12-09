#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <string>
#include <ranges>
#include <numeric>

#include "../utils/timer.h"
#include "../utils/parse_helper.h"


using t_input = std::vector<unsigned long>;

std::vector<unsigned long> parse_input(std::istream& input)
{
	t_input result;

	result.push_back(0);
	size_t current_dir = 0;

	std::list<size_t> parents;

	std::string line;
	while (std::getline(input, line))
	{
		auto command = parse_command(line);

		if (command[0] == "$" && command[1] == "cd")
		{
			if (command[2] == "/")
			{
				parents.erase(parents.begin(), parents.end());
			}
			else if (command[2] == "..")
			{
				current_dir = parents.back();
				parents.pop_back();
			}
			else
			{
				parents.push_back(current_dir);
				current_dir = result.size();
				result.push_back(0);
			}
		}
		else if (std::isdigit(command[0][0]))
		{
			unsigned long size = std::stoul(command[0]);
			result[current_dir] += size;
			for (auto& parent : parents)
			{
				result[parent] += size;
			}
		}
	}

	return result;
}

t_input parse_input(std::istream&& input)
{
	return parse_input(input);
}

void part1(const t_input& input)
{
	auto sum = std::reduce(input.begin(), input.end(), 0ul,
		[](unsigned long acc, const auto& p)
		{
			return p <= 100000 ? acc + p : acc;
		}
	);

	std::cout << "Sum of all folders smaller than 100000:   " << sum << std::endl;
}

void part2(const t_input& input)
{
	constexpr unsigned long disk_size = 70000000;
	constexpr unsigned long needed_free_space = 30000000;

	const unsigned long need_to_free = input[0] - (disk_size - needed_free_space);

	auto min = std::ranges::min(input | std::views::filter([need_to_free](unsigned long a) {return a >= need_to_free; }));

	std::cout << "Smallest folder to delete:                " << min << std::endl;
}


int main()
{
	Timer t;

	t_input input = parse_input(std::ifstream("input.txt"));

	std::cout << "Disk usage:                               " << input[0] << std::endl;

	part1(input);
	part2(input);
}
