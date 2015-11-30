#ifndef MUTUAL_H
#define MUTUAL_H

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "../../common/ipc.h"
 
#define SIZE sizeof(Request)
#define SHARED_KEY ftok("shm", 1)

static int memid;
 
void fatal(char *s);
void *getmem();
void initmutex();
void onSigInt(int sig);
void closeClient();
void closeServer();
void enter(int sem_num);
void leave(int sem_num);

#endif
