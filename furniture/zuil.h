#pragma once

#include "../controller/actions.hpp"

class Zuil {
public:
    /**
     * @brief Creates a new column
    */
    Zuil(uint16_t fileDescriptor);

    /**
     * @brief Confirm that the doorbell has been pressed
    */
    void deurBel() const { sendMessage(this->fd, "1"); }

    /**
     * @brief Confirm a fire at the column
    */
    void zuilBrand() const;

    /**
     * @brief Confirm the absense of a fire at the column
    */
    void geenBrand() const { sendMessage(this->fd, "9"); }

    /**
     * @brief Return the file descriptor for sending data to the column
    */
    int getFileDescriptor() const{ return fd; }
private:
    int fd;
    bool fire;
};
