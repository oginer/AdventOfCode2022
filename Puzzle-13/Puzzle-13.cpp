#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <variant>

using t_input = std::vector<std::pair<std::string, std::string>>;

t_input parse_input(std::istream& input)
{
	t_input result;

	std::string line;

	while (std::getline(input, line))
	{
		std::pair<std::string, std::string> messages;

		messages.first = line;
		std::getline(input, line);
		messages.second = line;

		std::getline(input, line);

		result.push_back(messages);
	}

	return result;
}

t_input parse_input(std::istream&& input)
{
	return parse_input(input);
}

std::variant<std::string, int> next_token(std::string& sv, size_t &p)
{
	std::variant<std::string, int> result;

	if (sv == "") return "";
	if (sv[p] == ',') ++p;
	if (sv[p] == '[' || sv[p] == ']')
	{
		result = std::string(sv.substr(p,1));
		++p;
		return result;
	}
	else
	{
		size_t n;
		int number = std::stoi(sv.substr(p), &n);
		p += n;
		result = number;
		return result;
	}
}

bool compare_messages(const std::pair<std::string, std::string>& messages)
{
	bool result = false;
	size_t p1=0, p2=0;
	auto [str1, str2] = messages;

	std::cout << "Comparing: ";
	std::cout << str1 << std::endl;
	std::cout << "           " << str2 << std::endl;


	do
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
				std::string s2 = std::get<std::string>(token2);
				if (s2 == "]") return false;
				// s2 == "["
				str1.insert(p1, 1, ']');
				p1 = prev_p1;
			}
			else
			{
				std::string s1 = std::get<std::string>(token1);
				int n2 = std::get<int>(token2);
				if (s1 == "]") return true;
				// s1 == "["
				str2.insert(p2, 1, ']');
				p2 = prev_p2;
			}
		}
		else
		{
			std::string s1 = std::get<std::string>(token1);
			std::string s2 = std::get<std::string>(token2);

			if (s1 != s2)
			{
				if (s1 == "]") return true;
				if (s2 == "]") return false;
			}
		}

	} while (str1 != "" && str2 != "");

	return true;
}

void part1(const t_input& input)
{
	unsigned i = 0;
	unsigned sum = 0;

	for (const auto &messages : input)
	{
		++i;
		if (compare_messages(messages))
		{
			std::cout << "Right order" << std::endl;
			sum += i;
		}
		else std::cout << "Wrong order" << std::endl;
	}
	std::cout << "Sum of correct indexes: " << sum << std::endl;
}

int main()
{
	auto input = parse_input(std::ifstream("input.txt"));

	part1(input);
}
