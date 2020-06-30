#include <algorithm>
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

std::uint32_t best_value(const std::vector<std::uint32_t>& input)
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
    return maxElement->first;
}

//Input: 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736
int main(int argc, char** argv)
{
    std::string input = argv[1];
    std::vector<std::uint32_t> inputHex;
    inputHex.reserve(input.length()/2);
    init_hex_vec(inputHex, input);
    auto key = best_value(inputHex);
    std::cout << std::hex << "key: 0x" << key << std::endl;
    for (std::uint32_t i = 0; i < inputHex.size(); ++i)
    {
        std::cout << static_cast<char>(inputHex[i] ^ key);
    }
    return 0;
}
