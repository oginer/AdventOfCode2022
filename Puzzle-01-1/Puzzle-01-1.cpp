#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <numeric>

using namespace std;

int main()
{
	string line;
	ifstream input("input.txt");
	multiset<unsigned long, greater<unsigned long>> elf_ranking;
	unsigned long elf_calories = 0;

	while (getline(input, line))
	{
		if (line != "")
		{
			elf_calories += stoul(line);
		}
		else
		{
			elf_ranking.insert(elf_calories);
			elf_calories = 0;
		}
	}

	auto top_elf = elf_ranking.begin();

	cout << "Max calories: " << *top_elf << endl;
	cout << "Sum of 3 best: " << accumulate(top_elf, next(top_elf, 3), 0ul) << endl;
}
