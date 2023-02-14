#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstdint>
#include <vector>
#include <string>

#include "actions.hpp"

/**
 * @brief Handles a new connection
 * @param fd The new connection
*/
void handleServerConnection(uint16_t fd);

/**
 * @brief Handles a device disconnecting from the server
 * @param fd The new connection
*/
void handleServerDisconnect(uint16_t fd);

/**
 * @brief Handles input from the server
 * @param apartment The apartment belonging to the server
 * @param fd The connection the server received input from
 * @param buffer The received input
*/
void handleServerInput(Apartment *apartment, uint16_t fd, char *buffer);

#endif // !CONTROLLER_H
