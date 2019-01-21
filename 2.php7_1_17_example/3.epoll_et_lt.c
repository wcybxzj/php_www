#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <errno.h>
//同时最多服务5个客户进行聊天
#define USER_LIMIT 3
#define BUFFER_SIZE 128
#define FD_LIMITS 65535

struct client_data{
	int fd;
	struct sockaddr_in address;
	char *write_buf;
	char buf[BUFFER_SIZE];
};

int setnonblocking(int fd)
{
	int old = fcntl(fd, F_GETFL);
	int new = old | O_NONBLOCK;
	fcntl(fd, F_SETFL, new);
	return old;
}

int get_fds_index(struct client_data *fds, int connfd, int user_counter){
	int i;
	for (i = 1; i <= user_counter; i++) {
		if (connfd == fds[i].fd) {
			return i;
		}
	}
	return -1;
}


void func1_et(int argc, const char *argv[])
{
	if (argc !=3) {
		printf("./a.out ip port\n");
		exit(1);
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);
	
	int i, j, k, err, ret, epoll_ret, fd, listenfd;
	socklen_t err_len;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("socket");
		exit(1);
	}

	int val=1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,\
				&val, sizeof(val)) < 0){ 
		perror("setsockopt()");
		exit(0);
	}

	ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
	if (ret == -1) {
		perror("bind");
		exit(1);
	}

	ret = listen(listenfd, 5);
	if (ret == -1) {
		perror("listen");
		exit(1);
	}

	char *info;
	int user_counter = 0;

	struct client_data  fds[USER_LIMIT+1];
	for (i = 0; i <= USER_LIMIT; i++) {
		fds[i].fd = -1;
	}
	fds[0].fd = listenfd;

	int epfd = epoll_create(1);
	if (epfd == -1) {
		perror("epoll_create");
		exit(1);
	}
	struct epoll_event ev, tmp_ev;
	struct epoll_event evlist[USER_LIMIT];
	ev.data.fd = listenfd;
	//使用ET
	ev.events = EPOLLIN|EPOLLERR|EPOLLRDHUP|EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	int connfd;
	struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof(client_address);

	while (1) {
		//printf("block\n");
		epoll_ret = epoll_wait(epfd, evlist, USER_LIMIT, -1);
		printf("unblock ret:%d\n", epoll_ret);
		if (epoll_ret == -1) {
			if (errno == EINTR) {
				printf("EINTR\n");
				continue;
			}else{
				perror("epoll_wait");
				exit(1);
			}
		}
	}

	free(fds);
}

void func2_lt(int argc, const char *argv[])
{
	if (argc !=3) {
		printf("./a.out ip port\n");
		exit(1);
	}

	const char *ip = argv[1];
	int port = atoi(argv[2]);
	
	int i, j, k, err, ret, epoll_ret, fd, listenfd;
	socklen_t err_len;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("socket");
		exit(1);
	}

	int val=1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,\
				&val, sizeof(val)) < 0){ 
		perror("setsockopt()");
		exit(0);
	}

	ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
	if (ret == -1) {
		perror("bind");
		exit(1);
	}

	ret = listen(listenfd, 5);
	if (ret == -1) {
		perror("listen");
		exit(1);
	}

	char *info;
	int user_counter = 0;

	struct client_data  fds[USER_LIMIT+1];
	for (i = 0; i <= USER_LIMIT; i++) {
		fds[i].fd = -1;
	}
	fds[0].fd = listenfd;

	int epfd = epoll_create(1);
	if (epfd == -1) {
		perror("epoll_create");
		exit(1);
	}
	struct epoll_event ev, tmp_ev;
	struct epoll_event evlist[USER_LIMIT];
	ev.data.fd = listenfd;
	//使用LT
	ev.events = EPOLLIN|EPOLLERR|EPOLLRDHUP;

	epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

	int connfd;
	struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof(client_address);

	while (1) {
		//printf("block\n");
		epoll_ret = epoll_wait(epfd, evlist, USER_LIMIT, -1);
		printf("unblock ret:%d\n", epoll_ret);
		if (epoll_ret == -1) {
			if (errno == EINTR) {
				printf("EINTR\n");
				continue;
			}else{
				perror("epoll_wait");
				exit(1);
			}
		}
	}


	free(fds);
}

//测试1-使用LT:
//终端1:
//./3.epoll_et_lt 127.0.0.1 1234
//unblock ret:1
//终端2:
//nc 127.0.0.1 1234

//测试2-使用ET:
//./3.epoll_et_lt 127.0.0.1 1234
//unblock ret:1
//unblock ret:1
//unblock ret:1
//unblock ret:1
//无数次unblock ret:1
//终端2:
//nc 127.0.0.1 1234

int main(int argc, const char *argv[])
{
	//func1_et(argc, argv);
	func2_lt(argc, argv);
}
