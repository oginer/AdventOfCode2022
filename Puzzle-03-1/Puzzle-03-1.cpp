#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

int main()
{
	std::ifstream file("input.txt");
	std::string line;

	unsigned long sum = 0;

	while (std::getline(file, line))
	{
		auto rucksacks_size = line.length();
		auto comp_size = rucksacks_size / 2;
		std::set<char> comp_item_types[2];

		for (auto i = comp_size; i > 0; --i)
		{
			comp_item_types[0].insert(line[i-1]);
			comp_item_types[1].insert(line[i+comp_size-1]);
		}

		std::vector<char> repeats(1);
		
		std::set_intersection(comp_item_types[0].begin(), comp_item_types[0].end(),
			comp_item_types[1].begin(), comp_item_types[1].end(),
			repeats.begin());

		for (auto c : repeats)
		{
			if (c <= 'Z')  // is c uppercase?
			{
				sum += c - 'A' + 27;
			}
			else
			{
				sum += c - 'a' + 1;
			}
		}
	}

	std::cout << "Sum of repeats = " << sum << std::endl;
}
