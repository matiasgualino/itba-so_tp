#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <signal.h>
#include "../../common/dbAccess.h"
#include "../../common/shared.h"
#include "../include/files-signals.h"
#include "../../common/clientback.h"
#include "../../common/ipc.h"

static Request req;
static Response resp;
static char clientFile[CLIENT_FILE_NAME_LEN];

struct sigaction sig;

void initClient() {
    req.pid = (long)getpid();
	snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
                (long) getpid());
	signal(SIGINT, onSigInt);
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = sig_usr2_handler;
}

void sig_usr2_handler(int s) {
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set,SIGUSR1);
    sigaddset(&signal_set,SIGUSR2);

	if(sigprocmask(SIG_BLOCK,&signal_set,NULL) == -1) {
		perror("Error bloqueando seniales");
        return;
	}

    snprintf(clientFile, SERVER_FILE_NAME_LEN, SERVER_FILE_TEMPLATE,
            (long) getpid());

    FILE * file = fopen(clientFile, "rb+");
    if(file == NULL){
        printf("Error abriendo el archivo cliente %s \n", clientFile);				
        exit(EXIT_FAILURE);
    }
    if(fread(&resp, sizeof(Response), 1, file) == -1){
        printf("Error leyendo el archivo cliente %s \n", clientFile);
        exit(EXIT_FAILURE);
    }

    fclose(file);

    if(remove(clientFile) == -1){
        printf("Error eliminado el archivo del cliente %s \n", clientFile);
        exit(EXIT_FAILURE);
    }

    if( sigprocmask(SIG_UNBLOCK, &signal_set, NULL) == -1 ){
        printf("Error desbloqueando seniales.");
        exit(EXIT_FAILURE);
    }
}

void closeClient() {
    int exit_status = EXIT_SUCCESS;
    snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
            (long) getpid());
    FILE *file = fopen(clientFile,"rb");
    if(file != NULL) {
        if(remove(clientFile) == -1) {
            exit_status = EXIT_FAILURE;
        }
    }
    exit(exit_status);
}

void onSigInt(int sig){
    closeClient();
}

Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]){
    req.comm = GET_FLIGHT;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    communicate_with_server();
    return resp.flight;
}

Matrix get_flights_list(void){
    req.comm = FLIGHT_LIST;
    communicate_with_server();
    return resp.matrix;
}

int cancel_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    req.comm = CANCEL_SEAT;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    req.seat = seat;
    communicate_with_server();
    return resp.responseCode;
}

int reserve_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    req.comm = RESERVE_SEAT;
    req.client = c;
    strncpy(req.flightNumber, flightNumber, FLIGHT_NUMBER_LENGTH);
    req.seat = seat;
    communicate_with_server();
    return resp.responseCode;
}

void create_client_file() {
    snprintf(clientFile, CLIENT_FILE_NAME_LEN, CLIENT_FILE_TEMPLATE,
            (long) getpid());
    FILE *file = fopen(clientFile, "wb");
    if(file == NULL){
        printf("error while creating %s file\n",clientFile);
        exit(EXIT_FAILURE);
    }
    if( fwrite(&req,sizeof(Request),1,file) == -1 ){
        printf("error while writing %s file\n", clientFile);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void communicate() {
    long serverpid;

    FILE *file = fopen(SERVER_PID_FILE, "rb");
    if ( file == NULL ){
        printf("Error abriendo el archivo de server_pid.\n");
        exit(EXIT_FAILURE);
    }
    if(fread(&serverpid, sizeof(serverpid), 1, file) == -1) {
        perror("Erorr en fread");
        return;
    }
    fclose(file);

    if(kill(serverpid, SIGUSR1) == -1 ){
        if(errno == EPERM) {
            printf("Existe el proceso. No tenemos permisos para matarlo.\n");            
        } else if(errno == ESRCH) {
            printf("El proceso no existe.\n");
        }
        closeClient();
    }
}

void  communicate_with_server() {
    sigset_t mask,oldmask;
    create_client_file();
    communicate();
    sigaction(SIGUSR2,&sig,NULL);

    if(sigprocmask(SIG_BLOCK, &mask, &oldmask) == -1) {
        perror("Fallo oldmask en communicate_with_server.");
        return;
    }
    sigemptyset(&mask);
    if(sigsuspend(&mask) == -1 && errno != EINTR) {
        perror("Fallo sigsuspend en communicate_with_server.");
        return;
    }
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) == -1) {
        perror("Fallo sigprocmask en communicate_with_server.");
        return;
    }
}
