#include <vector>
#include <deque>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include "../utils/timer.h"


using numbers = std::list<long long>;

numbers parse_input(std::ifstream& input)
{
	numbers result;

	while (input.peek() != EOF)
	{
		int k;
		input >> k;
		result.push_back(k);
	}

	return result;
}

numbers parse_input(std::ifstream&& input)
{
	return parse_input(input);
}

numbers::iterator next_rot(numbers& l, const numbers::iterator& it, long long n)
{
	numbers::iterator result = it;

	for (auto i = n; i > 0; --i)
	{
		++result;
		if (result == l.end()) result = l.begin();
	}

	return result;
}

numbers::iterator prev_rot(numbers& l, const numbers::iterator& it, long long n)
{
	numbers::iterator result = it;

	for (auto i = n; i > 0; --i)
	{
		if (result == l.begin()) result = l.end();
		--result;
	}

	return result;
}

void move_element(numbers& input, numbers::iterator& it, int times = 1)
{
	long long n = (*it);
	long long move_distance = (std::abs(n)) % (input.size() - 1);

	numbers::const_iterator it_dest;

	if (n >= 0)
		it_dest = next_rot(input, it, move_distance + 1);
	else
		it_dest = prev_rot(input, it, move_distance);

	input.splice(it_dest, input, it);
}

long long mix(numbers number_list, long long multiplier = 1, unsigned times = 1)
{

	std::vector<numbers::iterator> index;
	numbers::iterator it_zero;
	index.reserve(number_list.size());

	for (auto it = number_list.begin(); it != number_list.end(); ++it)
	{
		if (*it == 0) it_zero = it;
		*it *= multiplier;
		index.push_back(it);
	}

	for (auto rep = times; rep > 0; --rep)
	{
		for (auto& i : index)
		{
			move_element(number_list, i);
		}
	}

	long long result = 0;
	auto it = it_zero;
	for (auto i = 3; i > 0; --i)
	{
		it = next_rot(number_list, it, 1000 % (number_list.size()));
		result += *it;
	}

	return result;
}

void part1(const numbers &number_list)
{
	auto result = mix(number_list);

	std::cout << "Sum of grove coordinates: " << result << std::endl;
}

void part2(const numbers &number_list)
{
	constexpr long long decrypt_key = 811589153;

	auto result = mix(number_list, decrypt_key, 10);

	std::cout << "Sum of grove coordinates with decryption key: " << result << std::endl;
}

int main()
{
	Timer t("Total");

	Timer ti("Input");
	auto input = parse_input(std::ifstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t1.finish();
}
