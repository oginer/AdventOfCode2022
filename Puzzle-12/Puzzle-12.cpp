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
	const coord m_finish;

public:
	heuristic_manhattan_distance(const coord& finish)
		:m_finish(finish)
	{}

	unsigned operator() (const coord &pos) const
	{
		return std::abs(pos.first - m_finish.first) + std::abs(pos.second - m_finish.second);
	}
};

constexpr unsigned heuristic_zero(const coord& pos)
{
	return 0;
}

 constexpr unsigned heuristic_distance_to_left(const coord& pos)
{
	return pos.second;
}


std::vector<coord> get_neighbors_part1(const map& m, const coord& pos)
{
	std::vector<coord> result;

	const auto [x, y] = pos;

	if (x > 0 && m[x - 1][y] <= m[x][y] + 1) result.push_back({ x - 1, y });
	if (y > 0 && m[x][y - 1] <= m[x][y] + 1) result.push_back({ x, y - 1 });
	if (x < m.size()-1 && m[x + 1][y] <= m[x][y] + 1) result.push_back({x + 1, y});
	if (y < m[0].length()-1 && m[x][y + 1] <= m[x][y] + 1) result.push_back({x, y + 1});

	return result;
}

std::vector<coord> get_neighbors_part2(const map& m, const coord& pos)
{
	std::vector<coord> result;

	const auto [x, y] = pos;

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
	map m_map;

public:
	is_finish_part2(const map& input)
		:m_map(input)
	{}

	bool operator() (const coord& c) const
	{
		return m_map[c.first][c.second] == 'a';
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
	auto path = aStar(input.m, input.start, is_finish_part1(input.finish), get_neighbors_part1, heuristic_manhattan_distance(input.finish));

	std::cout << "Shortest path length: " << path.size() << std::endl;
}

void part2(const t_input& input)
{
	auto path = aStar(input.m, input.finish, is_finish_part2(input.m), get_neighbors_part2, heuristic_distance_to_left);

	std::cout << "Shortest path length backwards: " << path.size() << std::endl;
}

int main()
{
	Timer t("Total");

	Timer tInput("Input");
	auto input = parse_input(std::ifstream("input.txt"));
	tInput.finish(true);

	std::cout << "start:  (" << input.start.first << ", " << input.start.second << ")" << std::endl;
	std::cout << "finish: (" << input.finish.first << ", " << input.finish.second << ")" << std::endl << std::endl;

	Timer tPart1;
	part1(input);
	tPart1.finish();

	Timer tPart2;
	part2(input);
	tPart2.finish();
}
