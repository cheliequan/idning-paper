#include <stdlib.h>
#include <string.h>

#include <event.h>

#include "network.h"
#include "log.h"

int main(){
    server();
}

void sock_read(int fd, short event, void *arg){
    logging(LOG_INFO, "sock_read");
    char buf[1024];
    int len;
    printf("event is : %d\n", event);
    struct event *ev = arg;
    if((len = recv(fd, buf, sizeof(buf)-1,0)) == -1)
        send(fd, buf, strlen(buf)+1, 0);
    printf("read len: %d\n", len);
    event_add(ev, NULL);
}

static void sock_accept(int fd, short event, void *arg){
    logging(LOG_INFO, "sock_accept");
    struct event *ev = arg;
    struct sockaddr addr;
    int s;
    socklen_t len = sizeof(addr);
    struct event *rev = (struct event *)malloc(sizeof(*rev));

    if((s = accept(fd, &addr, &len)) == -1){
        fprintf(stderr, "Sock Accept Failed!\n");
        exit(0);
    }
    event_set(rev, s, EV_READ, sock_read, rev);
    event_add(rev, NULL);
    /*event_add(ev, NULL);*/
}

int server(){
    struct event ev;

    int ss = server_socket("127.0.0.1", "9991");
    if (ss < 0){
        logging(LOG_ERROR, "socket error!");
        return -1;
    }

    event_init();
    event_set(&ev, ss, EV_READ|EV_PERSIST, sock_accept, &ev);
    /*event_set(&ev, ss, EV_READ, sock_accept, &ev);*/
    event_add(&ev, NULL);
    event_dispatch();
}
