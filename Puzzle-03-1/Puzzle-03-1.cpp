#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <span>

unsigned short priority(char c)
{
	if (c <= 'Z')  // is c uppercase?
		return c - 'A' + 27;
	else
		return c - 'a' + 1;
}

int main()
{
	std::ifstream file("input.txt");
	std::string line;
	std::set<char> group_backsack[3];
	int group_index = 0;

	unsigned long sum = 0;
	unsigned long sum_badges = 0;

	while (std::getline(file, line))
	{
		auto rucksacks_size = line.length();
		auto comp_size = rucksacks_size / 2;
		std::set<char> comp_item_types[2];

		comp_item_types[0].insert(line.begin(), line.begin() + comp_size);
		comp_item_types[1].insert(line.begin()+comp_size, line.end());

		std::vector<char> repeats(1);
		std::ranges::set_intersection(comp_item_types[0], comp_item_types[1], repeats.begin());

		sum += priority(repeats[0]);

		group_backsack[group_index] = std::set<char>(line.begin(), line.end());

		if (group_index == 2)
		{
			std::vector<char> first_intersection;
			std::ranges::set_intersection(group_backsack[0], group_backsack[1], std::back_inserter(first_intersection));

			std::vector<char> badge;
			std::ranges::set_intersection(group_backsack[2], first_intersection, std::back_inserter(badge));

			sum_badges += priority(badge[0]);

			group_index = 0;
		}
		else
		{
			++group_index;
		}
	}

	std::cout << "Sum of repeats = " << sum << std::endl;
	std::cout << "Sum of badges = " << sum_badges << std::endl;
}
