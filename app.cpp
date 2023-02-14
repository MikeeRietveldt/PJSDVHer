#include <iostream>

#include "server/server.hpp"
#include "controller/controller.hpp"
#include "controller/actions.hpp"

int main(int argc, char const *argv[]){
    // Create the apartment
    Apartment apartment;

    // Create the server
    Server server(&apartment);

    // Initialize the server
    server.init();

    while(true){
        // Check the socket for a connection
        server.checkSocket();
    }
    return 0;
}