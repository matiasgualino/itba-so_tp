#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../common/clientback.h"
#include "../../common/dbAccess.h"
#include "../../common/error_handling.h"
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../include/fifo.h"

static char clientFifo[CLIENT_FIFO_NAME_LEN];
static int serverFd = -1, clientFd = -1;
static void removeFifo(void);

static void removeFifo(void){
    unlink(clientFifo);
}

void initClient() {
    req.pid = (long)getpid();
    umask(0);
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO, (long) getpid());

    if(mknod(clientFifo, S_IFIFO | 0666, 0) == -1) {
        printf("Error creando el FIFO (cliente). %d\n", errno);
        exit(EXIT_FAILURE);
    }

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if(serverFd == -1) {
        printf("Error abriendo el FIFO (cliente).\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, onSigInt);
    
    if(atexit(removeFifo) != 0) {
        printf("atexit error\n");
        exit(EXIT_FAILURE);
    }
}

void closeClient(){
    int exit_status = EXIT_SUCCESS;
    if(close(clientFd)) {
        exit_status = EXIT_FAILURE;
    }
    if(close(serverFd)) {
        exit_status = EXIT_FAILURE;
    }
    if(unlink(clientFifo)) {
        exit_status = EXIT_FAILURE;
    }
    exit(exit_status);
}

void onSigInt(int sig){
    closeClient();
}

void communicate() {
    
    write(serverFd, &req, sizeof(req));

    if( (clientFd = open(clientFifo, O_RDONLY)) == -1){
        printf("Error al abrir FIFO (cliente). \n");
        exit(EXIT_FAILURE);
    }
    
    read(clientFd, &resp, sizeof(resp));
        
    if(close(clientFd) == -1) {
        printf("Error cerrando FIFO (cliente).\n");
        exit(EXIT_FAILURE);
    }
}

Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]){
    req.comm = GET_FLIGHT;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    communicate();
    return resp.flight;
}

Matrix get_flights_list(){
    req.comm = FLIGHT_LIST;
    communicate();
    return resp.matrix;
}

int cancel_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    req.comm = CANCEL_SEAT;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    req.seat = seat;
    communicate();
    return resp.responseCode;
}

int reserve_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    req.comm = RESERVE_SEAT;
    req.client = c;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    req.seat = seat;
    communicate();
    return resp.responseCode;
}
