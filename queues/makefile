CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin
COMMON = ../common

default: client server

client: clientfront.o clientback.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o clientback.o

clientfront.o: $(COMMON)/clientfront.c $(COMMON)/clientfront.h $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/clientfront.c

clientback.o: $(SRC)/clientback.c $(COMMON)/ipc.h $(COMMON)/shared.h $(COMMON)/dbAccess.h $(SRC)/mutual.h 
	$(CC) $(CFLAGS) -c $(SRC)/clientback.c

server: server.o $(COMMON)/dbAccess.o $(COMMON)/servercommon.o 
	$(CC) $(CFLAGS) -o $(BIN)/server server.o $(COMMON)/dbAccess.o $(COMMON)/servercommon.o

server.o: $(SRC)/server.c $(COMMON)/dbAccess.c $(COMMON)/ipc.h $(COMMON)/shared.h $(SRC)/mutual.h
	$(CC) $(CFLAGS) -c $(SRC)/server.c $(COMMON)/dbAccess.c

servercommon.o: $(COMMON)/servercommon.c $(COMMON)/server.h 
	$(CC) $(CFLAGS) -c $(COMMON)/servercommon.c

dbAccess.o: $(COMMON)/dbAccess.c $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/dbAccess.c

clean:
	$(RM) client *.o *~
