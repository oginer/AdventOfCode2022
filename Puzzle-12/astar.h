#pragma once

#include <vector>
#include <string>
#include <queue>
#include <type_traits>

using map = std::vector<std::string>;
using coord = std::pair<int, int>;

template <typename T>
struct node
{
	T f{ std::numeric_limits<T>::max() };
	T g{ std::numeric_limits<T>::max() };
	coord parent;
};

template <typename T>
std::vector<coord> generate_path(const std::vector<std::vector<node<T>>>& nodes, const coord& end)
{
	std::vector<coord> path;
	coord current = end;
	path.push_back(end);
	do
	{
		current = nodes[current.first][current.second].parent;
		path.push_back(current);
	} while (current != nodes[current.first][current.second].parent);
	return path;
}

template <typename Tif, typename Tgn, typename Th>
std::vector<coord> aStar(const map& m, const coord& start,
	const Tif is_finish,
	const Tgn get_neighbors,
	const Th heuristic)
{
	using t_cost = std::invoke_result<Th, const coord&>::type;

	std::priority_queue<std::pair<t_cost, coord>, std::vector<std::pair<t_cost, coord>>, std::greater<std::pair<t_cost, coord>>> open_nodes;
	std::vector<std::vector<bool>> closed_nodes{ m.size(), std::vector<bool>(m[0].size(), false) };
	std::vector<std::vector<node<t_cost>>> nodes(m.size(), std::vector<node<t_cost>>(m[0].size()));

	open_nodes.emplace(0, start);

	nodes[start.first][start.second].f = static_cast<t_cost>(0);
	nodes[start.first][start.second].g = static_cast<t_cost>(0);
	nodes[start.first][start.second].parent = start;

	while (!open_nodes.empty())
	{
		auto current_coord = open_nodes.top().second;
		open_nodes.pop();
		closed_nodes[current_coord.first][current_coord.second] = true;
		auto childs = get_neighbors(m, current_coord);

		for (auto [cx, cy] : childs)
		{
			if (is_finish({cx, cy}))
			{
				auto parent = nodes[cx][cy].parent;
				return generate_path(nodes, current_coord);
			}

			if (!closed_nodes[cx][cy])
			{
				node<t_cost> succesor;
				succesor.g = nodes[current_coord.first][current_coord.second].g + static_cast<t_cost>(1);
				t_cost h = heuristic({cx, cy});
				succesor.f = succesor.g + h;
				succesor.parent = current_coord;

				if (nodes[cx][cy].f > succesor.f)
				{
					nodes[cx][cy] = succesor;
					open_nodes.emplace(succesor.f, coord{ cx, cy });
				}
			}
		}

	}

	return {};
}
