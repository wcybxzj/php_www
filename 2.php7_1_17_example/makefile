$(CC)=gcc
ARG+=-lpthread -march=nocona -mtune=generic

PROGS = 1.atmoic_cmp_set 2.use_signal_cancel_accept 3.epoll_et_lt

all:$(PROGS)

1.atmoic_cmp_set: 1.atmoic_cmp_set.o
	$(CC) -o 1.atmoic_cmp_set 1.atmoic_cmp_set.c $(ARG)

2.use_signal_cancel_accept: 2.use_signal_cancel_accept.o
	$(CC) -o 2.use_signal_cancel_accept  2.use_signal_cancel_accept.c

3.epoll_et_lt: 3.epoll_et_lt.o
	$(CC) -o 3.epoll_et_lt 3.epoll_et_lt.c

%.o: %.c
	$(CC) -c $<

clean:
	rm -rf *.o $(PROGS)

