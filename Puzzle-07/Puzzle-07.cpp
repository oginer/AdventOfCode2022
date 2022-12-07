#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <list>
#include <ranges>
#include <execution>

#include "../utils/timer.h"


std::vector<std::string> parse_command(const std::string& str)
{
	std::vector<std::string> result;

	std::ranges::for_each(str | std::views::split(' '),
		[&result](const auto& v)
		{
			result.push_back(std::string(v.begin(), v.end()));
		}
	);

	return result;
}

using t_input = std::unordered_map<std::string, unsigned long>;

std::string dir_up(const std::string& str)
{
	auto pos = str.find_last_of('/');
	if (pos != std::string::npos)
		return str.substr(0, str.find_last_of('/'));
	else
		return "";
}

t_input parse_input(std::istream& input)
{
	t_input result;

	std::string root = "";
	std::string current_path = root;
	result[""] = 0;

	std::list<unsigned long *> parents;

	std::string line;
	while (std::getline(input, line))
	{
		auto command = parse_command(line);

		if (command[0] == "$")
		{
			if (command[1] == "cd")
			{
				if (command[2] == "/")
				{
					current_path = root;
					parents = { };
				}
				else if (command[2] == "..")
				{
					current_path = dir_up(current_path);
					parents.pop_back();
				}
				else
				{
					parents.push_back(&result[current_path]);
					current_path = current_path + "/" + command[2];
				}
			}
		}
		else if (command[0] == "dir")
		{
			result[current_path + "/" + command[1]] = 0;
		}
		else
		{
			try
			{
				unsigned long size = std::stoul(command[0]);
				result.at(current_path) += size;
				for (auto &parent : parents)
				{
					*parent += size;
				}
			}
			catch (...) {}
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
			return p.second <= 100000 ? acc + p.second : acc;
		}
	);

	std::cout << "p1: " << sum << std::endl;
}

int main()
{
	Timer t;

	t_input input = parse_input(std::ifstream("input.txt"));

	part1(input);
//	part2(input);
}
