#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <numeric>
#include <ranges>
#include <set>

#include "../utils/timer.h"
#include "../utils/parse_helper.h"

struct Operation
{
    enum {sum, mult, pow2} operand = sum;
    int param = 0;
};

struct Monkey
{
    std::queue<long long> items;
    Operation op;
    unsigned divisible_by = 1;
    size_t throw_if_true = 0;
    size_t throw_if_false = 0;
    unsigned inspections = 0;
};

Monkey parse_monkey(std::istream& input)
{
    Monkey result;

    std::string line;
    while (std::getline(input, line))
    {
        auto command = parse_command(line);
        if (command.size() >= 2 && command[0] == "Starting")
        {
            for (size_t i = 2; i < command.size(); ++i)
            {
                result.items.push(std::stoll(command[i]));
            }
        }
        else if (command.size() == 6 && command[0] == "Operation:")
        {
            switch (command[4][0])
            {
            case '+':
                result.op.operand = Operation::sum;
                result.op.param = std::stol(command[5]);
                break;
            case '*':
                if (command[5] == "old")
                {
                    result.op.operand = Operation::pow2;
                }
                else
                {
                    result.op.operand = Operation::mult;
                    result.op.param = std::stol(command[5]);
                }
            }
        }
        else if (command.size() == 4 && command[0] == "Test:")
        {
            result.divisible_by = std::stoul(command[3]);
        }
        else if (command.size() == 6 && command[0] == "If")
        {
            if (command[1] == "true:") result.throw_if_true = std::stoul(command[5]);
            else result.throw_if_false = std::stoul(command[5]);
        }
        else if (command.size() == 0) break;
    }

    return result;
}

std::vector<Monkey> parse_input(std::istream& input)
{
    std::vector<Monkey> result;

    std::string line;
    while (std::getline(input, line))
    {
        auto command = parse_command(line);
        if (command[0] == "Monkey")
        {
            result.push_back(parse_monkey(input));
        }
    }

    return result;
}

std::vector<Monkey> parse_input(std::istream&& input)
{
    return parse_input(input);
}

void monkey_round(std::vector<Monkey>& input, bool divide_by_3, unsigned lcm)
{

    for (Monkey& monkey : input)
    {
        while (!monkey.items.empty())
        {
            long long item = monkey.items.front();
            monkey.items.pop();
            monkey.inspections++;
            switch (monkey.op.operand)
            {
            case Operation::sum:
                item += monkey.op.param;
                break;
            case Operation::mult:
                item *= monkey.op.param;
                break;
            case Operation::pow2:
                item *= item;
                break;
            }
            if (divide_by_3) item /= 3;
            else item %= lcm;

            if (item % monkey.divisible_by == 0)
            {
                input[monkey.throw_if_true].items.push(item);
            }
            else
            {
                input[monkey.throw_if_false].items.push(item);
            }
        }
    }
}

void monkey_play(std::vector<Monkey> input, unsigned rounds, bool divide_by_3)
{
    unsigned lcm = std::reduce(input.cbegin(), input.cend(), 1, [](unsigned acc, const Monkey& m) {return std::lcm(acc, m.divisible_by); });

    for (unsigned i = 0; i < rounds; ++i)
    {
        monkey_round(input, divide_by_3, lcm);
    }

    std::set<unsigned> inspections;
    for (const Monkey& m : input)
    {
        inspections.insert(m.inspections);
    }

    std::partial_sort(input.begin(), input.begin() + 2, input.end(), [](const Monkey& m1, const Monkey& m2) -> bool {return m1.inspections > m2.inspections; });

    std::cout << "Monkey business: " << static_cast<unsigned long long>(input[0].inspections) * input[1].inspections << std::endl;
}

int main()
{
    Timer t;

    std::vector<Monkey> input = parse_input(std::ifstream("input.txt"));

    monkey_play(input, 20, true);
    monkey_play(input, 10000, false);
}
