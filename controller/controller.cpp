#include <iostream>
#include <cstring>
#include <map>
#include <sys/socket.h>

#include "controller.hpp"
#include "../parser/parser.hpp"

void handleServerConnection(uint16_t fd)
{
    // Display the connected device
    std::cout << "Controller: got connection from " << fd << std::endl;
}

void handleServerDisconnect(uint16_t fd)
{  
    // Display the connected device
    std::cout << "Controller: got disconnect from " << fd << std::endl;
}

void handleServerInput(Apartment *apartment, uint16_t fd, char *buffer)
{
    // Display the received input and the sender
    std::cout << "Got input '" << buffer << "' from " << fd << ".\n";

    // Parse the input
    Parser prs;
    prs.parse(buffer);

    // Retrieve the action code
    std::string key;
    try
    {
        key = prs.values.at(0);
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << "no action" << std::endl;
    }

    // Handle the request
    apartment->handleRequest(fd, key);

    // Clear the parsed data
    prs.values.clear();
}

