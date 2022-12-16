#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "../utils/parse_helper.h"
#include "../utils/timer.h"


struct coord { long x = 0; long y = 0; };

using beacons = std::vector<std::pair<coord, coord>>;

beacons parse_input(std::istream& input)
{
	beacons result;
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

beacons parse_input(std::istream&& input)
{
	return parse_input(input);
}

void part1(const beacons& input)
{

}

void part2(const beacons& input)
{

}

int main()
{
	Timer t("Total");

	Timer ti("Input");
	auto input = parse_input(std::fstream("example.txt"));
	ti.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t2.finish();
}
