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
	jet_generator(const std::string& pattern);
	int next();
	unsigned pos() const;
};


class chamber
{
private:
	size_t m_width, m_height;
	unsigned long long real_height;
	shape m_lines;
	std::string empty_line;
	struct
	{
		unsigned x = 0;
		unsigned long long y = 0;
		shape s = {};
	} current_piece;

	bool is_place_free(const shape& s, unsigned x, long long y) const;
	void jet_push(jet_generator& jet);
	bool drop_step();
	void put();
	void add_empty_lines(unsigned qty);

public:
	chamber(size_t width);
	unsigned long long height() const;
	void simulate_drop(const shape& piece, jet_generator &jet, unsigned x = 2, long long h = 3);
	void draw() const;
	std::vector<std::string> top_lines(int) const;
};
