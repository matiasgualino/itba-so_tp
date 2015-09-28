#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../include/fifo.h"
#include "../../common/shared.h"
#include "../../common/server.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

static int serverFd = -1, dummyFd = -1, clientFd = -1;

int main(int argc, char *argv[]) {
    Request req;
    Response resp; 
    char clientFifo[CLIENT_FIFO_NAME_LEN];

    if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP | S_IFIFO | S_IRGRP | S_IROTH | 0777) == -1) {
        printf("Error creando Fifo (servidor).");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, onSigInt);

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if(serverFd == -1) {
        printf("Error abriendo Fifo (servidor).\n");
        exit(EXIT_FAILURE);
    }

    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if(dummyFd == -1){
        printf("Error abriendo Fifo (servidor).\n");
        exit(EXIT_FAILURE);
    }

    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        exit(EXIT_FAILURE);
    }

    for(;;){
        if(read(serverFd, &req, sizeof(Request)) != sizeof(Request)){
            fprintf(stderr, "Error al leer el requerimiento.\n");    
            continue;
        } 

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO, (long) req.pid);
        clientFd = open(clientFifo, O_WRONLY);
        if(clientFd == -1) {
            printf("No se pudo abrir %s", clientFifo);
            continue;
        }

        resp = execute(req);
        write(clientFd, &resp, sizeof(Response));
        /* VA ESTOif(write(clientFd, &resp, sizeof(Response)) != sizeof(Response)) {
            printf("Error escribiendo en el FIFO %s", clientFifo);
        }*/
        if(close(clientFd) == -1) {
            printf("Error al cerrar el FIFO %s\n", clientFifo); 
        }
    }
}

void onSigInt(int sig) {
    closeServer();
}

void closeServer() {
    int exit_status = EXIT_SUCCESS;
    if (clientFd != -1 && close(clientFd)) {
        exit_status = EXIT_FAILURE;
    } else if (dummyFd != -1 && close(dummyFd)) {
        exit_status = EXIT_FAILURE;
    } else if (serverFd != -1 && close(serverFd)) {
        exit_status = EXIT_FAILURE;
    } else if (unlink(SERVER_FIFO)) {
        exit_status = EXIT_FAILURE;
    }
    exit(exit_status);
}
