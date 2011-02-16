#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <event.h>
#include <evhttp.h>
#include <evutil.h>

#include "oc_serv.h"
#include "network.h"
#include "log.h"
#include "hdd.h"


void post_handler(struct evhttp_request *req, void * arg){
    struct evbuffer *input;
    struct evbuffer *evb = evbuffer_new();
    logging(LOG_DEUBG, "%s called\n", __func__);

    /* Expecting a PUT request */
    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
        fprintf(stdout, "FAILED (put type)\n");
        exit(1);
    }
    input = req->input_buffer;
    evbuffer_write(input, STDOUT_FILENO);

    evbuffer_add(evb, "abc", 3);
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void shutdown_handler(struct evhttp_request *req, void * arg){
    logging(LOG_DEUBG, "%s: called\n", __func__);

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        fprintf(stdout, "FAILED (put type)\n");
        exit(1);
    }
    exit(0);
}

void get_handler(struct evhttp_request *req, void * arg){
    struct evbuffer *input;
    struct evbuffer *evb = evbuffer_new();
    logging(LOG_DEUBG, "%s: called\n", __func__);

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        fprintf(stdout, "FAILED (put type)\n");
        exit(1);
    }
    input = req->input_buffer;
    int fd = open("Makefile", O_RDONLY);
    struct stat st;

    if (fstat(fd, &st)<0) {
        perror("fstat");
    }

    evbuffer_add_file(evb, fd, 0, st.st_size);

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void gen_handler(struct evhttp_request *req, void * arg){
    struct evbuffer *evb = evbuffer_new();

    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded_uri = NULL;
    const char *path;
    char *decoded_path;
    

    /* Decode the URI */
    decoded_uri = evhttp_uri_parse(uri);
    if (!decoded_uri) {
        logging(LOG_DEUBG, "It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    /* Let's see what path the user asked for. */
    path = evhttp_uri_get_path(decoded_uri);
    if (!path) path = "/";

    /* We need to decode it, to see what path the user really wanted. */
    decoded_path = evhttp_uridecode(path, 0, NULL);

    uint64_t chunkid;
    char * slash = strchr(path+1, '/');
    *slash = '\0';
    const char * op = path + 1; 

    sscanf(slash+1, "%"SCNx64, &chunkid);
    logging(LOG_DEUBG, "%s, %"PRIx64, op, chunkid);

    evbuffer_add(evb, "...", 3);
    evbuffer_add_printf(evb, "%s, %"PRIx64, op, chunkid);
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

int main(int argc, char **argv){
    hdd_init("etc/hdd.conf");

    struct evhttp * httpd;
    int port = 6006;
    event_init();
    httpd = evhttp_start("0.0.0.0", port);
    if( httpd == NULL){
        fprintf(stderr, "start server error %m\n");
        exit(1);
    }else{
        printf("Start server at port  %d\n", port);
    }
    evhttp_set_cb(httpd, "/_store", post_handler, NULL);
    evhttp_set_cb(httpd, "/_get", get_handler, NULL);
    evhttp_set_cb(httpd, "/shutdown", shutdown_handler, NULL);
    evhttp_set_gencb(httpd, gen_handler, NULL);
    event_dispatch();

    evhttp_free(httpd);
    return 0;
}

//int main(){
//    server();
//}
//
//void sock_read(int fd, short event, void *arg){
//    logging(LOG_INFO, "sock_read");
//    char buf[1024];
//    int len;
//    printf("event is : %d\n", event);
//    struct event *ev = arg;
//    if((len = recv(fd, buf, sizeof(buf)-1,0)) == 0){
//        tcpclose(fd);
//    }else{
//        event_add(ev, NULL);
//    }
//}
//
//static void sock_accept(int fd, short event, void *arg){
//    logging(LOG_INFO, "sock_accept");
//    struct event *ev = arg;
//    struct sockaddr addr;
//    int s;
//    socklen_t len = sizeof(addr);
//    struct event *rev = (struct event *)malloc(sizeof(*rev));
//
//    if((s = accept(fd, &addr, &len)) == -1){
//        fprintf(stderr, "Sock Accept Failed!\n");
//        exit(0);
//    }
//    event_set(rev, s, EV_READ, sock_read, rev);
//    event_add(rev, NULL);
//    /*event_add(ev, NULL);*/
//}
//
//int server(){
//    struct event ev;
//
//    int ss = server_socket("127.0.0.1", "9991");
//    if (ss < 0){
//        logging(LOG_ERROR, "socket error!");
//        return -1;
//    }
//
//    event_init();
//    event_set(&ev, ss, EV_READ|EV_PERSIST, sock_accept, &ev);
//    /*event_set(&ev, ss, EV_READ, sock_accept, &ev);*/
//    event_add(&ev, NULL);
//    event_dispatch();
//}
