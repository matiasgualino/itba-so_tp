#ifndef FIFO_H
#define FIFO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

#define SERVER_FIFO "fifo_server"
#define CLIENT_FIFO "fifo_client_%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO) + 20)

void communicate(void);
void onSigInt(int sig);

#endif
