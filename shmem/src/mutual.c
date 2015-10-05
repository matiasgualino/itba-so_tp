#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include "mutual.h"
#include "../../common/error_handling.h"

static Request *mem;
static int memid;
static int semid;

void *getmem() {
	if((memid= shmget(SHARED_KEY, SIZE, IPC_CREAT | 0666)) == -1) {
		perror("Error en shmget (getmem).");
		return mem;
	} 
	if(!(mem = shmat(memid, NULL, 0))) {
		perror("Error en shmat (getmem).");
		return mem;
	}
	return mem;
}

void initmutex() {
    if((semid = semget(SHARED_KEY, 3, IPC_CREAT | 0666)) == -1) {
    	perror("Error en semget (initmutex).");
		return;
    }
	semctl(semid, 0, SETVAL, 1);
	semctl(semid, 1, SETVAL, 0);
	semctl(semid, 2, SETVAL, 0);
}

void closeServer() {
    int exit_status = EXIT_SUCCESS;
    if(shmdt(mem) != 0) {
    	exit_status = EXIT_FAILURE;
    } 
    if(shmctl(memid, IPC_RMID, NULL) != 0) {
    	exit_status = EXIT_FAILURE;
    }
    if(semctl(semid, 0, IPC_RMID) != 0) {
    	exit_status = EXIT_FAILURE;
    } 
    if(remove("/tmp/shm") == -1) {
    	exit_status = EXIT_FAILURE;
    } 
    exit(exit_status);
}

void closeClient() {
    int exit_status = EXIT_SUCCESS;
	if(shmdt(mem) != 0) {
		exit_status = EXIT_FAILURE;
	}
    exit(exit_status);
}

void enter(int sem_num) {
    struct sembuf sb;
	sb.sem_num = sem_num - 1;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	if(semop(semid,&sb,1) == -1) {
		perror("Error en semop (enter).");
		return;
	}
}
 
void leave(int sem_num) {
	struct sembuf sb;
	sb.sem_num = sem_num - 1;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	if(semop(semid,&sb,1) == -1) {
		perror("Error en semop (leave).");
		return;
	}
}
