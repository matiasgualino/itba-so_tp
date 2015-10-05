#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../../common/ipc.h"
#include "../../common/shared.h"
#include "../../common/dbAccess.h"
#include "../../common/server.h"
#include "mutual.h"


int main() {
    Request *req;
    Response *resp;

    req = (Request *)getmem();
    memset(req, 0, SIZE);
    resp = (Response *)req; 

    initmutex();
    signal(SIGINT, onSigInt);

    for(;;){
        enter(2);
        *resp = execute(*req);
        leave(3);
    }
    return 0;
}

void onSigInt(int sig){
    closeServer();
}
