#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::uint32_t> convert_line_to_hex(std::string line)
{
    std::vector<std::uint32_t> hex(line.length());
    for (std::uint32_t i = 0; i < hex.size(); ++i)
    {
        hex[i] = line[i];
    }
    return hex;
}

void convert_lines_to_hex(std::vector<std::vector<std::uint32_t>>& lines, std::string fileName)
{
    std::ifstream file;
    file.open(fileName);
    std::string line;
    while (getline(file, line))
    {
        lines.push_back(convert_line_to_hex(line));
    }
    file.close();
}

//Input input.txt key
int main(int argc, char** argv)
{
    std::vector<std::vector<std::uint32_t>> lines;
    convert_lines_to_hex(lines, argv[1]);
    std::vector<std::uint32_t> key = convert_line_to_hex(argv[2]);
    std:uint32_t i = 0;
    for (auto line : lines)
    {
        if (i != 0)
            std::cout << ('\n' ^ key[i++ % key.size()]);
        for (std::uint32_t j = 0; j < line.size(); ++j,++i)
        {
            std::cout << std::setfill('0') << std::setw(2) << std::right << std::hex << (line[j] ^ key[i%key.size()]);
        }
    }
    return 0;
}
