$(CC)=gcc
ARG+=-lpthread -march=nocona -mtune=generic

PROGS = 1.atmoic_cmp_set 2.use_signal_cancel_accept

all:$(PROGS)

1.atmoic_cmp_set: 1.atmoic_cmp_set.o
	$(CC) -o 1.atmoic_cmp_set 1.atmoic_cmp_set.c $(ARG)

2.use_signal_cancel_accept: 2.use_signal_cancel_accept.o
	$(CC) -o 2.use_signal_cancel_accept  2.use_signal_cancel_accept.c

%.o: %.c
	$(CC) -c $<

clean:
	rm -rf *.o $(PROGS)

