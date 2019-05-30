$(CC)=gcc
ARG+=-lpthread -march=nocona -mtune=generic

PROGS = 3.11.small

all:$(PROGS)

3.11.small: 3.11.small.o
	$(CC) -o 3.11.small 3.11.small.c  $(ARG)


%.o: %.c
	$(CC) -c $<

clean:
	rm -rf *.o $(PROGS)

