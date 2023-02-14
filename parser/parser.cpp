#include "parser.hpp"

#include <iostream>
#include <string>
#include <sstream>

void Parser::parse(char *buffer)
{
    std::string input = buffer;
    std::istringstream iss(input);

    //Add every single word to Vector
    std::string sub;
    while(iss >> sub)
    {
        values.push_back(sub);
    }
}