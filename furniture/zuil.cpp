#include "zuil.h"

// Initialize the file descriptor for the column
Zuil::Zuil(uint16_t fileDescriptor): fd(fileDescriptor) {
}

void Zuil::zuilBrand() const {
    // Send a message for the fire
    const char *message = "8";
    send(this->fd, message, strlen(message), 0);
}
