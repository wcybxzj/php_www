#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
 
void signal_handler(int signo);
 
int main(void){
 
    //设置信号掩码，屏蔽信号:SIGINT（2 非可靠信号 ctrl+c )、SIGRTMIN(34 可靠信号）
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGRTMIN);
    sigprocmask(SIG_BLOCK, &set, NULL);
 
    //为以下信号安装信号处理器:SIGINT（2 非可靠信号 ctrl+c )、SIGRTMIN(34 可靠信号）、SIGQUIT（3 非可靠信号 ctrl+\ ）
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGRTMIN,&sa,NULL);
    sigaction(SIGQUIT,&sa,NULL);
 
    int count = 0;
    while(1){
        if(count >= 100){ //sleep 100s后，退出
            break;
        }
        printf("sleep ..\n");
        sleep(1);
        if(count > 0 && count%10 == 0){ //每10s，接收一次信号，接收之后继续屏蔽信号SIGINT、SIGRTMIN
            printf("挂起等待信号..\n");
            sigemptyset(&set);
            sigsuspend(&set);
        }
        count++;
    }
}
 
void signal_handler(int signo){ //由于信号掩码的设置，该信号处理器被调用的时候，不会被SIGINT、SIGRTMIN打断、干扰
    if(signo == SIGINT){
        printf("catch signal SIGINT:%d\n",signo);
    }else if(signo == SIGRTMIN){
        printf("catch signal SIGRTMIN:%d\n",signo);
    }else if(signo == SIGQUIT){
        printf("catch signal SIGQUIT:%d,exit..\n",signo);
        exit(0);
    }else{
        printf("catch signal :%d\n",signo);
    }
}
