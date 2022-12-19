#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <fstream>

#include "../utils/parse_helper.h"
#include "../utils/timer.h"

struct blueprint
{
	long ore_cost;
	long clay_cost;
	long obsidiant_cost_ore;
	long obsidiant_cost_clay;
	long geode_cost_ore;
	long geode_cost_obsidian;
};

struct state
{
	int ore_robot = 1;
	int clay_robot = 0;
	int obs_robot = 0;
	int geo_robot = 0;
	int ore_robot_fab = 0;
	int clay_robot_fab = 0;
	int obs_robot_fab = 0;
	int geo_robot_fab = 0;
	int ore = 0;
	int clay = 0;
	int obs = 0;
	int geo = 0;
	int minute = 0;
};

long simulate(const blueprint& bp, int time = 24)
{
	int best_geo = 0;
	std::stack<state> open_states;

	open_states.push({});

	while (!open_states.empty())
	{
		auto cstate = open_states.top();
		open_states.pop();

		++cstate.minute;
		int n = time-cstate.minute+cstate.geo_robot;
		int m = cstate.geo_robot;
		int max_posible = (n * n - m * m + n + m) / 2;
		if (cstate.minute <= time && max_posible + cstate.geo + cstate.geo_robot >= best_geo)
		{
			cstate.ore += cstate.ore_robot;
			cstate.clay += cstate.clay_robot;
			cstate.obs += cstate.obs_robot;
			cstate.geo += cstate.geo_robot;
			cstate.ore_robot += cstate.ore_robot_fab;
			cstate.ore_robot_fab = 0;
			cstate.clay_robot += cstate.clay_robot_fab;
			cstate.clay_robot_fab = 0;
			cstate.obs_robot += cstate.obs_robot_fab;
			cstate.obs_robot_fab = 0;
			cstate.geo_robot += cstate.geo_robot_fab;
			cstate.geo_robot_fab = 0;

			if (cstate.ore >= bp.geode_cost_ore && cstate.obs >= bp.geode_cost_obsidian)
			{
				auto cnext = cstate;
				cnext.ore -= bp.geode_cost_ore;
				cnext.obs -= bp.geode_cost_obsidian;
				cnext.geo_robot_fab = 1;
				open_states.push(cnext);
			}
			else
			{
				open_states.push(cstate);
				if (cstate.ore >= bp.obsidiant_cost_ore && cstate.clay >= bp.obsidiant_cost_clay && cstate.obs_robot <= bp.geode_cost_obsidian)
				{
					auto cnext = cstate;
					cnext.ore -= bp.obsidiant_cost_ore;
					cnext.clay -= bp.obsidiant_cost_clay;
					cnext.obs_robot_fab = 1;
					open_states.push(cnext);
				}
				if (cstate.ore >= bp.ore_cost && cstate.ore_robot <= std::max(bp.obsidiant_cost_ore, bp.geode_cost_ore))
				{
					auto cnext = cstate;
					cnext.ore -= bp.ore_cost;
					cnext.ore_robot_fab = 1;
					open_states.push(cnext);
				}
				if (cstate.ore >= bp.clay_cost && cstate.clay_robot <= bp.obsidiant_cost_clay)
				{
					auto cnext = cstate;
					cnext.ore -= bp.clay_cost;
					cnext.clay_robot_fab = 1;
					open_states.push(cnext);
				}
			}
		}
		else
		{
			if (cstate.geo > best_geo) best_geo = cstate.geo;
		}
	};

	return best_geo;
}

void part1(const std::vector<blueprint>& bps)
{
	struct state current_state;
	current_state.ore_robot = 1;

	long value = 0;
	int i = 1;
	for (auto& bp : bps)
	{
		value += i * simulate(bp);
		++i;
	}

	std::cout << "Sum of quality levels: " << value << std::endl;
}

void part2(const std::vector<blueprint>& bps)
{
	struct state current_state;
	current_state.ore_robot = 1;

	long value = 1;

	int i = 0;
	for (auto& bp : bps)
	{
		if (i == 3) break;
		long geo = simulate(bp, 32);
		value *= geo;
		++i;
	}

	std::cout << "Multiplication of maximum geodes: " << value << std::endl;
}

int main()
{
	Timer t("Total");

	Timer ti("input");
	std::string line;
	std::ifstream file("input.txt");

	std::vector<blueprint> blueprints;

	while (std::getline(file, line) && line != "")
	{
		auto command = parse_command(line);

		blueprints.push_back({ to_number(command[6]),
			to_number(command[12]),
			to_number(command[18]),
			to_number(command[21]),
			to_number(command[27]),
			to_number(command[30]) });
	}
	ti.finish(false);

	Timer t1;
	part1(blueprints);
	t1.finish();

	Timer t2;
	part2(blueprints);
	t2.finish();
}
