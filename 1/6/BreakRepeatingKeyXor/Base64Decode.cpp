#include "Base64Decode.h"
#include <iostream>
const std::map<std::string, std::uint32_t> ReverseIndexTable = {
    {"A", 0b000000}, // 0
    {"B", 0b000001}, // 1
    {"C", 0b000010}, // 2
    {"D", 0b000011}, // 3
    {"E", 0b000100}, // 4
    {"F", 0b000101}, // 5
    {"G", 0b000110}, // 6
    {"H", 0b000111}, // 7
    {"I", 0b001000}, // 8
    {"J", 0b001001}, // 9
    {"K", 0b001010}, // 10
    {"L", 0b001011}, // 11
    {"M", 0b001100}, // 12
    {"N", 0b001101}, // 13
    {"O", 0b001110}, // 14
    {"P", 0b001111}, // 15
    {"Q", 0b010000}, // 16
    {"R", 0b010001}, // 17
    {"S", 0b010010}, // 18
    {"T", 0b010011}, // 19
    {"U", 0b010100}, // 20
    {"V", 0b010101}, // 21
    {"W", 0b010110}, // 22
    {"X", 0b010111}, // 23
    {"Y", 0b011000}, // 24
    {"Z", 0b011001}, // 25
    {"a", 0b011010}, // 26
    {"b", 0b011011}, // 27
    {"c", 0b011100}, // 28
    {"d", 0b011101}, // 29
    {"e", 0b011110}, // 30
    {"f", 0b011111}, // 31
    {"g", 0b100000}, // 32
    {"h", 0b100001}, // 33
    {"i", 0b100010}, // 34
    {"j", 0b100011}, // 35
    {"k", 0b100100}, // 36
    {"l", 0b100101}, // 37
    {"m", 0b100110}, // 38
    {"n", 0b100111}, // 39
    {"o", 0b101000}, // 40
    {"p", 0b101001}, // 41
    {"q", 0b101010}, // 42
    {"r", 0b101011}, // 43
    {"s", 0b101100}, // 44
    {"t", 0b101101}, // 45
    {"u", 0b101110}, // 46
    {"v", 0b101111}, // 47
    {"w", 0b110000}, // 48
    {"x", 0b110001}, // 49
    {"y", 0b110010}, // 50
    {"z", 0b110011}, // 51
    {"0", 0b110100}, // 52
    {"1", 0b110101}, // 53
    {"2", 0b110110}, // 54
    {"3", 0b110111}, // 55
    {"4", 0b111000}, // 56
    {"5", 0b111001}, // 57
    {"6", 0b111010}, // 58
    {"7", 0b111011}, // 59
    {"8", 0b111100}, // 60
    {"9", 0b111101}, // 61
    {"+", 0b111110}, // 62
    {"/", 0b111111}  // 63
};


std::vector<std::uint32_t> get_hex(std::string firstChar, std::string secondChar)
{
    std::vector<std::uint32_t> hex;
    if (secondChar == "=")
    {
        hex.push_back(ReverseIndexTable.find(firstChar)->second>>2);
    }
    else
    {
        //firstChar = 4 byte firstNum 2 byte secondNum
        //secondChar = 2 byte secondNum 4 byte thirdNum
        hex.push_back((ReverseIndexTable.find(firstChar)->second & 0b111100) >> 2);
        hex.push_back(((ReverseIndexTable.find(firstChar)->second & 0b000011) << 2) | ((ReverseIndexTable.find(secondChar)->second & 0b110000) >> 4));
        hex.push_back(ReverseIndexTable.find(secondChar)->second & 0b001111);
    }
    return hex;
}

std::vector<std::uint32_t> convert_base64_to_hex(const std::string base64)
{
    //2 base64 char = 3 hex
    std::vector<std::uint32_t> results;    
    for (std::uint32_t i = 0; i+1 < base64.length(); i+=2)
    {
        auto result = get_hex(base64.substr(i, 1), base64.substr(i + 1, 1));
        results.insert(results.end(), result.begin(), result.end());
    }

    return results;
}

