#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

class Parser
{
    public:
        std::vector<std::string> values;

        /**
         * @brief Splits data on whitespace
        */
        void parse(char *buffer); //We will end up with a group of characters followed by a whitespace and so on. (no dots or comma's).
};

#endif // !PARSER_H
