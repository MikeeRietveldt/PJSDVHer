#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include "actions.hpp"

#include "../furniture/venster.h"
#include "../furniture/stoel.h"
#include "../furniture/bed.h"
#include "../furniture/deur.h"
#include "../furniture/zuil.h"
#include "../furniture/schemerlamp.h"

// Creates a new apartment, setting every piece of furniture to NULL
Apartment::Apartment() : bed(NULL), deur(NULL), schemerlamp(NULL), stoel(NULL), venster(NULL), zuil(NULL) {
}

void Apartment::meldBrand() const{
    // The message to send
    const char message[] = "8";

    // Send the message to the correct furniture, if available
    if(bed != NULL){
        sendMessage(bed->getFileDescriptor(), message);
    }
    if(deur != NULL){
        sendMessage(deur->getFileDescriptor(), message);
    }
    if(schemerlamp != NULL){
        sendMessage(schemerlamp->getFileDescriptor(), message);
    }
    if(stoel != NULL){
        sendMessage(stoel->getFileDescriptor(), message);
    }
    if(venster != NULL){
        sendMessage(venster->getFileDescriptor(), message);
    }
}

void Apartment::meldGeenBrand() const{
    // The message to send
    const char message[] = "9";

    // Send the message to the correct furniture, if available
    if(bed != NULL){
        sendMessage(bed->getFileDescriptor(), message);
    }
    if(deur != NULL){
        sendMessage(deur->getFileDescriptor(), message);
    }
    if(schemerlamp != NULL){
        sendMessage(schemerlamp->getFileDescriptor(), message);
    }
    if(stoel != NULL){
        sendMessage(stoel->getFileDescriptor(), message);
    }
    if(venster != NULL){
        sendMessage(venster->getFileDescriptor(), message);
    }
}

void Apartment::meldPatientHulp() const{
    // The message to send
    const char message[] = "4";

    // Send the message to the door, if available
    if(deur != NULL){
        sendMessage(deur->getFileDescriptor(), message);
    }
}

void Apartment::setBed(uint16_t fd) {
    // Delete the bed if there already is one
    if(bed != NULL){
        delete bed;
    }

    // Create a new bed
    bed = new Bed(fd);
}

void Apartment::setVenster(uint16_t fd) {
    // Delete the window if there already is one
    if(venster != NULL){
        delete venster;
    }
    // Create a new window
    venster = new Venster(fd);
}

void Apartment::setStoel(uint16_t fd) {
    // Delete the chair if there already is one
    if(stoel != NULL){
        delete stoel;
    }

    // Create a new chair
    stoel = new Stoel(fd);
}

void Apartment::setDeur(uint16_t fd) {
    // Delete the door if there already is one
    if(deur != NULL){
        delete deur;
    }

    // Create a new door
    deur = new Deur(fd);
}

void Apartment::setZuil(uint16_t fd) {
    // Delete the window if there already is one
    if(zuil != NULL){
        delete zuil;
    }

    // Create a new column
    zuil = new Zuil(fd);
}

void Apartment::setSchemerlamp(uint16_t fd) {
    // Delete the lamp if there already is one
    if(schemerlamp != NULL){
        delete schemerlamp;
    }

    // Create a new lamp
    schemerlamp = new Schemerlamp(fd);
}

void Apartment::processBed(const std::string& action) const {
    // Call the function corresponding to the action code
    if (strcmp(action.c_str(), "regelLed") == 0) {
        bed->regelLed();
    } else if (strcmp(action.c_str(), "hulpNodig") == 0) {
        bed->hulpNodig();

        // Notify devices that the client needs help
        meldPatientHulp();
    } else if(strcmp(action.c_str(), "afwezigheidBed") == 0) {
        bed->afwezigheidBed();
    } 
}

void Apartment::processVenster(const std::string& action) const {
    // Call the function corresponding to the action code
    if (strcmp(action.c_str(), "ldrRead") == 0) {
        venster->ldrRead();
    } else if (strcmp(action.c_str(), "potRead") == 0) {
        venster->potRead();
    } else if (strcmp(action.c_str(), "regelLcdPanel") == 0) {
        venster->regelLcdPanel();
    } else if (strcmp(action.c_str(), "regelVenster") == 0) {
        venster->regelVenster();
    } 
}

void Apartment::processStoel(const std::string& action) const {
    // Call the function corresponding to the action code
    if (strcmp(action.c_str(), "trillenAan") == 0) {
        stoel->trillenAan();
    } else if (strcmp(action.c_str(), "trillenUit") == 0) {
        stoel->trillenUit();
    } else if(strcmp(action.c_str(), "ledAan") == 0) {
        stoel->ledAan();
    } else if(strcmp(action.c_str(), "stoelTrilCyclus") == 0) {
        stoel->stoelTrilCyclus();
    } else if(strcmp(action.c_str(), "stoelKnopTril") == 0) {
        stoel->stoelKnopTril();
    } 
}

void Apartment::processDeur(const std::string& action) const {
    // Call the function corresponding to the action code
    if (strcmp(action.c_str(), "deurOpenBinnen") == 0) {
        deur->deurOpenBinnen();
    } else if (strcmp(action.c_str(), "deurOpenBuiten") == 0) {
        deur->deurOpenBuiten();
    } else if(strcmp(action.c_str(), "brand") == 0) {
        deur->brand();
        meldBrand();
    }
}

void Apartment::processZuil(const std::string& action) const {
    // Call the function corresponding to the action code
    if (strcmp(action.c_str(), "deurBel") == 0) {
        zuil->deurBel();
    } else if (strcmp(action.c_str(), "zuilBrand") == 0) {
        zuil->zuilBrand();
        // Notify devices there is a fire
        meldBrand();
    } else if(strcmp(action.c_str(), "geenBrand") == 0) {
        zuil->geenBrand();
        meldGeenBrand();
    }
}

void Apartment::processSchemerlamp(const std::string& action) const {
    // Call the function corresponding to the action code
    if (strcmp(action.c_str(), "ledOn") == 0) {
        schemerlamp->ledOn();
    } else if (strcmp(action.c_str(), "ledOff") == 0) {
        schemerlamp->ledOff();
    } else if(strcmp(action.c_str(), "brand") == 0) {
        schemerlamp->brand();
    } else if(strcmp(action.c_str(), "leesBeweging") == 0) {
        schemerlamp->leesBeweging();
    }
}

void Apartment::handleRequest(uint16_t fd, const std::string& action){
    // Process the request if the fd corresponds to an existing device
    if(bed != NULL && fd == bed->getFileDescriptor()){
        processBed(action);
    }else if(deur != NULL && fd == deur->getFileDescriptor()){
        processDeur(action);
    }else if(schemerlamp != NULL && fd == schemerlamp->getFileDescriptor()){
        processSchemerlamp(action);
    }else if(stoel != NULL && fd == stoel->getFileDescriptor()){
        processStoel(action);
    }else if(venster != NULL && fd == venster->getFileDescriptor()){
        processVenster(action);
    }else if(zuil != NULL && fd == zuil->getFileDescriptor()){
        processZuil(action);
    }
    // Create a new device, if the message requests to do so
    else if(action == "setBed"){
        setBed(fd);
    }else if(action == "setVenster"){
        setVenster(fd);
    }else if(action == "setStoel"){
        setStoel(fd);
    }else if(action == "setDeur"){
        setDeur(fd);
    }else if(action == "setZuil"){
        setZuil(fd);
    }else if(action == "setSchemerlamp"){
        setSchemerlamp(fd);
    }
    // Display a warning if an invalid message was sent
    else{
        std::cerr << "Unexpected action: " << action << " by " << fd << "\n";
    }
}

void sendMessage(uint16_t fd, const char* message) {
    // Send a message
    send(fd, message, strlen(message), 0);
}
