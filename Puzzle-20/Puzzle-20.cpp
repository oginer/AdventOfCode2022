#include <vector>
#include <deque>
#include <list>
#include <iostream>
#include <fstream>
#include <string>

#include "../utils/timer.h"


using numbers = std::list<long long>;

void print(const numbers& input)
{
	for (auto n : input)
	{
		std::cout << n << " ";
	}
	std::cout << std::endl;
}

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

void part1(numbers number_list)
{
	std::vector<numbers::iterator> index;
	numbers::iterator it_zero;
	index.reserve(number_list.size());

	for (auto it = number_list.begin(); it!= number_list.end(); ++it)
	{
		if (*it == 0) it_zero = it;
		index.push_back(it);
	}

//	print(number_list);
	for (auto i: index)
	{
		move_element(number_list, i);
//		print(number_list);
	}

	long long result = 0;
	auto it = it_zero;
	it = next_rot(number_list, it_zero, 1000 % (number_list.size()));
	result += *it;
	it = next_rot(number_list, it, 1000 % (number_list.size()));
	result += *it;
	it = next_rot(number_list, it, 1000 % (number_list.size()));
	result += *it;

	std::cout << "Decription key: " << result << std::endl;
}

void part2(numbers number_list)
{
	constexpr long long decrypt_key = 811589153;

	std::vector<numbers::iterator> index;
	numbers::iterator it_zero;
	index.reserve(number_list.size());

	for (auto it = number_list.begin(); it != number_list.end(); ++it)
	{
		if (*it == 0) it_zero = it;
		*it *= decrypt_key;
		index.push_back(it);
	}

//	print(number_list);
	for (auto rep = 10; rep > 0; --rep)
	{
		for (auto i : index)
		{
			move_element(number_list, i);
		}
//		print(number_list);
	}

	long long result = 0;
	auto it = it_zero;
	it = next_rot(number_list, it_zero, 1000 % (number_list.size()));
	result += *it;
	it = next_rot(number_list, it, 1000 % (number_list.size()));
	result += *it;
	it = next_rot(number_list, it, 1000 % (number_list.size()));
	result += *it;

	std::cout << "Decription key: " << result << std::endl;
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
