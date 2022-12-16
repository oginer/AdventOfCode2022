#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <numeric>
#include <cassert>

#include "../utils/parse_helper.h"
#include "../utils/timer.h"


struct coord { long x = 0; long y = 0; };
using input_data = std::vector<std::pair<coord, coord>>;


struct range
{
	long start = 0;
	long end = 0;

	long size() const
	{
		return std::abs(end - start);
	}

	bool overlaps(const range& other) const
	{
		return !(end < other.start || start > other.end);
	}

	void merge_with(const range& other)
	{
		assert(this->overlaps(other));

		if (other.start < start) start = other.start;
		if (other.end > end) end = other.end;
	}

	bool operator < (const range& r) const
	{
		return start < r.start;
	}
};


class non_overlaping_ranges
{
private:
	std::set<range> ranges;

public:
	void insert(const range& r)
	{
		if (ranges.empty()) ranges.insert(r);
		else
		{
			range merged_range = r;
			auto it = ranges.upper_bound(r);
			if (it != ranges.begin()) --it;

			while (it != ranges.end() && it->start <= r.end)
			{
				if (it->overlaps(merged_range))
				{
					merged_range.merge_with(*it);
					it = ranges.erase(it);
				}
				else ++it;
			}

			ranges.insert(merged_range);
		}
	}

	std::set<range>::const_iterator cbegin() const
	{
		return ranges.cbegin();
	}

	std::set<range>::const_iterator cend() const
	{
		return ranges.cend();
	}
};

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

void part1(const input_data& input)
{
	constexpr long test_row = 2000000;

	non_overlaping_ranges ranges;


	for (auto& [sensor, beacon] : input)
	{
		auto d = manhattan_distance(sensor, beacon);
		auto row_d = std::abs(test_row - sensor.y);

		if (row_d <= d)
		{
			ranges.insert({ sensor.x - d + row_d, sensor.x + d - row_d });
		}
	}

	auto safe_size = std::reduce(ranges.cbegin(), ranges.cend(), 0,
		[](const long acc, const range& r)
		{
			return acc + r.size();
		});

	std::cout << "Positions that cannot contain the beacon: " << safe_size << std::endl;
}

void part2(const input_data& input)
{

}

int main()
{
	Timer t("Total");

	Timer ti("Input");
	auto input = parse_input(std::fstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t2.finish();
}
