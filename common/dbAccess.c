#include <fcntl.h>
#include <stdio.h>
#include "dbAccess.h"

static struct flock fl = {.l_start = 0, .l_whence = SEEK_SET, .l_len = 0};
int unlockFile(int fd);
int wrlockFile(int fd);
int rdlockFile(int fd);

Flight get_flight(char flightNumber[FLIGHT_NUMBER_LENGTH]){
    Flight *f = malloc(sizeof(Flight));
    char flightName[MAX_NAME_LENGTH];

    sprintf(flightName, FLIGHT_PATH_IPC, flightNumber);

    FILE *file = fopen(flightName, "rb+");
    if (file == NULL) {
        printf("Numero de vuelo invalido.\n");
        return *f;
    }

    int fd = fileno(file);
    if (wrlockFile(fd) == -1) {
        printf("Error haciendo lock del archivo del vuelo\n");
        return *f;
    }

    if (fread(f, sizeof(Flight), 1, file) != 1) {
        printf("Error leyendo del archivo\n"); 
        return *f;
    }
   
    unlockFile(fd);
    fclose(file);
    return *f;
}

int reserve_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    int fd;
    Flight f;
    char flightName[MAX_NAME_LENGTH];
     
    if (seat < 1 || seat > STD_SEAT_QTY) {
        return INVALID_SEAT;
    }

    sprintf(flightName, FLIGHT_PATH_IPC, flightNumber); 
    FILE *file = fopen(flightName, "rb+");
    if (file == NULL) {
        printf("Error abriendo el archivo para el vuelo %s.\n", flightNumber);
        return INVALID_FLIGHT_NUMBER;
    }
    
    fd = fileno(file);
    if (wrlockFile(fd) == -1) {
        printf("Error protegiendo el archivo.\n");
        fclose(file);
        return -3;
    }
    
    if (fread(&f, sizeof(Flight), 1, file) != 1) {
        printf("Error leyendo el archivo.\n"); 
        unlockFile(fd);
        fclose(file);
        return -4;
    }

    if (strcmp(f.seats[seat-1], "\0") != 0) {
        unlockFile(fd);
        fclose(file);
        return SEAT_TAKEN;
    }
    
    f.seatsLeft--;
    strncpy(f.seats[seat-1], c.username, MAX_LENGTH);
 
    fclose(file);
    file = fopen(flightName, "wb");
    if (file == NULL) {
        printf("Error abriendo el archivo para el vuelo %s.\n", flightNumber);
        return INVALID_FLIGHT_NUMBER;
    }

    if (fwrite(&f, sizeof(Flight), 1, file) != 1) {
        printf("Error escribiendo el archivo para el vuelo %s.\n", flightNumber);
        unlockFile(fd);
        fclose(file);
        exit(1);
    }
    unlockFile(fd);
    fclose(file);
    return 0;
}

Matrix get_flights_list() {
    printf("Estoy en get_flights_list\n");
    Matrix *matrix = malloc(sizeof(Matrix));
    matrix->responseCode = 0;
    printf("hago fopen\n");
    FILE *file = fopen(FLIGHT_LIST_PATH_IPC, "rb+");
    if(file == NULL) {
        printf("file null\n");
       matrix->responseCode = -1; 
    } else if(fread(matrix, sizeof(Matrix), 1, file) == -1) {
        printf("error en read\n");
        matrix->responseCode = -2;
    }
    printf("te devuelvo la matriz\n");
    return *matrix;
}

int cancel_seat(Client c, char flightNumber[FLIGHT_NUMBER_LENGTH], int seat){
    int fd;
    char flightName[MAX_NAME_LENGTH];
    Flight f;
    f = get_flight(flightNumber);
    sprintf(flightName, FLIGHT_PATH_IPC, flightNumber); 

    if (strncmp(f.seats[seat-1], c.username, strlen(c.username)) != 0) {
        return 1;
    }

    strncpy(f.seats[seat-1], "\0", MAX_LENGTH);
    f.seatsLeft++;

    FILE *file = fopen(flightName, "wb");
    if (file == NULL) {
        printf("El numero de vuelo es incorrecto.\n");
        exit(1);
    }

    fd = fileno(file); 
    if( wrlockFile(fd) == -1 ){
        printf("Error protegiendo el archivo.\n");
        return INVALID_FLIGHT_NUMBER; 
    }

    if( fwrite(&f, sizeof(Flight), 1, file) != 1 ){
        printf("Error escribiendo el archivo para el vuelo %s.\n", flightNumber);
        exit(1);
    }
    unlockFile(fd);
    fclose(file);
    return 0;
}

int rdlockFile(int fd){
    fl.l_type = F_RDLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

int wrlockFile(int fd){
    fl.l_type = F_WRLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

int unlockFile(int fd){
    fl.l_type = F_UNLCK;
    return fcntl(fd, F_SETLKW, &fl);
}

bool noSeatsLeft(Flight f){
    return f.seatsLeft == 0;
}
