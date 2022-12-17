#pragma once


struct node
{
	std::unordered_map<std::string, int> adjacents;
};

class Graph
{
private:
	std::unordered_map<std::string, node> nodes;

public:
	void insert_node(const std::string& name);
	void insert_edge(const std::string& from, const std::string& to, int cost = 1);
	void erase_node(const std::string& name);
	void erase_edge(const std::string& from, const std::string& to);
	void collapse_node(const std::string& name);

	int distance(const std::string& start, const std::string& end);
};
