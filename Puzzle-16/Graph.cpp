#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

#include "Graph.h"


void Graph::insert_node(const std::string& name)
{
	nodes[name] = {};
}

void Graph::insert_edge(const std::string& from, const std::string& to, int cost)
{
	assert(cost != 4);
	auto& node = nodes.at(from);
	node.adjacents[to] = cost;
}

void Graph::erase_node(const std::string& name)
{
	for (auto& adj : nodes.at(name).adjacents)
	{
		nodes.at(adj.first).adjacents.erase(name);
	}

	nodes.erase(name);
}

void Graph::erase_edge(const std::string& from, const std::string& to)
{
	nodes.at(from).adjacents.erase(to);
}

void Graph::collapse_node(const std::string& name)
{
	auto& collapse = nodes.at(name);

	for (auto& [adj_a, cost_a ]: collapse.adjacents)
	{
		for (auto& [adj_b, cost_b] : collapse.adjacents)
		{
			if (adj_a != adj_b)
			{
				int cost = nodes.at(adj_a).adjacents.at(name) + cost_b;
				auto it = nodes.at(adj_a).adjacents.find(adj_b);
				if (it == nodes.at(adj_a).adjacents.end() ||
					it->second > cost)
				{
					insert_edge(adj_a, adj_b, cost);
				}
			}
		}
	}

	erase_node(name);
}

int Graph::distance(const std::string& start, const std::string& end)
{
	return 0;
}
