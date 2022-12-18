#pragma once

#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <charconv>
#include <system_error>


std::vector<std::string> parse_command(const std::string& str, const char separator = ' ')
{
	std::vector<std::string> result;

	std::ranges::for_each(str | std::views::split(separator),
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
	value = 0;
	size_t result = 0;

	auto [ptr, ec] = std::from_chars(str.data() + pos, str.data() + str.size(), value);
	if (ec == std::errc()) // no error
	{
		result = ptr - (str.data() + pos);
	}

	return result;
}
