$(CC)=gcc
ARG+=-lpthread -march=nocona -mtune=generic

PROGS = 1.atmoic_cmp_set

all:$(PROGS)

1.atmoic_cmp_set: 1.atmoic_cmp_set.o
	$(CC) -o 1.atmoic_cmp_set 1.atmoic_cmp_set.c $(ARG)

%.o: %.c
	$(CC) -c $<

clean:
	rm -rf *.o $(PROGS)

