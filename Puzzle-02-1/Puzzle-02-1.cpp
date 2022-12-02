
#include <iostream>
#include <fstream>
#include <map>

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

void do_the_thing(bool real_decode)
{
    std::ifstream input("input.txt");
    unsigned long score_p1 = 0;
    unsigned long score_p2 = 0;
    unsigned long line = 1;

    while (input.peek() != EOF)
    {
        char p1('-'), p2('-');

        input >> p1 >> p2;

        try
        {
            p1 = shape_decode.at(p1);
            if (real_decode)
                p2 = shape_decode_p2_real.at({ p1, p2 });
            else
                p2 = shape_decode.at(p2);

            score_p1 += shape_score.at(p1);
            score_p2 += shape_score.at(p2);

            if (p1 == p2)
            {
                score_p1 += draw_score;
                score_p2 += draw_score;
            }
            else
            {
                if ((p1 == 'R' && p2 == 'P') ||
                    (p1 == 'P' && p2 == 'S') ||
                    (p1 == 'S' && p2 == 'R'))
                {
                    score_p2 += win_score;
                }
                else
                {
                    score_p1 += win_score;
                }
            }
        }
        catch (...)
        {
            std::cout << "Error at line " << line << ": p1 = " << p1 << ", p2 = " << p2 << std::endl;
        }

        ++line;
    }

    std::cout << "Score after " << line - 1 << " matches (" << (real_decode ? "correct" : "incorrect") << " estratey): " << score_p2 << " (rival score: " << score_p1 << ")" << std::endl;
}

int main()
{
    do_the_thing(false);
    do_the_thing(true);
}
 