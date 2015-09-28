#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "dbAccess.h"
#include "shared.h"
#include "ipc.h"

Response execute(Request r) {
    Response response;
    switch(r.comm){
        case RESERVE_SEAT:
            response.responseCode = reserve_seat(r.client, r.flightNumber, r.seat);
            break;
        case CANCEL_SEAT:
            response.responseCode = cancel_seat(r.client, r.flightNumber, r.seat);
            break;
        case GET_FLIGHT:
            response.flight = get_flight(r.flightNumber);
            break;
        case FLIGHT_LIST:
            response.matrix = get_flights_list();
            break;
        default:
            printf("El requerimiento no es valido.\n");
            memset(&response, 0, sizeof(Response));
            response.responseCode = -1;
    }
    return response;
}
