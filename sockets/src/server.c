#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include "mutual.h"
#include "rdwrn.h"
#include "../../common/shared.h"
#include "../../common/error_handling.h"
#include "../../common/server.h"
#include "../../common/dbAccess.h"
#include "../../common/ipc.h"

// Reference http://man7.org/tlpi/code/online/book/sockets/is_seqnum_sv.c

#define BACKLOG 50
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
#include <netdb.h>

void onSigInt(int sig);
static int sfd = -1, cfd = -1;

int main() {
    Request req;
    Response resp;
    struct sockaddr_storage claddr;
    int lfd, cfd, optval;
    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        fatal("signal");

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

    if (getaddrinfo(NULL, "50000", &hints, &result) != 0)
        fatal("getaddrinfo");

    optval = 1;
    for(rp = result; rp != NULL; rp = rp->ai_next){
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); 
        if(lfd == -1)
            continue;
        if(setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))
                == -1)
            fatal("setsockopt");
        if(bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(lfd);
    }
    if(rp == NULL)
        fatal("Could not bind socket to any address");

    if(listen(lfd, BACKLOG) == -1)
        fatal("listen");

    freeaddrinfo(result);

    for(;;) {
        addrlen = sizeof(struct sockaddr_storage);
        cfd = accept(lfd, (struct sockaddr *) &claddr, &addrlen);
        if(cfd == -1){
            perror("accept");
            continue; 
        }

        if(getnameinfo((struct sockaddr *)&claddr, addrlen,
                    host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
            snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
        else
            snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
        if (readn(cfd, &req, sizeof(Request)) != sizeof(Request)) {
            close(cfd);
            continue;
        }
        resp = execute(req);
        if (writen(cfd, &resp, sizeof(Response)) != sizeof(Response))
            fprintf(stderr, "Error al escribir.");

        if(close(cfd) == -1)
            printf("Error al cerrar.\n");
    }
}

void onSigInt(int sig) {
    int exit_status = EXIT_SUCCESS;
    if(cfd != -1 && close(cfd) != 0) {
        exit_status = EXIT_FAILURE;
    }
    if(sfd != -1 && close(sfd) != 0) {
        exit_status = EXIT_FAILURE;
    }
    exit(exit_status);
}
