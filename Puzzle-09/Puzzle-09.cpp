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

    bool operator == (const position& p) const
    {
        return x == p.x && y == p.y;
    }

    void operator += (const std::pair<int, int>& v)
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
    std::pair<int, int> direction;
    int distance;

    static constexpr std::pair<int, int> up {0,1};
    static constexpr std::pair<int, int> down {0,-1};
    static constexpr std::pair<int, int> left {-1,0};
    static constexpr std::pair<int, int> right {1,0};
};

std::vector<movement> parse_input(std::istream& input)
{
    std::vector<movement> result;

    std::string line;
    while (std::getline(input, line))
    {
        movement mov;

        auto command = parse_command(line);

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
        default:
            continue;
        }

        mov.distance = std::stoi(command[1]);
        result.push_back(mov);
    }

    return result;
}

bool tail_touches_head(const position &head, const position &tail )
{
    if (std::abs(tail.x - head.x) <= 1 && std::abs(tail.y - head.y) <= 1)
        return true;
    else
        return false;
}

void fix_tail(const position& head, position& tail)
{
    const int dx = tail.x - head.x;
    const int dy = tail.y - head.y;

    if (dx > 0) tail.x--;
    else if (dx < 0) tail.x++;
    if (dy > 0) tail.y--;
    else if (dy < 0) tail.y++;
}

void simulate_rope(const std::vector<movement>& input, const int n_knots)
{
    std::unordered_set<position> tail_positions = { {0,0} };
    std::vector<position> rope(n_knots, { 0,0 });

    for (auto &command : input)
    {
        for (auto i : std::views::iota(0, command.distance))
        {
            rope[0] += command.direction;

            for (int i: std::views::iota(1, n_knots))
            {
                if (!tail_touches_head(rope[i-1], rope[i]))
                {
                    fix_tail(rope[i-1], rope[i]);
                }
            }
            tail_positions.insert(rope[n_knots-1]);
       }
    }

    std::cout << "Number of tail positions (rope with " << n_knots << " knots): " << tail_positions.size() << std::endl;
}

std::vector<movement> parse_input(std::istream&& input)
{
    return parse_input(input);
}

int main()
{
    Timer t;

    std::vector<movement> input = parse_input(std::ifstream("input.txt"));

    simulate_rope(input, 2);
    simulate_rope(input, 10);
}
