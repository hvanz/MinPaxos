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

# LFLAGS := -lpthread
TARGET := client

all: $(OBJS) $(TARGET)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c $(SRCS)

client: ../paxos_common/messages.o ../paxos_common/comm.o ../paxos_common/data.o ../paxos_common/space.o \
	../paxos_lib/wor_quorums.o ../paxos_lib/paxos_view.o \
	../paxos_lib/paxos_comm.o ../paxos_lib/wor_handler.o \
	../paxos_lib/wor_steps.o ../paxos_lib/wor_phases.o ../paxos_lib/wor_selector.o \
	../paxos_lib/wor_helper.o ../paxos_lib/wor.o \
	../paxos_lib/sharedlog_indices.o \
	../paxos_lib/wos_init.o ../paxos_lib/wos_state.o ../paxos_lib/wos.o ../paxos_lib/worm_sharedlog.o \
	repcounter.o client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	rm -rf *.o $(TARGET)
