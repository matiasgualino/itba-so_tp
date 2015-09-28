CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin
COMMON = ../common
LFLAGS := -lpthread

OS := $(shell uname -s)
ifeq ($(OS),Linux)
LFLAGS += -lrt
endif

default: client server

client: clientfront.o clientback.o rdwrn.o error_handling.o
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/client clientfront.o clientback.o rdwrn.o error_handling.o $(LFLAGS)

clientfront.o: $(COMMON)/clientfront.c $(COMMON)/clientfront.h $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/clientfront.c

clientback.o: $(SRC)/clientback.c $(COMMON)/ipc.h $(COMMON)/shared.h $(COMMON)/dbAccess.h $(SRC)/mutual.h $(SRC)/rdwrn.c $(COMMON)/error_handling.c $(COMMON)/error_handling.h
	$(CC) $(CFLAGS) -c $(SRC)/clientback.c $(SRC)/rdwrn.c $(COMMON)/error_handling.c 

rdwrn.o: $(SRC)/rdwrn.c
	$(CC) $(CFLAGS) -c $(SRC)/rdwrn.c

server: server.o $(COMMON)/servercommon.o $(COMMON)/dbAccess.o error_handling.o 
	$(CC) $(CFLAGS) -o $(BIN)/server server.o rdwrn.o $(COMMON)/dbAccess.o $(COMMON)/servercommon.o error_handling.o $(LFLAGS)

error_handling.o: $(COMMON)/error_handling.c $(COMMON)/error_handling.h 
	$(CC) $(CFLAGS) -c $(COMMON)/error_handling.c

servercommon.o: $(COMMON)/servercommon.c $(COMMON)/server.h
	$(CC) $(CFLAGS) -c servercommon.c

server.o: $(SRC)/server.c $(COMMON)/dbAccess.c $(COMMON)/ipc.h $(COMMON)/shared.h $(SRC)/mutual.h
	$(CC) $(CFLAGS) -c $(SRC)/server.c $(COMMON)/dbAccess.c $(SRC)/rdwrn.c

dbAccess.o: $(COMMON)/dbAccess.c $(COMMON)/dbAccess.h $(COMMON)/shared.h 
	$(CC) $(CFLAGS) -c $(COMMON)/dbAccess.c

clean:
	$(RM) client *.o *~
