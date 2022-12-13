#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include "../utils/timer.h"

#include "astar.h"


class heuristic_manhattan_distance
{
private:
	coord finish;

public:
	heuristic_manhattan_distance(const coord& f)
		:finish(f)
	{}

	double operator() (coord pos) const
	{
		return std::abs(pos.first - finish.first) + std::abs(pos.second - finish.second);
	}
};

class heuristic_zero
{
private:

public:
	heuristic_zero()
	{}

	double operator() (coord pos) const
	{
		return 0;
	}
};

std::vector<coord> get_neighbors_part1(const map& m, const coord& pos)
{
	std::vector<coord> result;

	int x = pos.first, y = pos.second;

	if (x > 0 && m[x - 1][y] <= m[x][y] + 1) result.push_back({ x - 1, y });
	if (y > 0 && m[x][y - 1] <= m[x][y] + 1) result.push_back({ x, y - 1 });
	if (x < m.size()-1 && m[x + 1][y] <= m[x][y] + 1) result.push_back({x + 1, y});
	if (y < m[0].length()-1 && m[x][y + 1] <= m[x][y] + 1) result.push_back({x, y + 1});

	return result;
}

std::vector<coord> get_neighbors_part2(const map& m, const coord& pos)
{
	std::vector<coord> result;

	int x = pos.first, y = pos.second;

	if (x > 0 && m[x - 1][y] >= m[x][y] - 1) result.push_back({ x - 1, y });
	if (y > 0 && m[x][y - 1] >= m[x][y] - 1) result.push_back({ x, y - 1 });
	if (x < m.size() - 1 && m[x + 1][y] >= m[x][y] - 1) result.push_back({ x + 1, y });
	if (y < m[0].length() - 1 && m[x][y + 1] >= m[x][y] - 1) result.push_back({ x, y + 1 });

	return result;
}


class is_finish_part1
{
private:
	coord finish;

public:
	is_finish_part1(const coord &c)
		:finish(c)
	{}

	bool operator() (const coord& c) const
	{
		return c == finish;
	}
};

class is_finish_part2
{
private:
	map m;

public:
	is_finish_part2(const map& input)
		:m(input)
	{}

	bool operator() (const coord& c) const
	{
		return m[c.first][c.second] == 'a';
	}
};


struct t_input
{
	map m;
	coord start;
	coord finish;
};

t_input parse_input(std::istream& input)
{
	t_input result;
	std::string line;
	int row = 0;
	
	while (std::getline(input, line))
	{
		auto f_start = line.find_first_of('S');
		if (f_start != std::string::npos)
		{
			line[f_start] = 'a';
			result.start = { row, static_cast<int>(f_start) };
		}
		auto f_end = line.find_first_of('E');
		if (f_end != std::string::npos)
		{
			line[f_end] = 'z';
			result.finish = { row, static_cast<int>(f_end) };
		}
		result.m.push_back(line);
		row++;
	}

	return result;
}

t_input parse_input(std::istream&& input)
{
	return parse_input(input);
}

void part1(const t_input& input)
{
	auto path = aStar(input.m, input.start, is_finish_part1(input.finish), get_neighbors_part1, heuristic_manhattan_distance{input.finish});

	map  out = input.m;

	std::cout << "Shortest path size: " << path.size() << std::endl;
}

void part2(const t_input& input)
{
	auto path = aStar(input.m, input.finish, is_finish_part2(input.m), get_neighbors_part2, heuristic_zero{});

	map  out = input.m;

	std::cout << "Shortest path size backwards: " << path.size() << std::endl;
}

int main()
{
	Timer t;

	auto input = parse_input(std::ifstream("input.txt"));

	std::cout << "start:  (" << input.start.first << ", " << input.start.second << ")" << std::endl;
	std::cout << "finish: (" << input.finish.first << ", " << input.finish.second << ")" << std::endl;

	part1(input);
	part2(input);
}