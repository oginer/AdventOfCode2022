#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "../utils/parse_helper.h"
#include "../utils/timer.h"


struct operation
{
	char op;
	std::string operand1;
	std::string operand2;
};

struct t_input
{
	std::unordered_map<std::string, long long> numbers;
	std::unordered_map<std::string, operation> unsolved;
	std::unordered_map<std::string, std::string> parent;
};

t_input parse_input(std::istream& input)
{
	t_input result;

	std::string line;
	while (std::getline(input, line) && line != "")
	{
		auto command = parse_command(line);
		if (command.size() == 2)
		{
			result.numbers[command[0].substr(0,4)] = to_number(command[1]);
		}
		else if (command.size() == 4)
		{
			result.unsolved[command[0].substr(0,4)] = operation{ .op = command[2][0], .operand1 = command[1], .operand2 = command[3]};
			result.parent[command[1]] = command[0].substr(0, 4);
			result.parent[command[3]] = command[0].substr(0, 4);
		}
	}

	return result;
}

t_input parse_input(std::istream&& input)
{
	return parse_input(input);
}

long long calculate(const t_input& input, std::string name)
{
	auto number_it = input.numbers.find(name);
	if (number_it != input.numbers.end())
	{
		return input.numbers.at(name);
	}
	else
	{
		auto uns_it = input.unsolved.at(name);
		long long op1 = calculate(input, uns_it.operand1);
		long long op2 = calculate(input, uns_it.operand2);

		switch (uns_it.op)
		{
		case '+':
			return op1 + op2;
		case '-':
			return op1 - op2;
		case '/':
			return op1 / op2;
		case '*':
			return op1 * op2;
		};
	}

	assert(true);
}

long long what_value(const t_input& input, const std::string& name, const std::string& child = "")
{
	if (name == "humn")
	{
		return what_value(input, input.parent.at(name), name);
	}
	else
	{
		auto uns_it = input.unsolved.at(name);
		std::string known = (uns_it.operand1 == child ? uns_it.operand2 : uns_it.operand1);

		if (name == "root")
		{
			return calculate(input, known);
		}
		else
		{
			long long operand = calculate(input, known);

			long long wanted_result = what_value(input, input.parent.at(name), name);

			switch (uns_it.op)
			{
			case '+':
				return wanted_result - operand;
			case '-':
				if (child == uns_it.operand1) return operand + wanted_result;
				else return operand - wanted_result;
			case '*':
				return wanted_result / operand;
			case '/':
				if (child == uns_it.operand1) return operand * wanted_result;
				else return operand / wanted_result;
			}
		}
	}
}

void part2(const t_input& input)
{
	long long result = what_value(input, "humn");

	std::cout << "Number to yell: " << result << std::endl;
}

void part1(const t_input& input)
{
	long long result = calculate(input, "root");

	std::cout << "Result: " << result << std::endl;
}

int main()
{
	Timer t;

	Timer ti("Input");
	t_input input = parse_input(std::ifstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t2.finish();
}

