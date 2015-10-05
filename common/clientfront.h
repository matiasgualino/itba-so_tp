#ifndef CLIENTFRONT_H
#define CLIENTFRONT_H

#include "dbAccess.h"

// ANSI colors definitions
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

Client login();
void execute_command(int command, Client currentUser);
void list_flights();
void buy_flight(Client c);
void cancel_order(Client c);
void print_flights(Flight flights[], int count);
void print_seats(char seats[STD_SEAT_QTY][MAX_NAME_LENGTH]);

#endif
