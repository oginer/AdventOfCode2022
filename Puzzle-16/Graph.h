#pragma once

#include <unordered_map>
#include <map>
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
	int edge_cost(const T& from, const T& to) const;
	std::map<std::pair<T, T>, int> shorter_distances_table() const;
};


template <typename T>
void graph<T>::insert_node(const T& name)
{
	nodes[name] = {};
}

template <typename T>
void graph<T>::insert_edge(const T& from, const T& to, int cost)
{
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

template<typename T>
int graph<T>::edge_cost(const T& from, const T& to) const
{
	if (from == to) return 0;
	auto adj = nodes.at(from).adjacents;
	auto it = adj.find(to);
	if (it == adj.end()) return std::numeric_limits<int>::max();
	else return it->second;
}

template <typename T>
std::map<std::pair<T, T>, int> graph<T>::shorter_distances_table() const
{
	std::map<std::pair<T, T>, int> result;

	for (const auto& from : nodes)
	{
		for (const auto& to : nodes)
		{
			result[{from.first, to.first}] = edge_cost(from.first, to.first);
		}
	}

	for (const auto& it_k: nodes)
		for (const auto& it_i : nodes)
			for (const auto& it_j : nodes) {
				auto i = it_i.first;
				auto j = it_j.first;
				auto k = it_k.first;
				int d;
				if (result[{i, k}] == std::numeric_limits<int>::max() ||
					result[{k, j}] == std::numeric_limits<int>::max())
				{
					d = std::numeric_limits<int>::max();
				}
				else
				{
					d = result[{i, k}] + result[{k, j}];
				}
				if (result[{i, j}] > d)
					result[{i, j}] = d;
			}

	return result;
}
