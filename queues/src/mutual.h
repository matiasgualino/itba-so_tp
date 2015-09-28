#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "../../common/shared.h"
#include "../../common/ipc.h"

#define QUEUE_SERVER "server_queue"
#define QUEUE_CLIENT "client_queue"

#define SERVER_KEY ftok(QUEUE_SERVER, 2)
#define CLIENTS_KEY ftok(QUEUE_CLIENT, 1)

void onSigInt(int sig);

typedef struct {
    long mtype;
    Request req;
} ReqMsg;

typedef struct {
    long mtype;
    Response resp;
} RespMsg;

#endif
