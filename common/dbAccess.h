#ifndef DBACCESS_H
#define DBACCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shared.h"

#define INVALID_SEAT -1
#define SEAT_TAKEN -2
#define NO_SEATS -1
#define INVALID_FLIGHT_NUMBER -3
#define FLIGHT_PATH "db/flight_%s" // flight_number
#define FLIGHT_LIST_PATH "db/flight_list"
#define FLIGHT_LIST_PATH_IPC "../database/db/flight_list"
#define FLIGHT_PATH_IPC "../database/db/flight_%s" // flight_number
#define FLIGHTS_QTY 12

bool noSeatsLeft(Flight m);
Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]);
Matrix get_flights_list();
int cancel_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat);
int reserve_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat);

#endif
