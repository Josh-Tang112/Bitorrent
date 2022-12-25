CC=gcc
CFLAG_INCLUDE=-I. -Iinclude
CFLAGS=-Wall $(CFLAG_INCLUDE) -Wextra -std=gnu99 -ggdb -lm
LDLIBS=-lcrypto
SRC=src
VPATH= $(SRC) include

all: bittorrent 

bittorrent: bencoder.o hash.o arg_parser.o helpers.o data.o talking_to_tracker.o talking_to_peer.o bittorrent.c

clean:
	rm -rf *~ bittorrent *.o

.PHONY : clean all
