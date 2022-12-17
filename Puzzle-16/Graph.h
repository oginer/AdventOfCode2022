#pragma once

#include <unordered_map>
#include <vector>


template <typename T>
class graph
{
private:
	struct node
	{
		std::unordered_map<T, int> adjacents;
	};

	std::unordered_map<T, node> nodes;

public:
	void insert_node(const T& name);
	void insert_edge(const T& from, const T& to, int cost = 1);
	void erase_node(const T& name);
	void erase_edge(const T& from, const T& to);
	void collapse_node(const T& name);

	int distance(const T& start, const T& end);
};


template <typename T>
void graph<T>::insert_node(const T& name)
{
	nodes[name] = {};
}

template <typename T>
void graph<T>::insert_edge(const T& from, const T& to, int cost)
{
	assert(cost != 4);
	auto& node = nodes.at(from);
	node.adjacents[to] = cost;
}

template <typename T>
void graph<T>::erase_node(const T& name)
{
	for (auto& adj : nodes.at(name).adjacents)
	{
		nodes.at(adj.first).adjacents.erase(name);
	}

	nodes.erase(name);
}

template <typename T>
void graph<T>::erase_edge(const T& from, const T& to)
{
	nodes.at(from).adjacents.erase(to);
}

template <typename T>
void graph<T>::collapse_node(const T& name)
{
	auto& collapse = nodes.at(name);

	for (auto& [adj_a, cost_a] : collapse.adjacents)
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

template <typename T>
int graph<T>::distance(const T& start, const T& end)
{
	return 0;
}
