#include <iostream>
#include <string>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <set>
#include <list>

#include "../utils/timer.h"
#include "../utils/parse_helper.h"


struct position
{
    int x, y;

    bool operator < (const position& p) const
    {
        if (x < p.x) return true;
        if (x > p.x) return false;
        return y < p.y;
    }
};

struct movement
{
    enum {up, down, right, left} dir;
    int distance;

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
            mov.dir = movement::up;
            break;
        case 'D':
            mov.dir = movement::down;
            break;
        case 'L':
            mov.dir = movement::left;
            break;
        case 'R':
            mov.dir = movement::right;
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
    if (abs(tail.x - head.x) <= 1 && abs(tail.y - head.y) <= 1)
        return true;
    else
        return false;
}

void fix_tail(const position& head, position& tail)
{
    int dx = tail.x - head.x;
    int dy = tail.y - head.y;

    if (dx > 0) tail.x--;
    if (dx < 0) tail.x++;
    if (dy > 0) tail.y--;
    if (dy < 0) tail.y++;
}

void simulate_rope(const std::vector<movement>& input, const int n_knots)
{
    std::set<position> tail_positions;

    std::vector<position> rope(n_knots, { 0,0 });

    tail_positions.insert(rope[n_knots - 1]);

    for (auto &command : input)
    {
        for (auto i : std::views::iota(0, command.distance))
        {
            switch (command.dir)
            {
            case (movement::down):
                rope[0].y--;
                break;
            case (movement::up):
                rope[0].y++;
                break;
            case (movement::left):
                rope[0].x--;
                break;
            case (movement::right):
                rope[0].x++;
                break;
            }

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
