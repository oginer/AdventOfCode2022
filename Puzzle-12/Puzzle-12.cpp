#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <string>

#include "../utils/timer.h"


using map = std::vector<std::string>;
using coord = std::pair<int, int>;

struct node
{
	double f{ std::numeric_limits<double>::infinity() };
	double g{ std::numeric_limits<double>::infinity() };
	double h{ std::numeric_limits<double>::infinity() };
	coord parent;
};

double heuristic(coord pos, coord finish)
{
	return std::abs(pos.first - finish.first) + std::abs(pos.second - finish.second);
//	return std::sqrt((pos.first - finish.first)*(pos.first - finish.first) + (pos.second - finish.second)*(pos.second - finish.second));
}

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

std::vector<coord> generate_path(const std::vector<std::vector<node>>& nodes, const coord& end, const coord& start)
{
	std::vector<coord> path;
	coord current = end;
	path.push_back(end);
	do
	{
		current = nodes[current.first][current.second].parent;
		path.push_back(current);
	} while (current != start);
	return path;
}

std::vector<coord> aStar(map m, coord start, coord finish, std::vector<coord> (*get_neighbors)(const map&, const coord&))
{
	std::priority_queue<std::pair<double, coord>, std::vector<std::pair<double, coord>>, std::greater<std::pair<double, coord>>> open_nodes;
	std::vector<std::vector<bool>> closed_nodes{ m.size(), std::vector<bool>(m[0].size(), false)};
	std::vector<std::vector<node>> nodes(m.size(), std::vector<node>(m[0].size()));

	open_nodes.emplace(0.0, start);

	nodes[start.first][start.second].f = 0.0;
	nodes[start.first][start.second].g = 0.0;
	nodes[start.first][start.second].h = 0.0;
	nodes[start.first][start.second].parent = start;

	while (!open_nodes.empty())
	{
		std::pair<double, coord> current = open_nodes.top();
		coord current_coord = current.second;
		open_nodes.pop();
		closed_nodes[current_coord.first][current_coord.second] = true;
		auto childs = get_neighbors(m, current_coord);

		for (auto& child : childs)
		{
			if (child == finish)
			{
				auto parent = nodes[child.first][child.second].parent;
				std::cout << "Found: " << nodes[parent.first][parent.second].f << std::endl;
				return generate_path(nodes, current_coord, start);
			}

			if (!closed_nodes[child.first][child.second])
			{
				node succesor;
				succesor.g = nodes[current_coord.first][current_coord.second].g + 1.0;
				succesor.h = heuristic(child, finish);
				succesor.f = succesor.g + succesor.h;
				succesor.parent = current_coord;

				if (nodes[child.first][child.second].f > succesor.f)
				{
					nodes[child.first][child.second] = succesor;
					open_nodes.emplace(succesor.f, coord(child.first, child.second));
				}
			}
		}

	}

	return {};
}

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
	auto path = aStar(input.m, input.start, input.finish, get_neighbors_part1);

	map  out = input.m;
	for (auto pos : path)
	{
		out[pos.first][pos.second] = std::toupper(out[pos.first][pos.second]);
	}

	std::cout << "Shortest path size: " << path.size() << std::endl;
}

int main()
{
	Timer t;

	auto input = parse_input(std::ifstream("input.txt"));

	std::cout << "start:  (" << input.start.first << ", " << input.start.second << ")" << std::endl;
	std::cout << "finish: (" << input.finish.first << ", " << input.finish.second << ")" << std::endl;

	part1(input);
}
