#include "StringUtils.hpp"

std::vector<std::string> SplitString(std::string str, char delimiter)
{
    std::vector<std::string> parts;
    std::string buffer;

    for (char c : str)
    {
        if (c == delimiter)
        {
            parts.push_back(buffer);
            buffer = "";
        }
        else
            buffer += c;
    }

    parts.push_back(buffer);
    return parts;
}