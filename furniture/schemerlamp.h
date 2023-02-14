#pragma once

#include "../controller/actions.hpp"

class Schemerlamp {
public:
    /**
     * @brief Creates a new lamp
    */
    Schemerlamp(uint16_t fileDescriptor);

    /**
     * @brief Confirm that the led can turn on
    */
    void ledOn() const { sendMessage(this->fd, "1"); }

    /**
     * @brief Confirm that the led can turn off
    */
    void ledOff() const { sendMessage(this->fd, "2"); }

    /**
     * @brief Confirm there is a fire
    */
    void brand() const { sendMessage(this->fd, "8"); }

    /**
     * @brief Confirm the detection of motion
    */
    void leesBeweging() const { sendMessage(this->fd, "4"); }

    /**
     * @brief return the file descriptor for sending data
    */
    int getFileDescriptor() const{ return fd; }
private:
    int fd;
};
