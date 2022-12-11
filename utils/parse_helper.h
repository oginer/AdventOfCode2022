#pragma once

#include <vector>
#include <string>
#include <ranges>
#include <algorithm>


std::vector<std::string> parse_command(const std::string& str)
{
	std::vector<std::string> result;

	std::ranges::for_each(str | std::views::split(' '),
		[&result](const auto& v)
		{
			std::string str = std::string(v.begin(), v.end());
			if (str.length() > 0)
				result.push_back(str);
		});

	return result;
}
