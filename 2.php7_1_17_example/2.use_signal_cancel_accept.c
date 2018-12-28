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
#include <sys/types.h>
#include <unistd.h>

#define IP_SIZE 16

#define SERVERPORT "1989"
#define FMT_STAMP "%lld"



//fpm运行在前台运行:
//父进程创建listenfd:
//代码fpm_init()-->fpm_sockets_init_main()
//
//worker子进程dup2(listenfd,STDIN_FILENO):
//代码fpm_run()-->fpm_children_create_initial()-->fpm_children_make()-->fpm_child_init()-->fpm_stdio_init_child()
//
//worker子进程正在accept(STDIN_FILENO):
//
//worker子进程通过信号SIGQUIT来 出发信号处理方法来close(STDIN_FILENO),目的是让accept()返回-1来结束
//代码fpm_run()-->fpm_children_create_initial()-->fpm_children_make()-->fpm_child_init()--> fpm_signals_init_child()-->sig_soft_quit()-->close(0)

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
说明:SININT close(0)后accept直接就挂了后边代码都不执行,返回码是130
#define EOWNERDEAD  130 // Owner died

终端1:
./2.use_signal_cancel_accept 
client usage: nc 127.0.0.1 1989 
pid:98027

accept() err!: Bad file descriptor

终端2:
kill -QUIT 98027
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

static void sig_soft_quit(int signo) /* {{{ */
{
	close(0);
}

int main(){
	printf("client usage: nc 127.0.0.1 1989 \n");
	printf("pid:%d\n",getpid());
	int sd, newsd;
	struct sockaddr_in laddr, raddr;
	socklen_t rlen;
	char ip[IP_SIZE];

	//信号
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = &sig_soft_quit;
	sigaction(SIGQUIT, &act, 0);

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
		newsd = accept(STDIN_FILENO, (void *)&raddr, &rlen);
		if(newsd<0){
			if(errno == EAGAIN ||errno == EINTR){
				continue;
			}
			perror("accept() err!");
			exit(-2);
		}else{
			printf("accept fine!\n");
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
