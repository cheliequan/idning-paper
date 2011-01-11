#include<stdio.h>
#include<unistd.h>
#include<assert.h>

#include "network.h"
#include "log.h"

int client();
int server();
char buffer[10000];
char buffer2[10000];


void read_file(char * path, char * buff, int bufferlen);
int test();
int shutdown_osd();



int main(){
    int pid ;
    if ((pid = fork()) > 0){
        execv("osd/osd.out", NULL); //父进程中起server
    }else{
        sleep(2);
        test(); 
        shutdown_osd();
        /*kill(getppid(), 9);*/
        /*exit(0);*/
    }
    return 0;
}


int shutdown_osd(){
    int cs = client_socket("127.0.0.1", "6006");
    char * getpath = "GET /shutdown HTTP/1.1\r\n\r\n";
    tcptowrite(cs, getpath, strlen(getpath), 1000);
    return 0;

}

int test(){
    int cs = client_socket("127.0.0.1", "6006");
    char * getpath = "GET /get HTTP/1.1\r\n\r\n";
    tcptowrite(cs, getpath, strlen(getpath), 1000);
    char * p = buffer;
    int readed = 0;
    while ((readed = tcptoread(cs, p, 1024, 1000)) > 0){
        printf("readed: %d \n", readed);
        p += readed;
    }
    char * rst = strstr(buffer, "\r\n\r\n");
    rst += 4;

    printf("buffer : \n%s\n", rst);

    read_file("Makefile", buffer2, 10000);
    printf("buffer2: \n%s\n", buffer2);
    assert(strcmp(rst, buffer2) == 0);
    tcpclose(cs);
    return 0;
}

void read_file(char * path, char * buff, int bufferlen){
    int fd = open(path, O_RDONLY);
    int readed;
    char * p = buff;
    while ((readed = read(fd, p, 1024)) > 0){
        printf("readed: %d \n", readed);
        p += readed;
    }
    close(fd);
}

