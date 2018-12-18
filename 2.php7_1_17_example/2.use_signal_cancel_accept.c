#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>

#define IP_SIZE 16

#define SERVERPORT "1989"
#define FMT_STAMP "%lld"

//起因:
//php-fpm中 fpm_child_init()-->fpm_signals_init_child()
//worker子进程正在accept(fd)
//worker子进程通过给信号SIGQUIT设置sig_soft_quit()-->close(fd) 来终止woker工作进程

/*
测试1:基本功能测试
终端1:
./2.use_signal_cancel_accept 
radd:127.0.0.1 rport:34136
worker() i:0
worker() i:1
worker() i:2
worker() i:3
worker() i:4
worker() i:5
worker() i:6
worker() i:7
worker() i:8
worker() i:9
=====len:10======

终端2:
nc 127.0.0.1 1989
*/

/*
测试2:

*/

/*
测试3:
*/
int worker(int newsd){
	char str[IP_SIZE]={'\0'};
	int len;
	int ret;

	for (int i = 0; i < 10; i++) {
		printf("worker() i:%d\n", i);
		sleep(1);
	}

	len = sprintf(str, FMT_STAMP, (long long)time(NULL));

	printf("=====len:%d======\n", len);
	printf("%s", str);

	ret = send(newsd, str, len, 0);

	if(ret < 0){
		perror("send()");
		exit(-3);
	}else{
		printf("send ok");
	}

	//办法2:
	close(newsd);
}


int main(){
	int accept_fd = 0;
	int sd, newsd;
	struct sockaddr_in laddr, raddr;
	socklen_t rlen;
	char ip[IP_SIZE];

	////信号
	//struct sigaction act;
	//memset(&act, 0, sizeof(act));
	//act.sa_handler = &sig_soft_quit;
	//sigaction(SIGQUIT, $act, 0);

	sd = socket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/);
	if(sd < 0){
		perror("socket");
		exit(0);
	}

	int val=1;
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,\
				&val, sizeof(val)) < 0){
		perror("setsockopt()");
		exit(0);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(SERVERPORT));
	if(inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr) != 1){
		perror("inet_pton()");
		exit(0);
	}

	if(bind(sd, (void *)&laddr, sizeof(laddr)) < 0){
		perror("bind()");
		exit(0);
	}

	listen(sd, 200);

	//关闭STDIO_FILENO,让sd使用STDIO_FILENO的文件描述符号
	if (0 > dup2(sd, STDIN_FILENO)) {
		printf("dup2(), error\n");
	}

	while (1) {
		newsd = accept(sd, (void *)&raddr, &rlen);
		if(newsd<0){
			if(errno == EAGAIN ||errno == EINTR){
				continue;
			}
			perror("accept()");
			exit(-2);
		}

		if(inet_ntop(AF_INET, (void *)&raddr.sin_addr, \
					ip, IP_SIZE) == NULL){
			perror("inet_ntop()");
			exit(-2);
		}

		printf("radd:%s rport:%d\n", \
				ip, htons(raddr.sin_port));

		worker(newsd);
	}

	exit(0);
}
