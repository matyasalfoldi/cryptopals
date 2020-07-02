#include "SingleByteXorBruteForce.h"

double calc_value(const std::vector<std::uint32_t>& xored)
{

    static const std::map<std::uint32_t, double> FreqTable = {
        {101, 12.702}, // e
        {116, 9.056}, // t
        {97, 8.167}, // a
        {111, 7.507}, // o
        {105, 6.966}, // i
        {110, 6.749}, // n
        {32, 15}, // space (slightly more frequent than e according to wikipedia)
        {115, 6.327}, // s
        {104, 6.094}, // h
        {114, 5.987}, // r
        {100, 4.253}, // d
        {108, 4.025}, // l
        {117, 2.758}, // u
        {99, 2.782}, // c
        {109, 2.406}, // m
        {119, 2.360}, // w
        {102, 2.228}, // f
        {103, 2.015}, // g
        {121, 1.974}, // y
        {112, 1.929}, // p
        {98, 1.492}, // b
        {118, 0.978}, // v
        {120, 0.150}, // x
        {107, 0.772}, // k
        {69, 12.702}, // E
        {84, 9.056}, // T
        {65, 8.167}, // A
        {79, 7.507}, // O
        {73, 6.966}, // I
        {78, 6.749}, // N
        {83, 6.327}, // S
        {72, 6.094}, // H
        {82, 5.987}, // R
        {68, 4.253}, // D
        {76, 4.025}, // L
        {85, 2.758}, // U
        {67, 2.782}, // C
        {87, 2.360}, // W
        {77, 2.406}, // M
        {70, 2.228}, // F
        {89, 1.974}, // Y
        {80, 1.929}, // P
        {66, 1.492}, // B
        {71, 2.015}, // G
        {86, 0.978}, // V
        {88, 0.150}, // X
        {75, 0.772}, // K
    };

    double score = 0;
    for (std::uint32_t i = 0; i < xored.size(); ++i)
    {
        if (FreqTable.find(xored[i]) != FreqTable.end())
        {
            score += FreqTable.find(xored[i])->second;
        }
    }
    return score;
}

std::uint32_t best_value(const std::vector<std::uint32_t>& input)
{
    std::map<std::uint32_t, double> scores;
    std::vector<std::uint32_t> xored(input.size());
    for (std::uint32_t key = 0; key <= 255; ++key)
    {
        for (std::uint32_t i = 0; i < input.size(); ++i)
        {
            xored[i] = input[i] ^ key;
        }
        scores[key] = calc_value(xored);
    }

    auto maxElement = std::max_element(
        scores.begin(),
        scores.end(),
        [](const std::pair<std::uint32_t, double> f, const std::pair<std::uint32_t, double> s)
        {
            return f.second < s.second;
        });
    return maxElement->first;
}