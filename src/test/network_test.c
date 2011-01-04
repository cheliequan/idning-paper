#include<stdio.h>
#include <unistd.h>

#include "network.h"
#include "log.h"

int main(){
    pid_t pid;
    if((pid = fork())<0){
        logging(LOG_ERROR, "fork error");
    }
    else if (pid == 0){ //child
        usleep(1);
        client();
    } else{ //parent
        server();
        usleep(1);
    }
}

int server(){
    char buffer[1024];
    int ss = server_socket("127.0.0.1", "9991");
    if (ss < 0){
        logging(LOG_ERROR, "socket error!");
        return -1;
    }
    int ns = tcptoaccept(ss, 100000);
    if (ns < 0){
        logging(LOG_ERROR, "accept error!");
        return -1;
    }
    tcpnonblock(ns);
    tcpnodelay(ns);
    int n = tcptoread(ns, buffer, 10, 3000);
    printf("read ok %d !!\n", n);
    printf("read data: \n%s\n", buffer);
}


int client(){
    int cs = client_socket("127.0.0.1", "9991");
    int n = tcptowrite(cs, "helloworl", 10, 1000);
    printf("write ok %d \n", n);
}


