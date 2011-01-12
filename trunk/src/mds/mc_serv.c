#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <event.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "protocol.gen.h"
#include "log.h"

void read_callback(struct bufferevent * bev, void * ctx);
void write_callback(struct bufferevent * bev, void * ctx);

//mc_serv_conn.state;
enum {KILL, HEADER, DATA};

typedef struct mc_serv_conn{
    uint8_t state; 
    uint32_t bytesleft;
    void * readbuf;
    void * readstart;
    void * msg;

}mc_serv_conn;

void error_callback(struct bufferevent *bev, short error, void *ctx)
{
    if (error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(bev);
}
void accept_callback(evutil_socket_t listener, short event, void * arg){
    struct event_base * base = arg;
    struct sockaddr sa;
    int salen = sizeof(sa);
    int fd = accept(listener, &sa, &salen);
    if (fd < 0){
        perror("error on accept");
    }else if (fd > FD_SETSIZE){
        close(fd);
    }else{
        struct bufferevent * bev;
        evutil_make_socket_nonblocking(fd);
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, read_callback, NULL, error_callback, NULL );
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}



#define HEADER_SIZE (sizeof(msg_header))
/* 这个函数基本上是 mfsmaster/matocsserv.c 中 matocsserv_read 的克隆.
 * */
void read_callback(struct bufferevent * bev, void * ctx){
    struct evbuffer * in = bufferevent_get_input(bev);
    struct evbuffer * out = bufferevent_get_output(bev);
    mc_serv_conn * conn = (mc_serv_conn *) ctx;

    if (evbuffer_get_length(in) < conn-> bytesleft)
        return;//还没有读够

    if(conn->state == HEADER){
        uint8_t tmp[HEADER_SIZE];
        msg_header header ;
        bufferevent_read(bev, tmp, HEADER_SIZE);
        msg_header_unpack(&header, tmp, HEADER_SIZE);
        conn->readbuf = malloc(header.msglength);
        if (conn->readbuf == NULL){
            logging(LOG_ERROR, "TODO: % s Out of Memory");
            conn->state = KILL;
            return;
        }
        conn->state = DATA;
        memcpy(conn->readbuf, tmp, HEADER_SIZE);
        conn->bytesleft = header.msglength - HEADER_SIZE;
        conn->readstart = conn->readbuf + HEADER_SIZE;
    }
    if (conn->state == DATA){
        bufferevent_read(bev, conn->readstart, HEADER_SIZE);
        mc_serv_got_msg(conn);
    }
}

int mc_serv_got_msg(mc_serv_conn *conn, int type, uint8_t * readbuf, int readsize){
    switch(type){
        case MC_MSG_NULL:
            break;
        case MC_MSG_MKDIR:
            break;
    }
}

int main(){
    struct evnet_base * base;
    struct event * listener_event;

    base = event_base_new();
    if (!base)
        return -1;
    int listener = server_socket("127.0.0.1", "9991");
    if (listener < 0){
        logging(LOG_ERROR, "socket error!");
        return -1;
    }
    evutil_make_socket_nonblocking(listener);
    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, accept_callback, (void*)base);
    /*XXX check it */
    event_add(listener_event, NULL);
    event_base_dispatch(base);
}
