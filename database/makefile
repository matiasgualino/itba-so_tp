CC = gcc
CFLAGS = -Wall
TARGET = client
SRC = ./src
BIN = ./bin

default: db
	
db: db.o
	@mkdir -p $(BIN)
	@mkdir -p ./db
	$(CC) $(CFLAGS) db.o -o $(BIN)/db

db.o: $(SRC)/db.c ../common/shared.h ../common/dbAccess.h ../common/db.h
	$(CC) $(CFLAGS) -c $(SRC)/db.c 

clean:
	$(RM) db *.o *~
