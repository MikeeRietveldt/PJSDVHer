#include "server.hpp"

#include "../controller/controller.hpp"

// define DEBUG for debug infomation

// Initialize the apartment and saving the adress of the apartment so we can communicate with it later
Server::Server(Apartment *apartmentPointer) : apartment(apartmentPointer)
{
    // Setup the server with the default port
    setup(DEFAULT_PORT);
}

// Initialize the apartment
Server::Server(Apartment *apartmentPointer, int port) : apartment(apartmentPointer)
{
    // Setup the server with a customn port
    setup(port);
}

Server::~Server()
{
    // To debug simply define DEBUG at the top.
    #ifdef DEBUG
    // Display the server is being destroyed
    printf("[SERVER] [DESTRUCTOR] Destroying Server.");
    #endif

    // Close the port of the server
    close(server_fd);
}

void Server::setup(int port)
{
    //Create an endpoint for communication.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket setup failed");
    }

    // Initialize the masterfds and tempfds
    FD_ZERO(&masterfds);
    FD_ZERO(&tempfds);

    // Set every property of the serveraddress to 0
    memset(&servaddr, 0, sizeof(servaddr));

    // Initialize the server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
}

void Server::initSocket()//Set the socket options
{
    #ifdef DEBUG
    // Display the socket of the server is being initialized
    printf("[SERVER] Initiating socket.\n");
    #endif

    // Set the opt value
    int opt = 1;

    // Set the socket options
    int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Testing if it works by putting it in ret.

    #ifdef DEBUG
    // Display the return value of setsockopt, to check whether it failed
    printf("[SERVER] setsocketopt() return value: %d\n", ret);
    #endif // DEBUG

    // Exit with an error if it failed
    if (ret < 0)
    {
        perror("setsocketopt() failed");
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocket()
{
    #ifdef DEBUG
    // Display that the server is binding to a socket
    std::cout << "[SERVER] binding\n";
    #endif // DEBUG

    // Bind the server to the socket
    int ret = bind(server_fd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    #ifdef DEBUG
    // Display the return value of bind
    printf("[SERVER] bind() return value: %d\n", ret);
    #endif // DEBUG

    // Print an error message if it failed
    if (ret < 0)
    {
        perror("bind() failed");
    }

    // Set the masterfds to the servers file descriptor
    FD_SET(server_fd, &masterfds);

    // Set the max_sd to the servers file descriptor
    max_sd = server_fd;
}

void Server::listenSocket()
{
    #ifdef DEBUG
    // Display that the server is listening for a connection
    std::cout << "[SERVER] listen starting\n";
    #endif // DEBUG

    // Create a queue for upto 3 connections
    int ret = listen(server_fd, 3);

    #ifdef DEBUG
    // Display the value returned by listen
    printf("[SERVER] listen() return value: %d.\n", ret);
    #endif // DEBUG

    // Print an error if it failed
    if (ret < 0)
    {
        perror("listen() failed");
    }
}

void Server::shutdown()
{
    // Close the socket for the server
    int ret = close(server_fd);

    #ifdef DEBUG
    // Print the value returned by the close function
    printf("Closing server_fd return value: %d\n", ret);
    #endif // DEBUG
}

void Server::newConnection()
{
    #ifdef DEBUG
    // Display that the server is waiting for a new connection
    std::cout << "[SERVER] [CONNECTION] new connection\n";
    #endif // DEBUG

    // Clear the client address
    memset(&client_addr, 0, sizeof(client_addr));

    // accpet a new request
    socklen_t addrlen = sizeof(client_addr);
    new_socket = accept(server_fd, (struct sockaddr*) &client_addr, &addrlen);

    // Print an error if it failed
    if (new_socket < 0)
    {
        perror("[SERVER] accept() failed");
    } else
    {
        // Set the masterfds to the new socket
        FD_SET(new_socket, &masterfds);

        // Set the max socket desciptors to the new socket, if the new socket is higher
        if (new_socket > max_sd)
        {
            max_sd = new_socket;

            #ifdef DEBUG
            // Display that the server has changed the max file descriptor
            std::cout << "[SERVER] incrementing max_sd to " << max_sd << std::endl;
            #endif // DEBUG
        }

        #ifdef DEBUG
        // Print the socket connected to the server
        printf("[SERVER] New connection on socket '%d'.\n", new_socket);
        #endif // DEBUG
    }

    // Handle the request
    handleServerConnection(new_socket);
}

void Server::recvInputFromConnection(int fd)
{
    // Receive data
    int checkrecv = recv(fd, buffer, BUFFER_SIZE, 0);

    if (checkrecv <= 0)
    {
        // If nothing has been received
        if (0 == checkrecv)
        {
            #ifdef DEBUG
            // Print the port that has been or will be disconnected
            printf("[SERVER] [DISCONNECT] connection '%d' disconnected\n", fd);
            #endif // DEBUG

            // Handle the disconnect
            handleServerDisconnect(fd);

            // Close the file descriptor
            close(fd);

            // Clear the file descriptor
            FD_CLR(fd, &masterfds);
            return;
        } else
        {
            // Print an error message
            perror("[SERVER] [ERROR] recv() failed");
        }

        // Close the file descriptor
        close(fd);

        // Clear the file descriptor
        FD_CLR(fd, &masterfds);
        return;
    }
    buffer[checkrecv] = '\0';

    #ifdef DEBUG
    // Print the data received from the client
    printf("[SERVER] [RECV] Recieved '%s' from client.\n", buffer);
    #endif // DEBUG

    // Handle the received data

    handleServerInput(apartment, fd, buffer);

    // Clear the buffer
    bzero(&buffer, BUFFER_SIZE);
}

void Server::checkSocket()//Check for change on socket connections
{
    // set the temporary file descriptor to the master file descriptor
    tempfds = masterfds;

    #ifdef DEBUG
    // Display the max descriptor
    printf("[SERVER] max sd = '%hu' \n", max_sd);

    // Display that select is going to be called
    std::cout << "[SERVER] calling select()\n";
    #endif // DEBUG

    // Select the next file descriptor
    int sel = select(max_sd + 1, &tempfds, NULL, NULL, NULL);

    // If it failed, print an error message and exit
    if (sel < 0 )
    {
        perror("[SERVER] [ERROR] select() failed");
        exit(EXIT_FAILURE);
    }

    // iterate through the sd's
    for (int i = 0; i <= max_sd; i++)
    {
        // If the sd is set
        if (FD_ISSET(i, &tempfds))
        {
            // And the server_fd is equal to the current index
            if (server_fd == i)
            {
                // create a new connection
                newConnection();
            } else
            {
                // If it isn't equal to the current index, receive input from that connection
                recvInputFromConnection(i);
            }
        }
    }
}

void Server::init()//initialise socket
{
    // Initialize, bind, and listen on the socket
    initSocket();
    bindSocket();
    listenSocket();
}
