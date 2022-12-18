#include <vector>
#include <array>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <cassert>

#include "../utils/parse_helper.h"
#include "../utils/timer.h"


struct cube
{
	int x, y, z;

	cube operator+ (const cube& c) const
	{
		return { x + c.x, y + c.y, z + c.z };
	}

	bool operator== (const cube&) const = default;
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

constexpr size_t side = 25;

unsigned fill(int area[side][side][side], const cube &start, short val)
{
	std::stack<cube> open;
	open.push(start);

	unsigned long count = 0;

	while (!open.empty())
	{
		auto current = open.top();
		open.pop();

		if (area[current.x][current.y][current.z] == 0)
		{
			area[current.x][current.y][current.z] = val;

			std::array<cube, 6> adjacents
			{
				current + cube{1,0,0},
				current + cube{-1,0,0},
				current + cube{0,1,0},
				current + cube{0,-1,0},
				current + cube{0,0,1},
				current + cube{0,0,-1}
			};

			for (auto& adj : adjacents)
			{
				if (adj.x >= 0 && adj.x < side &&
					adj.y >= 0 && adj.y < side &&
					adj.z >= 0 && adj.z < side)
				{
					int adj_val = area[adj.x][adj.y][adj.z];
					if (adj_val == 1) ++count;
					if (adj_val == 0) open.push(adj);
				}
			}
		}
	};

	return count;
}

void part2(const std::vector<cube>& input)
{

	int area[side][side][side];
	std::memset(area, 0, sizeof(area));

	unsigned long count = 0;

	for (auto& c : input)
	{
		assert(c.x+2 < side);
		assert(c.y+2 < side);
		assert(c.z+2 < side);
		area[c.x+1][c.y+1][c.z+1] = 1;
	}

	count = fill(area, { 0,0,0 }, 2);

	std::cout << "Surface area: " << count << std::endl;
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
				c + cube{-1,0,0},
				c + cube{0,1,0},
				c + cube{0,-1,0},
				c + cube{0,0,1},
				c + cube{0,0,-1}
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
	Timer t("Total");

	Timer ti("Input");
	auto input = parse_input(std::ifstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t2.finish();
}
