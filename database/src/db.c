#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../../common/db.h"
#include "../../common/dbAccess.h"
#include "../../common/shared.h"

void initializeFlightsList(char origins[][CITY_LENGTH], char destinations[][CITY_LENGTH], 
    char dates[][DATE_LENGTH], char times[][TIME_LENGTH], char flightNumbers[][FLIGHT_NUMBER_LENGTH]);
void initializeFlights(char origins[][CITY_LENGTH], char destinations[][CITY_LENGTH], 
    char dates[][DATE_LENGTH], char times[][TIME_LENGTH], char flightNumbers[][FLIGHT_NUMBER_LENGTH]);

int main(void) {
    char origins[FLIGHTS_QTY][CITY_LENGTH] = {
        "El_Calafate  (FTE)  ", "El_Calafate  (FTE)  ", "El_Calafate  (FTE)  ",
        "Bariloche    (BRC)  ", "Bariloche    (BRC)  ", "Bariloche    (BRC)  ", 
        "Salta        (SLA)  ", "Salta        (SLA)  ", "Salta        (SLA)  ",
        "Buenos_Aires (AEP)  ", "Buenos_Aires (AEP)  ", "Buenos_Aires (AEP)  "}; 

    char destinations[FLIGHTS_QTY][CITY_LENGTH] = {
        "Bariloche    (BRC)  ", "Buenos_Aires (AEP)  ", "Salta        (SLA)  ", 
        "Salta        (SLA)  ", "Buenos_Aires (AEP)  ", "El_Calafate  (FTE)  ", 
        "Buenos_Aires (AEP)  ", "Bariloche    (BRC)  ", "El_Calafate  (FTE)  ", 
        "El_Calafate  (FTE)  ", "Bariloche    (BRC)  ", "Salta        (SLA)  "};
    
    char dates[FLIGHTS_QTY][DATE_LENGTH] = {
        "06/01/2015", "06/01/2015", "06/01/2015", "06/01/2015", "06/01/2015",
        "06/01/2015", "06/01/2015", "06/01/2015", "06/01/2015", "06/01/2015",
        "06/01/2015", "06/01/2015"};

    char times[FLIGHTS_QTY][TIME_LENGTH] = {
        "03:09", "17:19", "17:38", "00:28", "15:53", "18:38", "18:03", "09:16", 
        "05:33", "20:08", "00:12", "06:33"};

    char flightNumbers[FLIGHTS_QTY][FLIGHT_NUMBER_LENGTH] = {
        "AA01357", "AA01363", "AA01364", "AA01365", "AA01367", "AA01330", 
        "AA01301", "AA00020", "AA00003", "AA00010", "AA01234", "AA01235"};

    initializeFlightsList(origins, destinations, dates, times, flightNumbers);
    initializeFlights(origins, destinations, dates, times, flightNumbers);
    return 0;
}

void initializeFlightsList(char origins[][CITY_LENGTH], char destinations[][CITY_LENGTH], 
    char dates[][DATE_LENGTH], char times[][TIME_LENGTH], char flightNumbers[][FLIGHT_NUMBER_LENGTH]) {
     int i;
     Matrix *mList = malloc(sizeof(Matrix));
     mList->responseCode = 0;
     FILE *file = fopen(FLIGHT_LIST_PATH, "wb");
     if (file == NULL) {
        printf("Error creando el archivo para el listado de vuelos.\n");
        exit(EXIT_FAILURE);
     }

     for(i = 0; i < FLIGHTS_QTY; i++) {
            strncpy(mList->values[i].origin, origins[i], CITY_LENGTH);
            strncpy(mList->values[i].destination, destinations[i], CITY_LENGTH);
            strncpy(mList->values[i].date, dates[i], DATE_LENGTH);
            strncpy(mList->values[i].hour, times[i], TIME_LENGTH);
            strncpy(mList->values[i].flightNumber, flightNumbers[i], FLIGHT_NUMBER_LENGTH);
     }        
     if(fwrite(mList, sizeof(Matrix), 1, file) != 1) {
        printf("Error creando el archivo para el listado de vuelos.\n");
        exit(EXIT_FAILURE);
     }
     fclose(file);
     free(mList);
}

void initializeFlights(char origins[][CITY_LENGTH], char destinations[][CITY_LENGTH], 
    char dates[][DATE_LENGTH], char times[][TIME_LENGTH], char flightNumbers[][FLIGHT_NUMBER_LENGTH]) {
    int i, j;
    char flightName[20];
    for(i = 0; i < FLIGHTS_QTY; i++) {
        Flight *flight = malloc(sizeof(Flight));
        strncpy(flight->origin, origins[i], CITY_LENGTH);
        strncpy(flight->destination, destinations[i], CITY_LENGTH);
        strncpy(flight->date, dates[i], DATE_LENGTH);
        strncpy(flight->hour, times[i], TIME_LENGTH);
        strncpy(flight->flightNumber, flightNumbers[i], FLIGHT_NUMBER_LENGTH);
        flight->seatsLeft = FLIGHTS_QTY;
        for(j = 0; j < STD_SEAT_QTY; j++){
            strcpy((flight->seats)[j], "\0");
        }
        sprintf(flightName, FLIGHT_PATH, flight->flightNumber);
        FILE *file = fopen(flightName, "wb");
        if (file == NULL) {
            printf("Error creando el archivo para el vuelo %s.\n", flight->flightNumber);
            exit(EXIT_FAILURE);
        }
        if (fwrite(flight, sizeof(Flight), 1, file) != 1) {
            printf("Error creando el archivo para el vuelo %s.\n", flight->flightNumber);
            exit(EXIT_FAILURE);
        }
        fclose(file);
        free(flight);
    }
}
