#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <sstream>
#include <ranges>
#include <algorithm>
#include <numbers>

struct cargo
{
	std::vector<std::stack<char>> stacks;
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
	std::vector<std::string> lines;

	while (std::getline(input, line) && line != "")
	{
		lines.push_back(line);
	}

	size_t n_stacks = (lines.rbegin()->length() + 1) / 4;

	result.stacks.push_back({});
	result.stacks.resize(n_stacks, std::stack<char>{});

	for (auto l : lines | std::views::reverse | std::views::drop(1))
	{
		for (int i = 0; i < n_stacks; ++i)
		{
			char c = l[1 + i * 4];
			if (c != ' ') result.stacks[i].push(c);
		}
	}

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

void part1(cargo cargos, const std::vector<operation>& ops)
{
	for (auto op : ops)
	{
		for (unsigned i = 0; i < op.qty; ++i)
		{
			cargos.stacks[op.to].push(cargos.stacks[op.from].top());
			cargos.stacks[op.from].pop();
		}
	}

	for (auto c : cargos.stacks)
	{
		std::cout << c.top();
	}
	std::cout << std::endl;
}

void part2(cargo cargos, const std::vector<operation>& ops)
{
	for (auto op : ops)
	{
		std::stack<char> stk;
		for (unsigned i = 0; i < op.qty; ++i)
		{
			stk.push(cargos.stacks[op.from].top());
			cargos.stacks[op.from].pop();
		}
		while (!stk.empty())
		{
			cargos.stacks[op.to].push(stk.top());
			stk.pop();
		}
	}

	for (auto c : cargos.stacks)
	{
		std::cout << c.top();
	}
	std::cout << std::endl;
}


int main()
{
	std::ifstream input("input.txt");
	cargo cargos = parse_cargo(input);
	std::vector<operation> ops = parse_operations(input);

	part1(cargos, ops);
	part2(cargos, ops);
}
