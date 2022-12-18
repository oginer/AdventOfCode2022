#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

#include "../utils/parse_helper.h"


struct cube
{
	int x, y, z;

	cube operator+ (const cube& c) const
	{
		return { x + c.x, y + c.y, z + c.z };
	}
	cube operator- (const cube& c) const
	{
		return { x - c.x, y - c.y, z - c.z };
	}

	bool operator== (const cube&) const = default;

//	cube(int _x, int _y, int _z)
//		:x(_x), y(_y), z(_z)
//	{}
};

struct hash_cube
{
	size_t operator() (const cube& c) const
	{
		return std::hash<int>{}(c.x) ^ std::hash<int>{}(c.y << 8) ^ std::hash<int>{}(c.z << 16);
	}
};

std::vector<cube> parse_input(std::istream& input)
{
	std::vector<cube> result;
	std::string line;

	while (std::getline(input, line) && line != "")
	{
		cube c;
		auto command = parse_command(line, ',');
		parse_number(command[0], c.x);
		parse_number(command[1], c.y);
		parse_number(command[2], c.z);
		result.push_back(c);
	}

	return result;
}

void part1(const std::vector<cube>& input)
{
	std::unordered_set<cube, hash_cube> cubes;

	unsigned long long count = 0;

	for (auto& c : input)
	{
		if (cubes.count(c) == 0)
		{
			std::array<cube, 6> adjacents
			{
				c + cube{1,0,0},
				c - cube{1,0,0},
				c + cube{0,1,0},
				c - cube{0,1,0},
				c + cube{0,0,1},
				c - cube{0,0,1}
			};

			count += 6;
			for (auto& adj : adjacents)
			{
				count -= 2 * cubes.count(adj);
			}

			cubes.insert(c);
		}
	}

	std::cout << "Surface area: " << count << std::endl;
}

std::vector<cube> parse_input(std::istream&& input)
{
	return parse_input(input);
}

int main()
{
	auto input = parse_input(std::ifstream("input.txt"));

	part1(input);
}
