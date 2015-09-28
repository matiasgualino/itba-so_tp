CC = gcc
CFLAGS = -Wall
TARGET = client
VPATH = src:include

default:
	cd database; make
	cd database/bin; ./db
	cd filelocks; make
	cd fifos; make
	cd shm-sem-POSIX; make
	cd shm-sem-SV; make
	cd mqueues-POSIX; make
	cd mqueues-SV; make
	cd files-signals; make
	cd sockets; make
