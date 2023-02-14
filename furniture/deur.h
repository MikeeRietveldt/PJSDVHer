#pragma once

#include "../controller/actions.hpp"

class Deur {
public:
    /**
     * @brief Creates a door
    */
    Deur(uint16_t fileDescriptor);

    /**
     * @brief Confirm that the door is allowed to open/close
    */
    void deurOpenBinnen() const { sendMessage(this->fd, "1"); }

    /**
     * @brief Confirm that the door is allowed to open/close
    */
    void deurOpenBuiten() const { sendMessage(this->fd, "2"); }

    /**
     * @brief Send a confirmation for the fire
    */
    void brand() const { sendMessage(this->fd, "8"); }

    /**
     * @brief Returns the file descriptor for sending messages
    */
    int getFileDescriptor() const{ return fd; }
private:
    int fd;
};
