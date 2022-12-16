#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <cassert>

#include "../utils/parse_helper.h"
#include "../utils/timer.h"

#include "NORange.h"


struct coord { long x = 0; long y = 0; };
using input_data = std::vector<std::pair<coord, coord>>;


input_data parse_input(std::istream& input)
{
	input_data result;
	std::string line;

	while (std::getline(input, line) && line != "")
	{
		auto command = parse_command(line);

		if (command.size() == 10)
		{
			coord sensor;
			parse_number(command[2], sensor.x, 2);
			parse_number(command[3], sensor.y, 2);
			
			coord beacon;
			parse_number(command[8], beacon.x, 2);
			parse_number(command[9], beacon.y, 2);
			
			result.push_back({ sensor, beacon });
		}
	}

	return result;
}

input_data parse_input(std::istream&& input)
{
	return parse_input(input);
}

long manhattan_distance(const coord& c1, const coord& c2)
{
	return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
}

non_overlaping_ranges create_ranges(const input_data& input, long test_row)
{
	non_overlaping_ranges result;

	for (auto& [sensor, beacon] : input)
	{
		auto d = manhattan_distance(sensor, beacon);
		auto row_d = std::abs(test_row - sensor.y);

		if (row_d <= d)
		{
			result.insert({ sensor.x - d + row_d, sensor.x + d - row_d });
		}
	}

	return result;
}

void part1(const input_data& input, long test_row)
{
	auto ranges = create_ranges(input, test_row);

	auto size = ranges.size();

	std::set<std::pair<int, int>> beacons;

	for (auto& [sensor, beacon] : input)
	{
		if (sensor.y == test_row && ranges.is_inside(sensor.x))
			--size;

		if (beacon.y == test_row && beacons.find({ beacon.x, beacon.y }) == beacons.end())
		{
			beacons.insert({ beacon.x, beacon.y });
			if (ranges.is_inside(beacon.x))
				--size;
		}
	}

	std::cout << "Positions that cannot contain the beacon: " << size << std::endl;
}

non_overlaping_ranges create_ranges_part2(const input_data& input, long test_row, long max_xy)
{
	non_overlaping_ranges result;

	for (auto& [sensor, beacon] : input)
	{
		auto d = manhattan_distance(sensor, beacon);
		auto row_d = std::abs(test_row - sensor.y);

		if (row_d <= d)
		{
			auto start = sensor.x - d + row_d;
			auto end = sensor.x + d - row_d;
			if (end >= 0 && start <= max_xy)
			{
				if (start < 0) start = 0;
				if (end > max_xy) end = max_xy;
				result.insert({ start, end });
			}
		}
	}

	return result;
}

void part2(const input_data& input, long max_xy)
{
#pragma omp parallel for
	for (long i = max_xy; i >= 0; --i)
	{
		auto ranges = create_ranges_part2(input, i, max_xy);
		if (ranges.size() == max_xy)
		{
			long x = -1;
			long prev = -1;
			for (const auto& range : ranges)
			{
				if (range.start > prev + 1) x = range.start - 1;
				prev = range.end;
			}
			if (prev == max_xy - 1) x = max_xy;
			std::cout << "Found: (" << i << ", " << x << ")" << std::endl;
			std::cout << "Tuning frequency: " << x * 4000000ll + i << std::endl;
			break;
		}
	}
}

int main()
{
	Timer t("Total");

	Timer ti("Input");
	auto input = parse_input(std::fstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(input, 2000000);
	t1.finish();

	Timer t2;
	part2(input, 4000000);
	t2.finish();
}
