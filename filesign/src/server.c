#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "../include/files-signals.h"
#include "../../common/shared.h"
#include "../../common/server.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

static struct sigaction sig;
static Request req;
static Response resp;

int main() {
    long p = (long)getpid();
    FILE *file = fopen(SERVER_PID_FILE, "wb");
    
    if(file == NULL) {
        printf("Error mientras se crea el archivo server_pid\n");
        exit(EXIT_FAILURE);
    }
    if(fwrite(&p, sizeof(p), 1, file) != 1) {
        printf("Error al escribir el archivo server_pid\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    signal(SIGINT, onSigInt);

    //no signals masked
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    sig.sa_handler = sig_usr1_handler;
    sigaction(SIGUSR1, &sig, NULL);

    for(;;);
}

void onSigInt(int sig) {
    int exit_status = EXIT_SUCCESS;
    if(remove(SERVER_PID_FILE) == -1) exit_status = EXIT_FAILURE;    
    exit(exit_status);
}

void sig_usr1_handler(int s) {
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGUSR1);
    sigaddset(&signal_set, SIGUSR2);

    if(sigprocmask(SIG_BLOCK, &signal_set, NULL) == -1) {
        printf("Error bloqueando seniales.");
        exit(EXIT_FAILURE);
    }

    read_client_messages();
    
    if(sigprocmask(SIG_UNBLOCK, &signal_set, NULL) == -1) {
        printf("Error desbloqueando seniales.");
        exit(EXIT_FAILURE);
    }
}

void read_client_messages(void) {
    char clientFile[CLIENT_FILE_NAME_LEN];
    DIR *dirp;
    struct dirent *direntp;

    if((dirp = opendir(CTOS_PATH)) == NULL) {
        printf("Error abriendo el directorio ctos.");
        exit(EXIT_FAILURE);
    }
    while((direntp = readdir(dirp)) != NULL) {
        if(direntp->d_name[0] == '.'){
            continue;
        }
        strncpy(clientFile, CTOS_PATH, CLIENT_FILE_NAME_LEN);
        strcat(clientFile, "/");
        strcat(clientFile, direntp->d_name);

        FILE *file = fopen(clientFile,"rb");
        if(file == NULL){
            printf("Error while opening %s file\n",clientFile);
            exit(EXIT_FAILURE);
        }
        if(fread(&req,sizeof(Request),1,file) == -1) {
            printf("Error while reading from %s file\n",clientFile);
            exit(EXIT_FAILURE);
        }
        fclose(file);
        
        if(remove(clientFile) == -1){
            printf("Error while removing %s file\n",clientFile);
            exit(EXIT_FAILURE);
        }
        resp = execute(req);

        strncpy(clientFile, STOC_PATH, CLIENT_FILE_NAME_LEN);
        strcat(clientFile,"/");
        strcat(clientFile, direntp->d_name);
        printf("%s\n", clientFile);

        create_server_file(clientFile);
        
        if(server_communicate(req.pid) == -1) {
            if(remove(clientFile) ==-1){
                printf("error while removing %s file\n",clientFile);
                exit(EXIT_FAILURE);
            }
        }
    }

    closedir(dirp);
}

int server_communicate(long clientpid) {
    if(kill(clientpid, SIGUSR2) == -1) {
        if( errno == EPERM) {
            printf("El proceso cliente existe, pero no tenemos permisos.\n");
        } else if( errno == ESRCH) {
            printf("El proceso del cliente no existe.\n");
        }
		return -1;        
    }
    return 0;
}

void create_server_file(char * clientFile) {
    FILE *file = fopen(clientFile, "wb");
    if(file == NULL){
        printf("error while creating %s file\n",clientFile);
        exit(EXIT_FAILURE);
    }
    if(fwrite(&resp,sizeof(Response),1,file) == -1){
        printf("error while writing %s file\n",clientFile);
        exit(EXIT_FAILURE);
    }
    fclose(file);
}
