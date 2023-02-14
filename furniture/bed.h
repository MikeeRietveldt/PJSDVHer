#pragma once

#include <cstdint>

class Bed {
public:
    /**
     * @brief Creates a new bed
     * @param fileDescriptor The connection with the bed
    */
    Bed(uint16_t fileDescriptor);

    /**
     * @brief Toggles the LED of the bed
    */
    void regelLed();

    /**
     * @brief Sends a response for a request for help
    */
    void hulpNodig() const;

    /**
     * @brief Sends a message for if the client left the bed
    */
    void afwezigheidBed() const;

    /**
     * @brief Returns the file descriptor, for sending data to the bed
    */
    int getFileDescriptor() const{ return fd; }
private:
    int fd;
    bool statusLed;
};
