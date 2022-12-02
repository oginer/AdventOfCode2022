
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

constexpr unsigned win_score = 6;
constexpr unsigned draw_score = 3;
const map<char, unsigned> shape_score
{
    {'R', 1},
    {'P', 2},
    {'S', 3}
};

const map<const char, const char> shape_decode
{
    {'A', 'R'},
    {'B', 'P'},
    {'C', 'S'},
    {'X', 'R'},
    {'Y', 'P'},
    {'Z', 'S'}
};

int main()
{
    ifstream input("input.txt");
    unsigned long score = 0;
    unsigned long line = 1;

    while (input.peek() != EOF)
    {
        char p1('-'), p2('-');

        input >> p1 >> p2;

        try
        {
            p1 = shape_decode.at(p1);
            p2 = shape_decode.at(p2);

            score += shape_score.at(p2);

            if (p1 == p2)
            {
                score += draw_score;
            }
            else
            {
                if ((p1 == 'R' && p2 == 'P') ||
                    (p1 == 'P' && p2 == 'S') ||
                    (p1 == 'S' && p2 == 'R'))
                {
                    score += win_score;
                }
            }
        }
        catch(...)
        {
            std::cout << "Error at line " << line << ": p1 = " << p1 << ", p2 = " << p2 << endl;
        }

        ++line;
    }

    std::cout << "Score after " << line-1 << " matches: " << score << endl;
}
 