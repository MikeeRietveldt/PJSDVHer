#pragma once

#include "../controller/actions.hpp"

class Stoel {
public:
    /**
     * @brief Create a new door
    */
    Stoel(uint16_t fileDescriptor);

    /**
     * @brief Confirm that the chair is allowed to fibrate
    */
    void trillenAan() const { sendMessage(this->fd, "3"); }

    /**
     * @brief Confirm that the chair is allowed to stop fibrating
    */
    void trillenUit() const { sendMessage(this->fd, "4"); }

    /**
     * @brief Confirm that the led is allowed to turn on
    */
    void ledAan() const { sendMessage(this->fd, "1"); }

    /**
     * @brief Confirm that the chair is done with a fibration cycle
    */
    void stoelTrilCyclus() const { sendMessage(this->fd, "5"); }

    /**
     * @brief Confirm that the fibration button of the chair has been pressed
    */
    void stoelKnopTril() const { sendMessage(this->fd, "2"); }

    /**
     * @brief Return the file desciptor for sending data
    */
    int getFileDescriptor() const{ return fd; }
private:
    int fd;
};
