#ifndef SHARED_H
#define SHARED_H

#define MAX_LENGTH 40
#define MAX_NAME_LENGTH 30
#define STD_SEAT_QTY 40
#define FLIGHTS_QTY 30
#define CITY_LENGTH 20
#define DATE_LENGTH 10
#define TIME_LENGTH 5
#define FLIGHT_NUMBER_LENGTH 7

typedef struct {
    char origin[CITY_LENGTH];
    char destination[CITY_LENGTH];
    char date[DATE_LENGTH];
    char hour[TIME_LENGTH];
    char flightNumber[FLIGHT_NUMBER_LENGTH];
    int seatsLeft;
    char seats[STD_SEAT_QTY][MAX_NAME_LENGTH];
} Flight;

typedef struct {
    char origin[CITY_LENGTH];
    char destination[CITY_LENGTH];
    char date[DATE_LENGTH];
    char hour[TIME_LENGTH];
    char flightNumber[FLIGHT_NUMBER_LENGTH];
} FlightInfo;

typedef struct {
    int responseCode;
    FlightInfo values[FLIGHTS_QTY];
} Matrix;

typedef struct{
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
} Client;

#endif
