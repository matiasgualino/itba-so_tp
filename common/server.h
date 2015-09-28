#ifndef SERVER_H
#define SERVER_H

#include "ipc.h"

void closeServer();
Response execute(Request request);
void onSigInt(int sig);

#endif
