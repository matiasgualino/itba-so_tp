#include <arpa/inet.h>
#include <errno.h>
#include "mutual.h"
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "rdwrn.h"
#include "../../common/clientback.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"
#include "../../common/shared.h"

#define HARDCODED_SV_IP "127.0.0.1"

void communicate(void);
void onSigInt(int sig);

static int cfd = -1;
static Request req;
static Response resp;
static char server_ip[14];

void initClient() {
    printf("IP donde queres conectar el socket: ");
    fflush(stdout);
    scanf("%s", server_ip);
    signal(SIGINT, onSigInt);
}

void closeClient() {
    exit(EXIT_SUCCESS);
}

void onSigInt(int sig){
    closeClient();
}

void communicate(){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    inet_pton(AF_INET, server_ip, &(addr.sin_addr));
    if( (cfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return;
    }
    if(connect(cfd, (struct sockaddr *) &addr, 
                sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        return;
    }
    if(writen(cfd, &req, sizeof(Request)) != sizeof(Request)) {
        perror("No pudimos escribir en el servidor.");
        return;
    }
    if(readn(cfd, &resp, sizeof(Response)) != sizeof(Response)) {
        perror("No pudimos leer la respuesta del servidor.");
        return;
    }
    if(close(cfd) == -1) {
        printf("Error al cerrar la comunicacion con el socket.\n");
        exit(EXIT_FAILURE);
    }
}

Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]){
    req.comm = GET_FLIGHT;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    communicate();
    return resp.flight;
}

Matrix get_flights_list(void){
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