#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "../../common/dbAccess.h"
#include "../../common/server.h"
#include "../../common/error_handling.h"
#include "mutual.h"

static int msqin = -1, msqout = -1;

int main() {
    ReqMsg reqMsg;
    RespMsg respMsg;
    signal(SIGINT, onSigInt);

    if((msqin = msgget(CLIENTS_KEY, IPC_CREAT | 0666)) == -1) {
        perror("Fallo msgget de CLIENTS_KEY en main");
        return -1;
    }
    if ((msqout = msgget(SERVER_KEY, IPC_CREAT | 0666)) == -1) {
        perror("Fallo msgget de SERVER_KEY en main");
        return -1;
    }

    for(;;){
        if(msgrcv(msqin, &reqMsg, sizeof(reqMsg), 0, 0) == -1) {
            if(errno == EINTR) {
                continue;
            }
            perror("Fallo msgrcv de request en main");
            break;
        }
        respMsg.resp = execute(reqMsg.req);
        respMsg.mtype = reqMsg.mtype;
        if(msgsnd(msqout, &respMsg, sizeof(respMsg), 0) == -1) {
            perror("Fallo msgsnd de response en main server");
            break;
        }
    }
    closeServer();
    return 0;
}

void closeServer() {
    int exit_status = EXIT_SUCCESS;
    if(msqin != -1 && msgctl(msqin, IPC_RMID, NULL) == -1) {
        exit_status = EXIT_FAILURE;
    }
    if(msqout != -1 && msgctl(msqout, IPC_RMID, NULL) == -1) {
        exit_status = EXIT_FAILURE;
    }
    if(remove(QUEUE_CLIENT) == -1) {
        exit_status = EXIT_FAILURE;
    }
    if(remove(QUEUE_SERVER) == -1) {
        exit_status = EXIT_FAILURE;
    }
    exit(exit_status);
}

void onSigInt(int sig) {
    closeServer();
}
