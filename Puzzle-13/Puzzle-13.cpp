#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <variant>
#include <algorithm>

#include "../utils/timer.h"


using t_input = std::vector<std::string>;

t_input parse_input(std::istream& input)
{
	t_input result;

	std::string line;

	while (std::getline(input, line))
	{
		if (line != "")
			result.push_back(line);
	}

	return result;
}

t_input parse_input(std::istream&& input)
{
	return parse_input(input);
}

std::variant<char, int> next_token(std::string& str, size_t &pos)
{
	std::variant<char, int> result{ '\0' };

	if (pos < str.length())
	{
		if (str[pos] == ',') ++pos;
		if (str[pos] == '[' || str[pos] == ']')
		{
			result = str[pos];
			++pos;
		}
		else
		{
			std::string_view sv(str.begin() + pos, str.end());
			int val;
			auto n = std::from_chars(sv.data(), sv.data() + sv.size(), val).ptr - sv.data();
			result = val;
			pos += n;
		}
	}

	return result;
}

bool compare_messages(std::string str1, std::string str2)
{
	size_t p1=0, p2=0;

	while (p1 < str1.length() && p2 < str2.length())
	{
		size_t prev_p1 = p1;
		size_t prev_p2 = p2;

		auto token1 = next_token(str1, p1);
		auto token2 = next_token(str2, p2);

		if (std::holds_alternative<int>(token1) && std::holds_alternative<int>(token2))
		{
			int n1 = std::get<int>(token1);
			int n2 = std::get<int>(token2);
			if (n1 < n2) return true;
			if (n1 > n2) return false;
		}
		else if (std::holds_alternative<int>(token1) || std::holds_alternative<int>(token2))
		{
			if (std::holds_alternative<int>(token1))
			{
				int n1 = std::get<int>(token1);
				char s2 = std::get<char>(token2);
				if (s2 == ']') return false;
//				// s2 == "["
				str1.insert(p1, 1, ']');
				p1 = prev_p1;
			}
			else
			{
				char s1 = std::get<char>(token1);
				int n2 = std::get<int>(token2);
				if (s1 == ']') return true;
				// s1 == "["
				str2.insert(p2, 1, ']');
				p2 = prev_p2;
			}
		}
		else
		{
			char s1 = std::get<char>(token1);
			char s2 = std::get<char>(token2);

			if (s1 != s2)
			{
				if (s1 == ']') return true;
				if (s2 == ']') return false;
			}
		}

	};

	return true;
}

void part1(const t_input& input)
{
	unsigned i = 0;
	unsigned sum = 0;
	auto it = input.begin();

	while (it != input.end())
	{
		++i;
		auto& str1 = *it;
		++it;
		auto& str2 = *it;
		++it;
		if (compare_messages(str1, str2))
		{
			sum += i;
		}
	}
	std::cout << "Sum of correct indexes: " << sum << std::endl;
}

void part2(t_input input)
{
	input.push_back("[[6]]");
	input.push_back("[[2]]");

	std::sort(input.begin(), input.end(), compare_messages);

	auto item_2 = std::lower_bound(input.begin(), input.end(), "[[2]]", compare_messages) - input.begin();
	auto item_6 = std::lower_bound(input.begin(), input.end(), "[[6]]", compare_messages) - input.begin();

	std::cout << "Decoder key: " << item_2 * item_6 << std::endl;
}

int main()
{
	Timer t("Total");

	Timer tI("Input");
	auto input = parse_input(std::ifstream("input.txt"));
	tI.finish(false);

	Timer t1;
	part1(input);
	t1.finish();

	Timer t2;
	part2(input);
	t2.finish();
}
