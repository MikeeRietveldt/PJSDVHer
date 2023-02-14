#include "bed.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>

// Initialize the file descriptor and status led for the bed
Bed::Bed(uint16_t fileDescriptor): fd(fileDescriptor), statusLed(false){
}

void Bed::regelLed() {
    // Toggle the LED
    if (statusLed) {
        const char *message = "2"; 
        send(fd, message, strlen(message), 0);
        statusLed = false;
    }else{
        const char* message = "1"; 
        send(fd, message, strlen(message), 0);
        statusLed = true;
    }
}

void Bed::hulpNodig() const{
    // Send a 3 to confirm the request
    const char *message = "3";
    send(fd, message, strlen(message), 0);
}

void Bed::afwezigheidBed() const {
    // Display that the client fell out of the bed
    std::cout << "Mary is uit bed gevallen"  << std::endl;
}
