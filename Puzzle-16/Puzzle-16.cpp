#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

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
	result.start = "AA";

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

std::pair<long, std::vector<std::string>> best_flow_r(std::vector<std::string> opening_order,
	const std::string& valve,
	long current_flow,
	std::unordered_set<std::string> unopened_valves,
	const std::unordered_map<std::string, unsigned>& valves,
	const std::map<std::pair<std::string, std::string>, int>& distances,
	unsigned time)
{
	assert(time <= 30);

	std::pair<long, std::vector<std::string>> result = { current_flow, opening_order };

	unopened_valves.erase(valve);
	opening_order.push_back(valve);

	result.second = opening_order;
	for (auto &open_valve : opening_order)
	{
		result.first += valves.at(open_valve) * (30-time);
	}

	for (const auto& unopen_valve : unopened_valves)
	{
		unsigned t = distances.at({valve, unopen_valve}) + 1;
		if (time + t < 30)
		{
			long flow = current_flow;
			for (auto open_valve : opening_order)
			{
				flow += valves.at(open_valve) * (t);
			}
			auto [f, order] = best_flow_r(opening_order, unopen_valve, flow, unopened_valves, valves, distances, time + t);
			if (f > result.first) result = { f, order };
		}
	}

	return result;
}

std::pair<long, std::vector<std::string>> best_flow(const input_data& input, const std::map<std::pair<std::string, std::string>, int>& distances)
{
	std::pair<long, std::vector<std::string>> result = { 0, {} };

	std::vector<std::string> opening_order;
	std::unordered_set<std::string> unopened_valves;
	std::unordered_map<std::string, unsigned> valves;

	for (auto& valve : input.valves)
	{
		if (valve.flow_rate > 0)
		{
			unopened_valves.insert(valve.name);
			valves[valve.name] = valve.flow_rate;
		}
	}

	opening_order.reserve(unopened_valves.size());

	for (const auto& valve : unopened_valves)
	{
		auto t = distances.at({ input.start, valve }) + 1;
		if (t < 30)
		{
			auto [flow, order] = best_flow_r(opening_order, valve, 0, unopened_valves, valves, distances, t);
			if (flow > result.first) result = { flow, order };
		}
	}

	for (auto &v : result.second)
	{
		std::cout << v << ", ";
	}
	std::cout << std::endl;
	return result;
}

void part1(const input_data& input)
{
	auto g = build_graph(input.valves);

	for (auto& v : input.valves)
	{
		if (v.flow_rate == 0 && v.name != input.start) g.collapse_node(v.name);
	}

	auto distances = g.shorter_distances_table();

	auto flow = best_flow(input, distances);

	std::cout << "Best flow: " << flow.first << std::endl;
}

int main()
{
	Timer t("Total");

	Timer ti("Input");
	auto data = parse_input(std::ifstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(data);
	t1.finish();
}
