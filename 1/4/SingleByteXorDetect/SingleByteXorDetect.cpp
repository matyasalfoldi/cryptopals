#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void init_hex_vec(std::vector<std::uint32_t>& hex, const std::string& hexString)
{
    std::string tmp = "";
    tmp = hexString[0];
    std::uint32_t i;
    for (i = 1; i < hexString.length(); ++i)
    {
        if (i % 2 == 0)
        {
            hex.push_back(std::stoi(tmp, nullptr, 16));
            tmp = hexString[i];
        }
        else
        {
            tmp += hexString[i];
        }
    }
    hex.push_back(std::stoi(tmp, nullptr, 16));
}

//https://en.wikipedia.org/wiki/Letter_frequency#Relative_frequencies_of_letters_in_the_English_language
std::uint32_t calc_value(const std::vector<std::uint32_t>& xored)
{

    static const std::map<std::uint32_t, std::uint32_t> FreqTable = {
        {101, 12}, // e
        {116, 9}, // t
        {97, 8}, // a
        {111, 8}, // o
        {105, 7}, // i
        {110, 7}, // n
        {32, 7}, // space
        {115, 6}, // s
        {104, 6}, // h
        {114, 6}, // r
        {100, 4}, // d
        {108, 4}, // l
        {117, 3}, //u
        {69, 12}, // E
        {84, 9}, // T
        {65, 8}, // A
        {79, 8}, // O
        {73, 7}, // I
        {78, 7}, // N
        {83, 6}, // S
        {72, 6}, // H
        {82, 6}, // R
        {68, 4}, // D
        {76, 4}, // L
        {85, 3}, // U
    };

    std::uint32_t score = 0;
    for (std::uint32_t i = 0; i < xored.size(); ++i)
    {
        if (FreqTable.find(xored[i]) != FreqTable.end())
        {
            score += FreqTable.find(xored[i])->second;
        }
    }
    return score;
}

std::pair<std::uint32_t, std::uint32_t> best_value(const std::vector<std::uint32_t>& input)
{
    std::map<std::uint32_t, std::uint32_t> scores;
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
        [](const std::pair<std::uint32_t, std::uint32_t> f, const std::pair<std::uint32_t, std::uint32_t> s)
        {
            return f.second < s.second;
        });
    return *maxElement;
}

std::pair<std::uint32_t, std::uint32_t> get_key_and_score(std::vector<std::vector<std::uint32_t>>& lines, std::string hexString)
{
    std::vector<std::uint32_t> inputHex;
    inputHex.reserve(hexString.length() / 2);
    init_hex_vec(inputHex, hexString);
    lines.push_back(inputHex);
    return best_value(inputHex);
}

//Input: input.txt
int main(int argc, char** argv)
{
    std::string fileName = argv[1];
    std::ifstream file;
    file.open(fileName);
    std::vector<std::vector<std::uint32_t>> lines;
    std::string line;
    std::uint32_t index = 0;
    std::map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> scores;
    while (getline(file, line))
    {
        auto key_and_score = get_key_and_score(lines, line);
        scores[index++] = key_and_score;
    }
    file.close();
    auto maxElement = std::max_element(
        scores.begin(),
        scores.end(),
        [](const std::pair<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> f,
           const std::pair<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> s)
        {
            return f.second.second < s.second.second;
        });
    std::cout << std::hex << "key: 0x" << maxElement->second.first << std::endl;
    std::cout << "score: " << maxElement->second.second << std::endl;
    for (std::uint32_t i = 0; i < lines[maxElement->first].size(); ++i)
    {
        std::cout << static_cast<char>(lines[maxElement->first][i] ^ maxElement->second.first);
    }
    return 0;
}

