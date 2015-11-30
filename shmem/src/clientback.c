#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "mutual.h"
#include "../../common/clientback.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"
 
static Request *shm_req;
static Response *shm_resp;

void initClient() {
    shm_req = (Request *)getmem();
    shm_resp = malloc(sizeof(Response));
    initmutex();
    signal(SIGINT, onSigInt);
    return;
}

void onSigInt(int sig) {
    closeClient();
}

Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]){
    enter(1);
    shm_req->comm = GET_FLIGHT;
    strncpy(shm_req->flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    leave(2);
    enter(3);
    memcpy(shm_resp, shm_req, sizeof(Response));
    leave(1);
    return shm_resp->flight;
}

Matrix get_flights_list() {
    enter(1);
    shm_req->comm = FLIGHT_LIST;
    leave(2);
    enter(3);
    memcpy(shm_resp, shm_req, sizeof(Response));
    leave(1);
    return shm_resp->matrix;
}

int cancel_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    enter(1);
    shm_req->comm = CANCEL_SEAT;
    strncpy(shm_req->flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    shm_req->seat = seat;
    shm_req->client = c;
    leave(2);
    enter(3);
    memcpy(shm_resp, shm_req, sizeof(Response));
    leave(1);
    return shm_resp->responseCode;
}

int reserve_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    enter(1);
    shm_req->comm = RESERVE_SEAT;
    shm_req->client = c;
    strncpy(shm_req->flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    shm_req->seat = seat;
    leave(2);
    enter(3);
    memcpy(shm_resp, shm_req, sizeof(Response));
    leave(1);
    return shm_resp->responseCode;
}