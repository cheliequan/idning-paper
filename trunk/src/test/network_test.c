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
        sleep(3);
    }
}

int server(){
    char buffer[1024];
    int ss = server_socket("127.0.0.1", "9991");
    int ns = tcpaccept(ss);
    if (ns < 0){
        logging(LOG_ERROR, "accept error!");
        return -1;
    }
    tcpnonblock(ns);
    tcpnodelay(ns);
    int n = tcpread(ns, buffer, 10, 3000);
    printf("read ok %d !!\n", n);
    printf("%s\n", buffer);
}


int client(){
    int cs = client_socket("127.0.0.1", "9991");
    int n = tcpwrite(cs, "helloworl", 10, 1000);
    printf("write ok %d \n", n);
}


