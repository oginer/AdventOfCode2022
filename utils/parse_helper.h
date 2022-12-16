#pragma once

#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <charconv>
#include <system_error>


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

template <typename T>
size_t parse_number(const std::string& str, T& value, size_t pos = 0)
{
	auto [ptr, ec] = std::from_chars(str.data() + pos, str.data() + str.size(), value);
	if (ec == std::errc())
	{
		return ptr - (str.data() + pos);
	}

	return 0;
}
