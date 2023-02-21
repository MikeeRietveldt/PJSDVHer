#include "parser.hpp"

#include <iostream>
#include <string>
#include <sstream>

void Parser::parse(char *buffer)
{
    std::string input = buffer; //Convert buffer into C++ string.
    std::istringstream iss(input); //Safe way of dynamically reading other  datatypes (ints, doubles, floats etc.) from a string.

    //Add every single word to Vector
    std::string sub; //We will use this for the While loop.
    while(iss >> sub) //Keeps going till there are no words left.
    {
        values.push_back(sub); //Add this at the end of the vector. So we can later retrieve it.
    }
}
