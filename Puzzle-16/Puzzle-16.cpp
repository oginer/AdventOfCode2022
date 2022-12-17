#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#include "graph.h"

#include "../utils/parse_helper.h"
#include "../utils/timer.h"


struct valve
{
	std::string name;
	int flow_rate;
	std::vector<std::string> leads_to;
};

struct input_data
{
	std::string start;
	std::vector<valve> valves;
};

input_data parse_input(std::istream& s)
{
	input_data result;
	std::string line;

	while (std::getline(s, line) && line != "")
	{
		auto command = parse_command(line);

		valve v;
		v.name = command[1];
		if (result.start == "") result.start = command[1];
		parse_number(command[4], v.flow_rate, 5);

		for (rsize_t i = 9; i < command.size(); ++i)
		{
			v.leads_to.push_back(command[i].substr(0, 2));
		}

		result.valves.push_back(v);
	}

	return result;
}

input_data parse_input(std::istream&& s)
{
	return parse_input(s);
}


graph<std::string> build_graph(const std::vector<valve>& input)
{
	graph<std::string> result;

	for (auto& v : input)
	{
		result.insert_node(v.name);

		for (auto& adj : v.leads_to)
		{
			result.insert_edge(v.name, adj);
		}
	}

	return result;
}

template <typename T>
using table = std::unordered_map<std::string, std::unordered_map<std::string, T>>;

table<int> distance_table(const std::vector<valve>& input)
{
	table<int> result;

	return result;
}

void part1(const input_data& input)
{
	auto graph = build_graph(input.valves);

	for (auto& v : input.valves)
	{
		if (v.flow_rate == 0 && v.name != input.start) graph.collapse_node(v.name);
	}
}

int main()
{
	Timer t("Total");

	Timer ti("Input");
	auto data = parse_input(std::ifstream("example.txt"));
	ti.finish(false);

	Timer t1;
	part1(data);
	t1.finish();
}
