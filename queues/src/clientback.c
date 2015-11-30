#include <fcntl.h>
#include "mutual.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../../common/error_handling.h"
#include "../../common/clientback.h"
#include "../../common/shared.h"
#include "../../common/ipc.h"

static ReqMsg reqMsg;
static RespMsg respMsg;
static int msqin = -1, msqout = -1;
void communicate();

void initClient() {
    signal(SIGINT, onSigInt);
    reqMsg.mtype = (long)getpid();

	msqin = msgget(SERVER_KEY, IPC_CREAT | 0666); 
	if(msqin == -1) {
        perror("Fallo msgget de SERVER_KEY en initClient");
        return;
    }

	msqout = msgget(CLIENTS_KEY, IPC_CREAT | 0666); 
	if(msqout == -1) {
        perror("Fallo msgget de CLIENTS_KEY en initClient");
        return;
    }
}

void onSigInt(int sig){
    closeClient();
}
 
void closeClient(){
    exit(EXIT_SUCCESS);
}

Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]){
    reqMsg.req.comm = GET_FLIGHT;
    strncpy(reqMsg.req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    communicate();
    return respMsg.resp.flight;
}

Matrix get_flights_list() {
    reqMsg.req.comm = FLIGHT_LIST;
    communicate();
    return respMsg.resp.matrix;
}

int cancel_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    reqMsg.req.comm = CANCEL_SEAT;
    strncpy(reqMsg.req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    reqMsg.req.seat = seat;
    communicate();
    return respMsg.resp.responseCode;
}

int reserve_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    reqMsg.req.comm = RESERVE_SEAT;
    reqMsg.req.client = c;
    strncpy(reqMsg.req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    reqMsg.req.seat = seat;
    communicate();
    return respMsg.resp.responseCode;
}

void communicate() {
    if(msgsnd(msqout, &reqMsg, sizeof(Request), 0) == -1) {
        perror("Fallo msgsnd de request en communicate");
        return;
    }
    if((msgrcv(msqin, &respMsg, sizeof(Response), getpid(), 0)) == -1) {
        perror("Fallo msgrcv de response en communicate");
        return;
    }
}
