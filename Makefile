CC = gcc
CFLAGS = -Wall
LDFLAGS =

SRCS = scheduler.c initialize.c write.c scenarios.c main.c
OBJS = $(SRCS:.c=.o)
EXEC = main

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
