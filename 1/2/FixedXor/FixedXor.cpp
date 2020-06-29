#include <iostream>
#include <string>
#include <vector>

void init_hex_vec(std::vector<std::uint8_t>& hex, const std::string& hexString)
{
    for (std::uint32_t i = 0; i < hexString.length(); ++i)
    {
        hex[i] = std::stoi(hexString.substr(i, 1), nullptr, 16);
    }
}

std::vector<std::uint8_t> Xor(const std::vector<std::uint8_t>& input, const std::vector <std::uint8_t>& xorKey)
{
    std::vector<uint8_t> result(input.size());
    for (std::uint32_t i = 0; i < input.size(); ++i)
    {
        result[i] = input[i] ^ xorKey[i];
    }
    return result;
}

//Input: 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965
int main(int argc, char** argv)
{
    std::uint32_t firstInputLen = strlen(argv[1]);
    std::uint32_t secondInputLen = strlen(argv[2]);
    if (argc == 3 && firstInputLen == secondInputLen)
    {
        std::vector<std::uint8_t> input(firstInputLen);
        std::vector<std::uint8_t> xorKey(secondInputLen);
        init_hex_vec(input,  std::string(argv[1]));
        init_hex_vec(xorKey, std::string(argv[2]));
        auto results = Xor(input, xorKey);
        for (std::uint32_t i = 0; i < results.size(); ++i)
            std::cout << std::hex << +results[i];
    }
    else
        std::cout << "No" << std::endl;
}

