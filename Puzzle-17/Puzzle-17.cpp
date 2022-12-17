#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>

const std::vector<std::vector<std::string>> pieces{
	{
		"####"
	},
	{
		".#.",
		"###",
		".#."
	},
	{
		"..#",
		"..#",
		"###"
	},
	{
		"#",
		"#",
		"#",
		"#"
	},
	{
		"##",
		"##"
	}

};


class jet_generator
{
private: 
	std::string m_pattern;
	unsigned m_pos;

public:
	jet_generator (const std::string &pattern)
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
};

void jet_push(const std::vector<std::string>& chamber, int& posx, int posy, int p)
{

}

void simulate_fall(std::vector<std::string> chamber, int p, jet_generator &jet)
{
	int posy = chamber.size()+3;
	int posx = 2;
	bool done = false;

	while (!done)
	{
		jet_push(chamber, &posx, posy, p);
		if (can_fall())
		{
			--posy
		}
		else
		{
			drop()
				done = true;
		}
	}
}

void part1(const std::string& input, unsigned qty)
{
	std::vector<std::string> chamber;
		
	jet_generator jet(input);

	for (unsigned i = 0; i < qty; ++i)
	{
		simulate_fall(chamber, i % pieces.size(), jet);
	}
}

int main()
{
	std::string input;
	std::getline(std::ifstream("example.txt"), input);

	part1(input, 2022);
}
