#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <unordered_map>

#include "../utils/timer.h"

#include "chamber.h"


const std::vector<shape> pieces{
	{
		"####"
	},
	{
		".#.",
		"###",
		".#."
	},
	{
		"###",
		"..#",
		"..#"
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

long long simulate(chamber& ch, jet_generator jet, unsigned long long qty, int piece = 0)
{
	for (unsigned long long i = piece; i < qty+piece; ++i)
	{
		ch.simulate_drop(pieces[i % pieces.size()], jet);
	}

	return ch.height();
}

void part1(const std::string& input, long long qty)
{
	chamber ch(7);

	jet_generator jet(input);

	std::cout << "The tower is " << simulate(ch, jet, qty) << " units tall." << std::endl;
}

struct state
{
	size_t piece_idx;
	unsigned int jet_pos;
	std::vector<std::string> top_tower;

	bool operator== (const state&) const = default;
};

struct state_hash
{
	size_t operator() (const state& st) const
	{
		std::string r;
		for (auto& str : st.top_tower)
		{
			r += str;
		}
		return std::hash<size_t>{}(st.piece_idx) ^ (std::hash<unsigned>{}(st.jet_pos) << 4) ^ (std::hash<std::string>{}(r) << 8);
	}
};

void part2(const std::string& input, unsigned long long qty)
{
	constexpr int buffer_size = 20;

	std::unordered_map<state, std::pair<unsigned long long, unsigned long long>, state_hash> prev_states;
	chamber ch(7);

	jet_generator jet(input);

	unsigned long long big_height = 0;

	for (unsigned long long i = 0; i < qty; ++i)
	{
		ch.simulate_drop(pieces[i % pieces.size()], jet);

		state current{ i % pieces.size(), jet.pos(), ch.top_lines(buffer_size)};
		auto it = prev_states.find(current);
		if (it != prev_states.end())
		{
			const auto& [previous_state, previous_data] = *it;
			const auto& [previous_i, previous_height] = previous_data;

			std::cout << "Repeated state at iteration " << i << ", with height = " << ch.height() << std::endl;
			std::cout << "  piece: " << current.piece_idx << ", jet: " << current.jet_pos << std::endl;
			std::cout << "Previous was at iteration " << previous_i << ", with height = " << previous_height << std::endl;
			std::cout << "  piece: " << previous_state.piece_idx << ", jet: " << previous_state.jet_pos << std::endl;

			unsigned long long cycle_size = i - previous_i;
			unsigned long long height_delta = ch.height() - previous_height;
			std::cout << std::endl << "Cycle: every " << cycle_size << " iterations, heigh increments by " << height_delta << std::endl;

			big_height += previous_height;  // size of the beginning section
			big_height += (qty - previous_i - 1) / cycle_size * height_delta;   // increment during the cicles;

			auto prev_height = ch.height();
			// heigh of the end, uncompleted cycle
			big_height += simulate(ch, jet, (qty - previous_i - 1) % cycle_size, (previous_state.piece_idx + 1) % pieces.size()) - prev_height;

			break;
		}
		else
		{
			prev_states[current] = {i, ch.height()};
		}
	}

	std::cout << std::endl << "The tower is " << big_height << " units tall." << std::endl;
}

int main()
{
	Timer t("Total");

	std::string input;
	std::getline(std::ifstream("input.txt"), input);

	Timer t1;
	part1(input, 2022);
	t1.finish();

	Timer t2;
	part2(input, 1000000000000);
	t2.finish();
}
