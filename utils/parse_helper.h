#pragma once

#include <vector>
#include <string>


std::vector<std::string> parse_command(const std::string& str)
{
	std::vector<std::string> result;

	std::ranges::for_each(str | std::views::split(' '),
		[&result](const auto& v)
		{
			result.push_back(std::string(v.begin(), v.end()));
		});

	return result;
}
