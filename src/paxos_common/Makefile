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

all: $(OBJS)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c $(SRCS)

clean:
	rm -rf *.o $(TARGET)
