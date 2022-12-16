#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <numeric>
#include <charconv>
#include <algorithm>

#include "../utils/timer.h"
#include "../utils/parse_helper.h"


constexpr unsigned ncol = 1000;
using bitmap = std::vector<std::array<int8_t, ncol>>;
struct coord { int row = 0; int col = 0; };

coord parse_coord(const std::string& str)
{
	coord result;

	auto comma = str.find_first_of(',');

	std::from_chars(str.data(), (str.data() + comma), result.col);
	std::from_chars(str.data() + comma + 1, str.data() + str.length(), result.row);

	return result;
}

void draw_line(bitmap& bmp, const coord &start, const coord &end)
{
	const coord* st = &start;
	const coord* ed = &end;
	if (start.row == end.row)
	{
		if (start.col > end.col)
		{
			ed = &start;
			st = &end;
		}
		
		for (int i = st->col; i <= ed->col; ++i)
		{
			bmp[start.row][i] = 1;
		}
	}
	else
	{
		if (start.row > end.row)
		{
			ed = &start;
			st = &end;
		}
		for (int i = st->row; i <= ed->row; ++i)
		{
			bmp[i][start.col] = 1;
		}
	}
}

bitmap parse_input(std::istream& input)
{
	bitmap result;
	std::string line;

	while (std::getline(input, line))
	{
		auto command = parse_command(line);
		coord prev = parse_coord(command[0]);

		for (size_t i = 2; i < command.size(); i += 2)
		{
			coord next = parse_coord(command[i]);
			while (prev.row >= result.size() || next.row >= result.size())
			{
				result.push_back({});
			}
			draw_line(result, prev, next);
			prev = next;
		}
	}

	return result;
}

bitmap parse_input(std::istream&& input)
{
	return parse_input(input);
}

bool drop_sand(bitmap& input, coord sand)
{
	while (++sand.row < input.size())
	{
		if (input[sand.row][sand.col] != 0)
		{
			if (input[sand.row][sand.col - 1] != 0)
			{
				if (input[sand.row][sand.col + 1] != 0)
				{
					input[sand.row-1][sand.col] = 2;
					if (sand.row - 1 == 0) return false;
					else return true;
				}
				else ++sand.col;
			}
			else --sand.col;
		}
	}

	return false;
}

void part1(bitmap input)
{
	unsigned sand_deposited = 0;

	while (drop_sand(input, { 0,500 }))
	{
		++sand_deposited;
	}

	std::cout << "Sand deposited: " << sand_deposited << std::endl;
}

void part2(bitmap input)
{
	unsigned sand_deposited = 0;

	input.push_back({});
	input.push_back({});
	input.back().fill(1);

	while (drop_sand(input, { 0,500 }))
	{
		++sand_deposited;
	}

	std::cout << "Sand deposited: " << sand_deposited+1 << std::endl;
}

int main()
{
	Timer t("Total");

	Timer ti("input");
	bitmap input = parse_input(std::ifstream("input.txt"));
	ti.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t2.finish();
}
