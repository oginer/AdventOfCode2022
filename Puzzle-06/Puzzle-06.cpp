// Puzzle-06.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <ranges>
#include <algorithm>


size_t find_distinct_characters(const std::string &input, unsigned n)
{
	for (size_t i = 0; i < input.length()-n-1; ++i)
	{
		std::string substr = input.substr(i, n);
		std::ranges::sort(substr);
		if (std::ranges::adjacent_find(substr) == substr.end()) return i+n;
	}
}

int main()
{
	std::string line;
	std::getline(std::ifstream("input.txt"), line);

	std::cout << "Start of packet marker : " << find_distinct_characters(line, 4) << std::endl;
	std::cout << "Start of message marker: " << find_distinct_characters(line, 14) << std::endl;
}
