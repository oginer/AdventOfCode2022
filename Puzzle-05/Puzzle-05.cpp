#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <numbers>
#include <ranges>

struct cargo
{
	std::vector<std::list<char>> stacks;
};

struct operation
{
	unsigned from;
	unsigned to;
	unsigned qty;
};

cargo parse_cargo(std::istream& input)
{
	cargo result;

	std::string line;

	std::getline(input, line);
	size_t n_stacks = (line.length() + 1) / 4;
	result.stacks.resize(n_stacks);
	do
	{
		for (size_t i = 0; i < n_stacks; ++i)
		{
			char c = line[i*4 + 1];
			if (c != ' ') result.stacks[i].push_front(c);
		}

	} while (std::getline(input, line) && line[1] != '1');

	return result;
}

std::vector<operation> parse_operations(std::istream& input)
{
	std::vector<operation> result;
	std::string line;

	while (std::getline(input, line))
	{
		if (line.substr(0, 4) == "move")
		{
			std::stringstream ss(line.substr(4));

			operation op;
			std::string str1, str2;

			ss >> op.qty >> str1 >> op.from >> str2 >> op.to;

			if (str1 == "from" && str2 == "to" && op.from != op.to)
			{
				op.from--;
				op.to--;
				result.push_back(op);
			}
		}
	}

	return result;
}

void print_stack_tops(const cargo& cargos)
{
	for (const auto &c : cargos.stacks)
	{
		std::cout << c.back();
	}
	std::cout << std::endl;
}

void part1(cargo cargos, const std::vector<operation>& ops)
{
	for (const auto &op : ops)
	{
		for (unsigned i = 0; i < op.qty; ++i)
		{
			cargos.stacks[op.to].push_back(cargos.stacks[op.from].back());
			cargos.stacks[op.from].pop_back();
		}
	}

	print_stack_tops(cargos);
}

void part2(cargo cargos, const std::vector<operation>& ops)
{
	for (const auto &op : ops)
	{
		auto it = std::prev(cargos.stacks[op.from].cend(), op.qty);
		cargos.stacks[op.to].splice(cargos.stacks[op.to].cend(), cargos.stacks[op.from], it, cargos.stacks[op.from].cend());
	}

	print_stack_tops(cargos);
}


int main()
{
	std::ifstream input("input.txt");

	cargo cargos = parse_cargo(input);
	std::vector<operation> ops = parse_operations(input);

	part1(cargos, ops);
	part2(cargos, ops);
}
