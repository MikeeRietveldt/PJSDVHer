#ifndef ACTIONS_H
#define ACTIONS_H

#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>

class Venster;
class Stoel;
class Bed;
class Deur;
class Zuil;
class Schemerlamp;

void sendMessage(uint16_t fd, const char* message);

class Apartment {
private:
    Venster *venster;
    Stoel *stoel;
    Bed *bed;
    Deur *deur;
    Zuil *zuil;    
    Schemerlamp *schemerlamp;
public:
    /**
     * @brief Creates an apartment.
    */
    Apartment();

    /**
     * @brief Notifies devices of a fire, so that the devices can react correctly.
    */
    void meldBrand() const;

    /**
     * @brief Notifies devices there is no fire, so that the devices can return to normal.
    */
    void meldGeenBrand() const;

    /**
     * @brief Notifies devices the client needs help, so that the devices can react correctly.
    */
    void meldPatientHulp() const;

    /**
     * @brief Creates and stores a new bed for this apartment.
     * @param fd The connection with the bed in the apartment.
    */
    void setBed(uint16_t fd);

    /**
     * @brief Creates and stores a new window for this apartment.
     * @param fd The connection with the window in this apartment.
    */
    void setVenster(uint16_t fd);
    
    /**
     * @brief Creates and stores a new chair for this apartment.
     * @param fd The connection with the chair in this apartment.
    */
    void setStoel(uint16_t fd);

    /**
     * @brief Creates and stores a new door for this apartment.
     * @param fd The connection with the door in this apartment.
    */
    void setDeur(uint16_t fd);

    /**
     * @brief Creates and stores a new column for this apartment.
     * @param fd The connection with the column in this apartment.
    */
    void setZuil(uint16_t fd);

    /**
     * @brief Creates and stores a new lamp for this apartment.
     * @param fd The connection with the lamp in this apartment.
    */
    void setSchemerlamp(uint16_t fd);
    
    /**
     * @brief Processes a request from the bed.
     * @param action The action code send by the bed.
    */
    void processBed(const std::string& action) const;

    /**
     * @brief Processes a request from the window.
     * @param action The action code send by the window.
    */
    void processVenster(const std::string&  action) const;

    /**
     * @brief Processes a request from the chair.
     * @param action The action code send by the chair.
    */
    void processStoel(const std::string&  action) const;

    /**
     * @brief Processes a request from the door.
     * @param action The action code send by the door.
    */
    void processDeur(const std::string&  action) const;

    /**
     * @brief Processes a request from the column.
     * @param action The action code send by the column.
    */
    void processZuil(const std::string&  action) const;

    /**
     * @brief Processes a request from the lamp.
     * @param action The action code send by the lamp.
    */
    void processSchemerlamp(const std::string& action) const;

    /**
     * @brief Creates a new connection with furniture or processes a request from furniture, depending on the action code and current state.
     * @param fd The connection that sent a request.
     * @param action The request/action code
    */
    void handleRequest(uint16_t fd, const std::string& action);
};
#endif // !ACTIONS_H
