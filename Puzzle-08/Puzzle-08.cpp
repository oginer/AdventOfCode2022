#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <ranges>
#include <numeric>
#include <algorithm>

#include "../utils//timer.h"

using tree_t = uint8_t;

using forest = std::vector<std::vector<tree_t>>;
using vis_t = std::vector<std::vector<bool>>;

forest parse_input(std::istream& input)
{
	forest result;
	std::string line;

	while (std::getline(input, line) && line != "")
	{
		result.push_back(std::vector<tree_t>{line.begin(),line.end()});
	}

	return result;
}

forest parse_input(std::istream&& input)
{
	return parse_input(input);
}

unsigned long test_from_left(const forest& input, vis_t& vis)
{
	unsigned long count = 0;

	for (size_t i = 0; i < vis.size(); ++i)
	{
		long tallest = input[i+1][0];
		for (size_t j = 0; j < vis[0].size(); ++j)
		{
			if (input[i + 1][j + 1] > tallest)
			{
				if (!vis[i][j]) ++count;
				vis[i][j] = true;
				tallest = input[i + 1][j + 1];
			}
		}
	}

	return count;
}

unsigned long test_from_right(const forest& input, vis_t& vis)
{
	unsigned long count = 0;

	for (long i = 0; i < vis.size(); ++i)
	{
		long tallest = input[i + 1][input[0].size() - 1];
		for (long j = vis[0].size()-1; j >= 0; --j)
		{
			if (input[i + 1][j + 1] > tallest)
			{
				if (!vis[i][j]) ++count;
				vis[i][j] = true;
				tallest = input[i + 1][j + 1];
			}
		}
	}

	return count;
}

unsigned long test_from_up(const forest& input, vis_t& vis)
{
	unsigned long count = 0;

	for (size_t j = 0; j < vis[0].size(); ++j)
	{
		long tallest = input[0][j+1];
		for (size_t i = 0; i < vis.size(); ++i)
		{
			if (input[i + 1][j + 1] > tallest)
			{
				if (!vis[i][j]) ++count;
				vis[i][j] = true;
				tallest = input[i + 1][j + 1];
			}
		}
	}

	return count;
}

unsigned long test_from_down(const forest& input, vis_t& vis)
{
	unsigned long count = 0;

	for (size_t j = 0; j < vis[0].size(); ++j)
	{
		long tallest = input[input.size()-1][j + 1];
		for (long i = vis.size()-1; i >= 0; --i)
		{
			if (input[i + 1][j + 1] > tallest)
			{
				if (!vis[i][j]) ++count;
				vis[i][j] = true;
				tallest = input[i + 1][j + 1];
			}
		}
	}

	return count;
}

void part1(const forest &input)
{
	vis_t visibility(input.size()-2, std::vector<bool>(input[0].size()-2, false));

	auto visible = test_from_left(input, visibility) +
		test_from_up(input, visibility) +
		test_from_right(input, visibility) +
		test_from_down(input, visibility) +
		input.size() * 2 + (input[0].size()-2) * 2;

	std::cout << "Visible: " << visible << std::endl;
}

unsigned long score_up (const forest& input,  long x,  long y)
{
	unsigned score = 0;

	long i = 1;
	while (x - i >= 0 && input[x - i][y] < input[x][y])
	{
		++score;
		++i;
	}
	if (x - i > 0) ++score;

	return score;
}

unsigned long score_down(const forest& input, long x, long y)
{
	unsigned score = 0;

	long i = 1;
	while (x + i < input.size() && input[x + i][y] < input[x][y])
	{
		++i;
		++score;
	}
	if (x + i < input.size()) ++score;

	return score;
}

unsigned long score_left(const forest& input, long x, long y)
{
	unsigned score = 0;

	long i = 1;
	while (y - i >= 0 && input[x][y - i] < input[x][y])
	{
		++score;
		++i;
	}
	if (y - i > 0) ++score;

	return score;
}

unsigned long score_right(const forest& input, long x, long y)
{
	unsigned score = 0;

	long i = 1;
	while (y + i < input[0].size() && input[x][y + i] < input[x][y])
	{
		++score;
		++i;
	}
	if (y + i < input[0].size()) ++score;

	return score;
}

unsigned long scenic_score(const forest& input, long x, long y)
{
	unsigned long score = score_up(input, x, y) * score_down(input, x, y) * score_left(input, x, y) * score_right(input, x, y);

	return score;
}

void part2(const forest& input)
{
	unsigned long max_score = 0;

	for (size_t i = 1; i < input.size()-1; ++i)
	{
		for (size_t j = 1; j < input[0].size()-1; ++j)
		{
			auto score = scenic_score(input, i, j);
			max_score = std::max(score, max_score);
		}
	}

	std::cout << "Max score: " << max_score << std::endl;
}

int main()
{
	Timer t;

	auto input = parse_input(std::ifstream("input.txt"));

	part1(input);
	part2(input);
}
