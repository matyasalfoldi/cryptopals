#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Base64Decode.h"

extern std::vector<std::uint32_t> convert_base64_to_hex(const std::string base64);
extern std::uint32_t best_value(const std::vector<std::uint32_t>& input);

std::uint32_t hamming_dist(std::vector<std::uint32_t> f, std::vector<std::uint32_t> s)
{
    std::uint32_t diff = 0;
    for (std::uint32_t i = 0; i < f.size(); ++i)
    {
        std::uint32_t xoredVal = f[i] ^ s[i];
        while (xoredVal != 0)
        {
            diff += xoredVal & 1;
            xoredVal >>= 1;
        }
    }
    return diff;
}

std::vector<std::uint32_t> get_key_len(const std::vector<std::uint32_t>& hex)
{
    std::uint32_t minKeyLen = 2;
    std::uint32_t maxKeyLen = std::min<std::uint32_t>(40, hex.size()/2);
    std::map<std::uint32_t, double> keyLenScores;
    for (std::uint32_t keyLen = minKeyLen; keyLen <= maxKeyLen; ++keyLen)
    {
        double dist = 0.0;
        std::uint32_t c = 0;
        for (std::uint32_t i = 0; i+(2*keyLen) < hex.size(); i+=2*keyLen)
        {
            std::vector<std::uint32_t> first(hex.cbegin() + i,
                                             hex.cbegin() + i + keyLen);
            std::vector<std::uint32_t> second(hex.cbegin() + i + keyLen,
                                              hex.cbegin() + i + (2 * keyLen));
            dist = hamming_dist(first,second) / static_cast<double>(keyLen);
            ++c;
        }
        keyLenScores[keyLen] = dist / (double)c;
    }
    std::vector<std::uint32_t> keyLens;
    double minValue = (std::min_element(keyLenScores.cbegin(), keyLenScores.cend(),
                    [](const std::pair<std::uint32_t, double>& f,
                       const std::pair<std::uint32_t, double>& s)
                    {
                        return f.second < s.second;
                    }))->second;
    for (auto keyLenScore : keyLenScores)
    {
        if (keyLenScore.second == minValue)
            keyLens.push_back(keyLenScore.first);
    }
    return keyLens;
}

void read_base64_input(std::string fileName, std::vector<std::string>& lines)
{
    std::ifstream file;
    file.open(fileName);
    std::string line;
    std::uint32_t index = 0;
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();
}

std::vector<std::vector<std::uint32_t>> create_blocks(const std::vector<std::uint32_t>& hexValues, std::uint32_t blockSize)
{
    std::uint32_t blocks = hexValues.size() % blockSize == 0 ? hexValues.size() / blockSize : (hexValues.size() / blockSize) + 1;
    std::vector<std::vector<std::uint32_t>> hexBlocks(blocks);
    int counter = 0;
    for (std::uint32_t i = 0; i+1 < hexValues.size(); i += blockSize)
    {
        std::uint32_t end = (i + blockSize) < hexValues.size() ? i + blockSize : hexValues.size();
        hexBlocks[counter].insert(hexBlocks[counter].end(), hexValues.cbegin() + i, hexValues.cbegin() + end);
        ++counter;
    }

    return hexBlocks;
}

std::vector<std::vector<std::uint32_t>> transpose_blocks(const std::vector<std::vector<std::uint32_t>>& hexBlocks, std::uint32_t keyLen)
{
    std::vector<std::vector<std::uint32_t>> transposedHexBlocks(keyLen);
    for (std::uint32_t i = 0; i < hexBlocks.size(); ++i)
    {
        for (std::uint32_t j = 0; j < hexBlocks[i].size(); ++j)
        {
            transposedHexBlocks[j].push_back(hexBlocks[i].at(j));
        }
    }
    return transposedHexBlocks;
}

void rep_key_xor(const std::vector<std::uint32_t>& hexValues, const std::vector<std::uint32_t>& key)
{
    for (std::uint32_t i = 0; i < hexValues.size(); ++i)
    {
        std::cout << static_cast<char>(hexValues[i] ^ key[i % key.size()]);
    }
}

int main(int argc, char** argv)
{
    // Don't remove newlines, might be the cause
    std::vector<std::string> lines;
    read_base64_input(argv[1], lines);
    std::vector<std::uint32_t> hexValues;
    for (auto line : lines)
    {
        auto result = convert_base64_to_hex(line);
        hexValues.insert(hexValues.end(), result.begin(), result.end());
    }
    auto keyLen = get_key_len(hexValues);
    for (std::uint32_t j = 0; j < keyLen.size(); ++j)
    {
        std::cout << "#######keyLen:" << keyLen[j] << "#######" << std::endl;
        std::vector<std::vector<std::uint32_t>> hexBlocks = create_blocks(hexValues, keyLen[j]);
        std::vector<std::vector<std::uint32_t>> transposedHexBlocks = transpose_blocks(hexBlocks, keyLen[j]);
        std::vector<std::uint32_t> key(keyLen[j]);
        for (std::uint32_t i = 0; i < keyLen[j]; ++i)
        {
            key[i] = best_value(transposedHexBlocks[i]);
        }
        rep_key_xor(hexValues, key);
        std::cout << std::endl;
    }
    return 0;
}
