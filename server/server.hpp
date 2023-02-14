/*
* File: server.h
* Author: M0rga-n
*
* Created on December 9, 2022, 11:16 AM
*/

#ifndef SERVER_H
#define SERVER_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#include "../controller/actions.hpp"

#define BUFFER_SIZE 100 //Budder size for output
#define DEFAULT_PORT 8080 // Default port

class Server
{
public:
    /**
     * @brief Creates and start a new server
     * @param apartmentPointer The apartment for this server
    */
    Server(Apartment *apartmentPointer);

    /**
     * @brief Creates and start a new server on a customn port
     * @param apartmentPointer The apartment for this server
     * @param port The port to connect to
    */
    Server(Apartment *apartmentPointer, int port);

    /**
     * @brief Destroy server
    */
    ~Server();

    /**
     * @brief Initialize the server
    */
    void init();
    
    /**
     * @brief Shut the server down
    */
    void shutdown();

    /**
     * @brief Checks whether the port and all connections on it are valid and whether there is a message available
    */
    void checkSocket();
private:
    Apartment *apartment;
    fd_set masterfds;
    fd_set tempfds;

    uint16_t max_sd;

    //socket that recieves connections
    int server_fd;
    int new_socket;

    //server adderess details
    struct sockaddr_in servaddr;
    struct sockaddr_storage client_addr;

    // The buffer to receive data in
    char buffer[BUFFER_SIZE];

    /**
     * @brief Setup the server
     * @param port The port to listen on
    */
    void setup(int port);

    /**
     * @brief Initialize the socket
    */
    void initSocket();

    /**
     * @brief Bind the socket
    */
    void bindSocket();

    /**
     * @brief Listen on the socket
    */
    void listenSocket();

    /**
     * @brief Start a new connection
    */
    void newConnection();

    /**
     * @brief Receives input from the connection
     * @param fd The file descriptor to receive data on
    */
    void recvInputFromConnection(int fd);
};

#endif // !SERVER_H
