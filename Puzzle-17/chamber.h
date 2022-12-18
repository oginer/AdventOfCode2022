#pragma once

#include <vector>
#include <string>
#include <iostream>

using shape = std::vector<std::string>;

class jet_generator
{
private:
	std::string m_pattern;
	unsigned m_pos;

public:
	jet_generator(const std::string& pattern)
		:m_pattern(pattern), m_pos(0)
	{}

	int next()
	{
		int result = 0;
		if (m_pattern[m_pos] == '<') result = -1;
		if (m_pattern[m_pos] == '>') result = 1;
		++m_pos;
		if (m_pos == m_pattern.length()) m_pos = 0;
		return result;
	}

	unsigned pos()
	{
		return m_pos;
	}
};

class chamber
{
private:
	size_t m_width, m_height;
	unsigned long long real_height;
	std::vector<std::string> m_lines;
	std::string empty_line;
	struct
	{
		unsigned x;
		unsigned long long y;
		shape s;
	} current_piece;

	bool is_place_free(const shape& s, unsigned x, long long y) const;
	void jet_push(jet_generator& jet);
	bool drop_step();
	void put();
	void add_empty_lines(unsigned qty);

public:
	chamber(size_t width);
	unsigned long long height();
	void simulate_drop(const std::vector<std::string>& piece, jet_generator &jet, unsigned x = 2, long long h = 3);
	void draw() const;
	std::vector<std::string> top_lines(int) const;
};
