#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
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
	unsigned time,
	unsigned minutes)
{
	assert(time <= minutes);

	std::pair<long, std::vector<std::string>> result = { current_flow, opening_order };

	unopened_valves.erase(valve);
	opening_order.push_back(valve);

	result.second = opening_order;
	for (auto &open_valve : opening_order)
	{
		result.first += valves.at(open_valve) * (minutes -time);
	}

	for (const auto& unopen_valve : unopened_valves)
	{
		unsigned t = distances.at({valve, unopen_valve}) + 1;
		if (time + t < minutes)
		{
			long flow = current_flow;
			for (auto open_valve : opening_order)
			{
				flow += valves.at(open_valve) * (t);
			}
			auto [f, order] = best_flow_r(opening_order, unopen_valve, flow, unopened_valves, valves, distances, time + t, minutes);
			if (f > result.first) result = { f, order };
		}
	}

	return result;
}

std::pair<long, std::vector<std::string>> best_flow(
	const std::string start,
	std::unordered_set<std::string> unopened_valves,
	const std::unordered_map<std::string, unsigned>& valves,
	const std::map<std::pair<std::string, std::string>, int>& distances,
	const unsigned minutes = 30)
{
	std::pair<long, std::vector<std::string>> result = { 0, {} };

	std::vector<std::string> opening_order;

	opening_order.reserve(unopened_valves.size());

	for (const auto& valve : unopened_valves)
	{
		auto t = distances.at({ start, valve }) + 1;
		if (t < minutes)
		{
			auto [flow, order] = best_flow_r(opening_order, valve, 0, unopened_valves, valves, distances, t, minutes);
			if (flow > result.first) result = { flow, order };
		}
	}

	return result;
}

void part1(const input_data& input)
{
	auto g = build_graph(input.valves);

	std::unordered_set<std::string> unopened_valves;
	std::unordered_map<std::string, unsigned> valves;

	for (auto& v : input.valves)
	{
		if (v.flow_rate == 0 && v.name != input.start) g.collapse_node(v.name);
		if (v.flow_rate > 0)
		{
			unopened_valves.insert(v.name);
			valves[v.name] = v.flow_rate;
		}
	}

	auto distances = g.shorter_distances_table();

	auto [flow, path] = best_flow(input.start, unopened_valves, valves, distances);

	std::cout << "Open valves in this order: " << std::accumulate(path.cbegin() + 1, path.cend(), *path.cbegin(),
		[](const std::string& s1, const std::string & s2) -> std::string const
		{
			return s1 + " -> " + s2;
		});
	std::cout << std::endl;

	std::cout << "Best flow: " << flow << std::endl;
}

void part2(const input_data& input)
{
	auto g = build_graph(input.valves);

	std::unordered_set<std::string> unopened_valves;
	std::unordered_map<std::string, unsigned> valves;

	for (auto& v : input.valves)
	{
		if (v.flow_rate == 0 && v.name != input.start) g.collapse_node(v.name);
		if (v.flow_rate > 0)
		{
			unopened_valves.insert(v.name);
			valves[v.name] = v.flow_rate;
		}
	}

	auto distances = g.shorter_distances_table();

	auto [flow, path] = best_flow(input.start, unopened_valves, valves, distances, 26);

	std::cout << "Open valves in this order: " << std::accumulate(path.cbegin() + 1, path.cend(), *path.cbegin(),
		[](const std::string& s1, const std::string& s2) -> std::string const
		{
			return s1 + " -> " + s2;
		});
	std::cout << std::endl;

	std::cout << "Best flow: " << flow << std::endl;

	std::unordered_set<std::string> unopened_valves2;

	for (auto& v : input.valves)
	{
		if (v.flow_rate > 0)
		{
			unopened_valves2.insert(v.name);
		}
	}

	for (auto& v : path)
	{
		unopened_valves2.erase(v);
	}

	auto [flow2, path2] = best_flow(input.start, unopened_valves2, valves, distances, 26);

	std::cout << "Open valves in this order: " << std::accumulate(path2.cbegin() + 1, path2.cend(), *path2.cbegin(),
		[](const std::string& s1, const std::string& s2) -> std::string const
		{
			return s1 + " -> " + s2;
		});
	std::cout << std::endl;

	std::cout << "Best flow: " << flow2 << std::endl;

	std::cout << std::endl << "Total flow: " << flow + flow2 << std::endl;

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

	Timer t2;
	part2(data);
	t2.finish();
}
