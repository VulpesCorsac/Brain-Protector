CC=gcc
CFLAGS=-Wall -std=gnu99 -g -I.
LDFLAGS=-L. -lfann
STRIP=strip

default: all

all: bp bpconfig bptrain

bp: main.o config.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bp main.o config.o

bpconfig: configedit.o config.o configmenuitem.o configmenu.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bpconfig config.o configmenuitem.o configmenu.o configedit.o

bptrain: train.o config.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o bptrain config.o train.o
