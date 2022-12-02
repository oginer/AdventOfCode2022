
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

constexpr unsigned win_score = 6;
constexpr unsigned draw_score = 3;

const std::map<char, unsigned> shape_score
{
    {'R', 1},
    {'P', 2},
    {'S', 3}
};

const std::map<const char, const char> shape_decode
{
    {'A', 'R'},
    {'B', 'P'},
    {'C', 'S'},
    {'X', 'R'},
    {'Y', 'P'},
    {'Z', 'S'}
};

// X = lose
// Y = draw
// Z = win
const std::map<const std::pair<const char, const char>, const char> shape_decode_p2_real
{
    // P1  code  P2
    {{'R', 'X'}, 'S'},
    {{'R', 'Y'}, 'R'},
    {{'R', 'Z'}, 'P'},
    {{'P', 'X'}, 'R'},
    {{'P', 'Y'}, 'P'},
    {{'P', 'Z'}, 'S'},
    {{'S', 'X'}, 'P'},
    {{'S', 'Y'}, 'S'},
    {{'S', 'Z'}, 'R'}
};

using matches = std::vector<std::pair<char, char>>;

matches parse_input(std::istream &input)
{
    matches data;

    while (input.peek() != EOF)
    {
        char p1, p2;
        input >> p1;
        input >> p2;
        if (input.good()) data.push_back({ p1, p2 });
    }

    return data;
}

bool p2_wins(const char p1, const char p2)
{
    return (p1 == 'R' && p2 == 'P') ||
        (p1 == 'P' && p2 == 'S') ||
        (p1 == 'S' && p2 == 'R');
}

void do_the_thing(const matches const& input, const bool real_decode)
{
    unsigned long score_p2 = 0;
 
    for (auto match : input)
    {
        char p1, p2;

        try
        {
            p1 = shape_decode.at(match.first);
            if (real_decode)
                p2 = shape_decode_p2_real.at({ p1, match.second });
            else
                p2 = shape_decode.at(match.second);
        }
        catch (...)
        {
            std::cout << "Bad input: {" << match.first << ", " << match.second << "}" << std::endl;
            return;
        }

        score_p2 += shape_score.at(p2);

        if (p1 == p2)
            score_p2 += draw_score;
        else if (p2_wins(p1, p2))
            score_p2 += win_score;
    }

    std::cout << "Score with " << (real_decode ? "correct" : "incorrect") << " estrategy: " << score_p2 << std::endl;
}

int main()
{
    std::ifstream file("input.txt");
    auto input = parse_input(file);

    do_the_thing(input, false);
    do_the_thing(input, true);
}
 