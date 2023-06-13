#ifndef F_MANAGE_H
#define F_MANAGE_H

#include <iostream>

void insertFlight(const std::string& dbPath);
void searchFlightViaDst(const std::string& dbPath);
void searchFlightViaNumber(const std::string& dbPath);
void browseFlights24(const std::string& dbPath);
void browseFlights(const std::string& dbPath);
void searchUserFlights(const std::string& dbPath, const std::string& userID);
void bookFlight(const std::string& dbPath, const std::string& flightNumber, const std::string& userID);
void cancelBooking(const std::string& dbPath, const std::string& flightNumber, const std::string& userID);

#endif F_MANAGE_H
