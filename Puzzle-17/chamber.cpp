#include "chamber.h"


jet_generator::jet_generator(const std::string& pattern)
	:m_pattern(pattern), m_pos(0)
{}

int jet_generator::next()
{
	int result = 0;
	if (m_pattern[m_pos] == '<') result = -1;
	if (m_pattern[m_pos] == '>') result = 1;
	++m_pos;
	if (m_pos == m_pattern.length()) m_pos = 0;
	return result;
}

unsigned jet_generator::pos() const
{
	return m_pos;
}


chamber::chamber(size_t width)
	:m_width(width), m_height(0)
{
	m_lines.push_back(std::string(m_width + 2, '='));
	empty_line = "|" + std::string(m_width, '.') + "|";
	add_empty_lines(3);
}

void chamber::simulate_drop(const shape& piece, jet_generator& jet, unsigned x, long long h)
{
	auto needed_h = real_height + h + piece.size() + 1;
	if (needed_h > m_lines.size())
		add_empty_lines(needed_h - m_lines.size());

	current_piece.y = real_height + h + 1;
	current_piece.x = x + 1;
	current_piece.s = piece;

	bool done = false;
	while (!done)
	{
		jet_push(jet);
		if (!drop_step())
		{
			put();
			done = true;
		}
	}
}

std::vector<std::string> chamber::top_lines(int qty) const
{
	std::vector<std::string > result;

	for (auto it = m_lines.rbegin(); qty > 0 && it != m_lines.rend(); ++it)
	{
		if (*it != empty_line)
		{
			result.push_back(*it);
			--qty;
		}
	}
	return result;
}

void chamber::draw() const
{
	for (auto it = m_lines.rbegin(); it != m_lines.rend(); ++it)
		std::cout << *it << std::endl;
}

unsigned long long chamber::height() const
{
	return real_height;
}

bool chamber::is_place_free(const shape& s, unsigned x, long long y) const
{
	unsigned i = 0;
	for (auto& line : s)
	{
		unsigned j = 0;
		for (auto& c : line)
		{
			if (c == '#' && m_lines[i+y][j+x] != '.') return false;
			++j;
		}
		++i;
	}
	return true;
}

void chamber::jet_push(jet_generator& jet)
{
	auto try_x = current_piece.x + jet.next();

	if (is_place_free(current_piece.s, try_x, current_piece.y))
	{
		current_piece.x = try_x;
	}
}

bool chamber::drop_step()
{
	if (is_place_free(current_piece.s, current_piece.x, current_piece.y-1))
	{
		--current_piece.y;
		return true;
	}

	return false;
}

void chamber::put()
{
	unsigned long long y = current_piece.y;
	for (auto& sline : current_piece.s)
	{
		unsigned x = current_piece.x;
		for (auto& c : sline)
		{
			if (c == '#') m_lines[y][x] = '*';
			++x;
		}
		++y;
	}

	if (y-1 > real_height) real_height = y-1;
}

void chamber::add_empty_lines(unsigned qty)
{
	for (auto i = qty; i>0; --i)
		m_lines.push_back(empty_line);
}
