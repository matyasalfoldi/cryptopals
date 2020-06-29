#include <future>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <Windows.h>

const std::map<std::uint8_t, std::string> IndexTable = {
    {0b000000,"A"}, // 0
    {0b000001,"B"}, // 1
    {0b000010,"C"}, // 2
    {0b000011,"D"}, // 3
    {0b000100,"E"}, // 4
    {0b000101,"F"}, // 5
    {0b000110,"G"}, // 6
    {0b000111,"H"}, // 7
    {0b001000,"I"}, // 8
    {0b001001,"J"}, // 9
    {0b001010,"K"}, // 10
    {0b001011,"L"}, // 11
    {0b001100,"M"}, // 12
    {0b001101,"N"}, // 13
    {0b001110,"O"}, // 14
    {0b001111,"P"}, // 15
    {0b010000,"Q"}, // 16
    {0b010001,"R"}, // 17
    {0b010010,"S"}, // 18
    {0b010011,"T"}, // 19
    {0b010100,"U"}, // 20
    {0b010101,"V"}, // 21
    {0b010110,"W"}, // 22
    {0b010111,"X"}, // 23
    {0b011000,"Y"}, // 24
    {0b011001,"Z"}, // 25
    {0b011010,"a"}, // 26
    {0b011011,"b"}, // 27
    {0b011100,"c"}, // 28
    {0b011101,"d"}, // 29
    {0b011110,"e"}, // 30
    {0b011111,"f"}, // 31
    {0b100000,"g"}, // 32
    {0b100001,"h"}, // 33
    {0b100010,"i"}, // 34
    {0b100011,"j"}, // 35
    {0b100100,"k"}, // 36
    {0b100101,"l"}, // 37
    {0b100110,"m"}, // 38
    {0b100111,"n"}, // 39
    {0b101000,"o"}, // 40
    {0b101001,"p"}, // 41
    {0b101010,"q"}, // 42
    {0b101011,"r"}, // 43
    {0b101100,"s"}, // 44
    {0b101101,"t"}, // 45
    {0b101110,"u"}, // 46
    {0b101111,"v"}, // 47
    {0b110000,"w"}, // 48
    {0b110001,"x"}, // 49
    {0b110010,"y"}, // 50
    {0b110011,"z"}, // 51
    {0b110100,"0"}, // 52
    {0b110101,"1"}, // 53
    {0b110110,"2"}, // 54
    {0b110111,"3"}, // 55
    {0b111000,"4"}, // 56
    {0b111001,"5"}, // 57
    {0b111010,"6"}, // 58
    {0b111011,"7"}, // 59
    {0b111100,"8"}, // 60
    {0b111101,"9"}, // 61
    {0b111110,"+"}, // 62
    {0b111111,"/"}  // 63
};

void init_hex_vec(std::vector<std::uint8_t>& hex, const std::string& hexString)
{
    for (std::uint32_t i = 0; i < hexString.length(); ++i)
    {
        hex.push_back(std::stoi(hexString.substr(i,1), nullptr, 16));
    }
}

std::uint32_t get_core_num()
{
    SYSTEM_INFO sysInfo;
    GetNativeSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

std::string get_base64_chars(std::uint8_t firstNum, std::uint8_t secondNum, std::uint8_t thirdNum)
{
    /*
    Example:
    base 64: 6 bit

    x: 1111
    y: 1111
    z: 1111

    first:(x<<2)+(y>>2)
    second:((y&3)<<4)+z

    first:
    (x<<2):111100
    (y>>2):000011
    +
    =111111

    second:
    (y&000011):000011
    (^<<4):110000
    +z:
    111111
    */
    std::uint8_t firstChar  = (firstNum<<2)+(secondNum>>2);
    std::uint8_t secondChar = ((secondNum&3)<<4)+thirdNum;
    return IndexTable.find(firstChar)->second + IndexTable.find(secondChar)->second;
}

std::string calc_base64(const std::vector<std::uint8_t>& hex, const std::uint32_t coreNum)
{
    std::string result = "";
    std::vector<std::future<std::string>> results(coreNum);

    bool finished = false;
    std::uint32_t hexInd = 0;
    while (!finished)
    {
        std::uint32_t threadsStarted = 0;
        for (std::uint32_t threadInd = 0; 
            threadInd < coreNum && hexInd+2 < hex.size();
            ++threadInd,hexInd+=3)
        {
            ++threadsStarted;
            results[threadInd] = std::async(get_base64_chars, hex[hexInd], hex[hexInd + 1], hex[hexInd + 2]);
        }

        if (hexInd >= hex.size())
            finished = true;

        for (std::uint32_t threadInd = 0; threadInd < threadsStarted; ++threadInd)
        {
            result += results[threadInd].get();
        }
    }
    return result;
}

int main(int argc, char** argv)
{
    std::vector<std::uint8_t> hex;
    std::string hexString = argv[1];
    hex.reserve(strlen(argv[1]));
    if (argc > 1)
    {
        init_hex_vec(hex, hexString);
        auto coreNum = get_core_num();
        std::cout << calc_base64(hex, coreNum) << std::endl;
    }
}
