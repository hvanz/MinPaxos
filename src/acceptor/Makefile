#!/usr/bin/make -f
CC := c99
HDRS := $(wildcard *.h)
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

# ifeq ($(CC), ccomp)
# CFLAGS := -D_XOPEN_SOURCE=500 -D_GNU_SOURCE
# else ifeq ($(CC), c99)
# CFLAGS := -D_XOPEN_SOURCE=500 -D_GNU_SOURCE
# else
# CFLAGS := -Wall
# endif

#LFLAGS := -lpthread
TARGET := acceptor

all: $(OBJS) $(TARGET)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c $(SRCS)

acceptor: ../paxos_common/messages.o ../paxos_common/data.o \
	../paxos_common/comm.o ../paxos_common/space.o\
	acceptor.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -rf *.o $(TARGET)
