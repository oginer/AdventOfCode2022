#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

#include "../utils/timer.h"
#include "../utils/parse_helper.h"

struct instruction
{
	enum {nop, addx} opcode = nop;
	unsigned cycles=1;
	int param=0;
};

std::vector<instruction> parse_input(std::istream& input)
{
	std::vector<instruction> result;

	std::string line;
	while (std::getline(input, line))
	{
		auto command = parse_command(line);

		if (command.size() > 0)
		{
			if (command[0] == "noop") result.push_back({});
			else if (command[0] == "addx")
			{
				if (command.size() == 2 && (std::isdigit(command[1][0]) || command[1][0] == '-'))
				{
					result.push_back(instruction {instruction::addx, 2, std::stol(command[1])});
				}
			}
		}
	}

	return result;
}

std::vector<instruction> parse_input(std::istream&& input)
{
	return parse_input(input);
}

void part1(const std::vector<instruction>& input)
{
	std::array cycles_wanted {20u, 60u, 100u, 140u, 180u, 220u};
	unsigned current_cycle = 1;
	auto op = input.cbegin();
	int registerx = 1;
	int acc_registerx = 0;

	for (unsigned target_cycle : cycles_wanted)
	{
		while (current_cycle < target_cycle-1)
		{
			if (op->opcode == instruction::addx)
			{
				registerx += op->param;
			}
			current_cycle += op->cycles;
			op++;
		}
		acc_registerx += registerx * target_cycle;
	}

	std::cout << "sum = " << acc_registerx << std::endl;
}


constexpr unsigned crt_w = 40;
constexpr unsigned crt_h = 6;

void draw_screen(const char screen[][crt_w])
{
	std::cout << std::endl;

	for (size_t i = 0; i < crt_h; ++i)
	{
		for (size_t j = 0; j < crt_w; ++j)
			std::cout << screen[i][j];
		std::cout << std::endl;
	}
}

void draw_sprite(int registerx, int current_col, char *current_pixel)
{
	if (registerx >= current_col - 1 &&
		registerx <= current_col + 1)
	{
		*current_pixel = '#';
	}
}

void part2(const std::vector<instruction>& input)
{
	char screen[crt_h][crt_w];
	std::memset(screen, '.', crt_h * crt_w);

	char* current_pixel = &(screen[0][0]);
	unsigned current_cycle = 0;
	int current_col = 0;

	int registerx = 1;
	auto op = input.cbegin();

	while (current_cycle < crt_h * crt_w)
	{
		for (unsigned i = 0; i < op->cycles; ++i)
		{
			draw_sprite(registerx, current_col, current_pixel);
			++current_pixel;
			current_col = (current_col + 1) % crt_w;
		}

		if (op->opcode == instruction::addx)
		{
			registerx += op->param;
		}

		current_cycle += op->cycles;
		op++;
	}

	draw_screen(screen);
}

int main()
{
	Timer t;

	std::vector<instruction> input = parse_input(std::ifstream("input.txt"));

	part1(input);
	part2(input);
}
