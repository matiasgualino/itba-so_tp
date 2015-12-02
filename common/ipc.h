#ifndef IPC_H
#define IPC_H
#include <sys/types.h> 
#include "shared.h"

typedef enum{RESERVE_SEAT, CANCEL_SEAT, GET_FLIGHT, FLIGHT_LIST} Command;

typedef struct  {
    pid_t pid;
    Command comm;
    Client client;
    char flightNumber[FLIGHT_NUMBER_LENGTH];
    int seat;
} Request;

typedef struct  {
    int responseCode;
    Flight flight;
    Matrix matrix;
} Response;



#endif
