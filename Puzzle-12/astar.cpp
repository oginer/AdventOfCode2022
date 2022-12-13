#include <vector>
#include <functional>
#include <string>
#include <queue>

#include "astar.h"


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

std::vector<coord> aStar(const map& m, const coord& start,
	const std::function<bool(const coord&)> is_finish,
	const std::function<std::vector<coord>(const map&,const coord&)> get_neighbors,
	const std::function<unsigned(const coord&)> heuristic)
{
	std::priority_queue<std::pair<unsigned, coord>, std::vector<std::pair<unsigned, coord>>, std::greater<std::pair<unsigned, coord>>> open_nodes;
	std::vector<std::vector<bool>> closed_nodes{ m.size(), std::vector<bool>(m[0].size(), false) };
	std::vector<std::vector<node>> nodes(m.size(), std::vector<node>(m[0].size()));

	open_nodes.emplace(0, start);

	nodes[start.first][start.second].f = 0;
	nodes[start.first][start.second].g = 0;
	nodes[start.first][start.second].parent = start;

	while (!open_nodes.empty())
	{
		std::pair<unsigned, coord> current = open_nodes.top();
		coord current_coord = current.second;
		open_nodes.pop();
		closed_nodes[current_coord.first][current_coord.second] = true;
		auto childs = get_neighbors(m, current_coord);

		for (auto& child : childs)
		{
			if (is_finish(child))
			{
				auto parent = nodes[child.first][child.second].parent;
				return generate_path(nodes, current_coord, start);
			}

			if (!closed_nodes[child.first][child.second])
			{
				node succesor;
				succesor.g = nodes[current_coord.first][current_coord.second].g + 1;
				unsigned h = heuristic(child);
				succesor.f = succesor.g + h;
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
