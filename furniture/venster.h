#pragma once

#include "../controller/actions.hpp"

class Venster{
public:
    /**
     * @brief Create a new window
    */
    Venster(uint16_t fileDescriptor);

    /**
     * @brief Confirm the reading of the ldr
    */
    void ldrRead() const { sendMessage(this->fd, "1"); }

    /**
     * @brief Confirm the reading of the potmeter
    */
    void potRead() const { sendMessage(this->fd, "2"); }

    /**
     * @brief Confirm the change of the LCD
    */
    void regelLcdPanel() const { sendMessage(this->fd, "1"); }

    /**
     * @brief Confirm the change of the window
    */
    void regelVenster() const { sendMessage(this->fd, "2"); }

    /**
     * @brief Return the file descriptor for sending data
    */
    int getFileDescriptor() const{ return fd; }
private:
    int fd;
};
