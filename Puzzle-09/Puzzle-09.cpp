#include <iostream>
#include <string>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <unordered_set>

#include "../utils/timer.h"
#include "../utils/parse_helper.h"


struct position
{
    int x, y;

    constexpr bool operator == (const position& p) const = default;

    constexpr void operator += (const std::pair<int, int>& v)
    {
        x += v.first;
        y += v.second;
    }
};

namespace std
{
    template<> struct hash<position>
    {
        size_t operator()(const position& p) const
        {
            return p.x ^ (p.y << 8);
        }
    };
}

struct movement
{
    std::pair<int8_t, int8_t> direction;
    unsigned distance = 0;

    static constexpr decltype(direction) up {0,1};
    static constexpr decltype(direction) down {0,-1};
    static constexpr decltype(direction) left {-1,0};
    static constexpr decltype(direction) right {1,0};
};

std::vector<movement> parse_input(std::istream& input)
{
    std::vector<movement> result;

    std::string line;
    while (std::getline(input, line))
    {
        movement mov;

        auto command = parse_command(line);

        if (command.size() == 2 && std::isdigit(command[1][0]))
        {
            switch (command[0][0])
            {
            case 'U':
                mov.direction = movement::up;
                    break;
            case 'D':
                mov.direction = movement::down;
                    break;
            case 'L':
                mov.direction = movement::left;
                break;
            case 'R':
                mov.direction = movement::right;
                break;
            }
            mov.distance = std::stoi(command[1]);
            result.push_back(mov);
        }
    }

    return result;
}

std::vector<movement> parse_input(std::istream&& input)
{
    return parse_input(input);
}

bool tail_touches_head(const position &head, const position &tail )
{
    return std::abs(tail.x - head.x) <= 1 && std::abs(tail.y - head.y) <= 1;
}

void fix_tail(const position& head, position& tail)
{
    if (!tail_touches_head(head, tail))
    {
        const int dx = tail.x - head.x;
        const int dy = tail.y - head.y;

        if (dx > 0) --tail.x;
        else if (dx < 0) ++tail.x;
        if (dy > 0) --tail.y;
        else if (dy < 0) ++tail.y;
    }
}

void simulate_rope(const std::vector<movement>& input, const unsigned n_knots)
{
    std::unordered_set<position> tail_positions = {{0, 0}};
    std::vector<position> rope(n_knots, {0, 0});

    for (auto &command : input)
    {
        for (unsigned i = 0; i<command.distance; ++i)
            {
            rope[0] += command.direction;

            for (size_t j = 1; j<n_knots; ++j)
            {
                fix_tail(rope[j-1], rope[j]);
            }
            tail_positions.insert(rope[n_knots-1]);
        }
    }

    std::cout << "Number of tail positions (rope with " << n_knots << " knots): " << tail_positions.size() << std::endl;
}

int main()
{
    Timer t;

    std::vector<movement> input = parse_input(std::ifstream("input.txt"));

    simulate_rope(input, 2);
    simulate_rope(input, 10);
}
