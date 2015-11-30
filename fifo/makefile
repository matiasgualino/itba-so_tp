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

clientback.o: $(SRC)/clientback.c ./include/fifo.h $(COMMON)/shared.h $(COMMON)/dbAccess.h
	$(CC) $(CFLAGS) -c $(SRC)/clientback.c

server: $(SRC)/server.o $(COMMON)/dbAccess.o $(COMMON)/servercommon.o
	$(CC) $(CFLAGS) -o $(BIN)/server $(SRC)/server.c $(COMMON)/dbAccess.c $(COMMON)/servercommon.c

servercommon.o: $(COMMON)/servercommon.c  $(COMMON)/server.h
	$(CC) $(CFLAGS) -c $(COMMON)/servercommon.c 

server.o: $(SRC)/server.c $(COMMON)/dbAccess.c ./include/fifo.h $(COMMON)/shared.h $(COMMON)/ipc.h
	$(CC) $(CFLAGS) -c $(SRC)/server.c $(COMMON)/dbAccess.c

dbAccess.o: $(COMMON)/dbAccess.c $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/dbAccess.c

clean:
	$(RM) client *.o *~
