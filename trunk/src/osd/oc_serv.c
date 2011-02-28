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
#include "http_client.h"
#include "protocol.gen.h"

/*static machine this_machine = {*/
    /*0, */
    /*9,*/
    /*"127.0.0.1",*/
    /*9528,*/
    /*CLUSTER_MACHINE_TYPE_OSD,*/
/*};*/

/*static machine mgr_machine = {*/
    /*0, */
    /*9,*/
    /*"127.0.0.1",*/
    /*9527,*/
    /*CLUSTER_MACHINE_TYPE_MGR,*/
/*};*/
/*//curl http://localhost:9527/ping -d"      41,15684944,0,106,9,127.0.0.1,9528,"*/
/*int do_ping(machine * this, machine * m){*/
    /*char buffer[2560];*/
    /*ping * msg_ping = ping_new(); //TODO: cache this*/
    
    /*msg_ping -> self_iplength = this->iplength;*/
    /*msg_ping -> self_ip = this->ip;*/
    /*msg_ping -> self_port = this->port;*/

    /*ping_pack(msg_ping, buffer, 0);*/

    /*struct http_response * response = http_post(m->ip, m->port, "/ping", buffer);*/
    /*//TODO: here, unpack*/
    /*//*/
    /*//*/
    /*char * p;*/
    /*printf("%d\n", response -> status_code);*/

    /*while( (p = evbuffer_readline(response->headers)) ){*/
        /*printf("%s\n", p);*/
        /*free(p);*/
    /*}*/

    /*while( ( p = evbuffer_readline(response->body))){*/
        /*printf("%s\n", p);*/
        /*free(p);*/
    /*}*/
    /*return 0;*/
/*}*/

//void post_handler(struct evhttp_request *req, void * arg){
//    struct evbuffer *input;
//    struct evbuffer *evb = evbuffer_new();
//    logging(LOG_DEUBG, "%s called\n", __func__);
//
//    /* Expecting a PUT request */
//    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
//        fprintf(stdout, "FAILED (put type)\n");
//        exit(1);
//    }
//    input = req->input_buffer;
//    evbuffer_write(input, STDOUT_FILENO);
//
//    evbuffer_add(evb, "abc", 3);
//    evhttp_send_reply(req, HTTP_OK, "OK", evb);
//    evbuffer_free(evb);
//}

void shutdown_handler(struct evhttp_request *req, void * arg){
    logging(LOG_DEUBG, "%s: called\n", __func__);

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        fprintf(stdout, "FAILED (put type)\n");
        exit(1);
    }
    exit(0);
}

//void get_handler(struct evhttp_request *req, void * arg){
//    struct evbuffer *input;
//    struct evbuffer *evb = evbuffer_new();
//    logging(LOG_DEUBG, "%s: called\n", __func__);
//
//    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
//        fprintf(stdout, "FAILED (put type)\n");
//        exit(1);
//    }
//    input = req->input_buffer;
//    int fd = open("Makefile", O_RDONLY);
//    struct stat st;
//
//    if (fstat(fd, &st)<0) {
//        perror("fstat");
//    }
//
//    evbuffer_add_file(evb, fd, 0, st.st_size);
//
//    evhttp_send_reply(req, HTTP_OK, "OK", evb);
//    evbuffer_free(evb);
//}

void write_chunk(uint64_t chunkid, struct evhttp_request *req){
    struct evbuffer *input;
    struct evbuffer *evb = evbuffer_new();
    logging(LOG_DEUBG, "%s called\n", __func__);

    /* Expecting a POST request */
    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
        fprintf(stdout, "FAILED (put type)\n");
        exit(1);
    }
    input = req->input_buffer;
    hdd_chunk * chunk = hdd_create_chunk(chunkid, 0);//TODO

    int fd = open(chunk -> path, O_WRONLY|O_CREAT, 0755);
    if (-1 == fd) {
        logging(LOG_ERROR, "file open error <%s>", chunk->path);
        
        exit(1);
    }

    //struct stat st;

    /*if (fstat(fd, &st)<0) {*/
        /*perror("file not exist or access deny");*/
    /*}*/

    evbuffer_write(input, fd);
    close(fd);

    evbuffer_add(evb, "abc", 3);
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);

}

void read_chunk(uint64_t chunkid, struct evhttp_request *req){
    struct evbuffer *evb = evbuffer_new();
    logging(LOG_DEUBG, "%s: called\n", __func__);

    hdd_chunk * chunk = chunk_hashtable_get(chunkid);
    hdd_chunk_printf(chunk);
    
    int fd = open(chunk -> path, O_RDONLY);
    struct stat st;

    if (fstat(fd, &st)<0) {
        perror("file not exist or access deny");
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

    if (strcmp(op, "put") == 0){
        write_chunk(chunkid, req);
    }else if (strcmp(op, "get") == 0){
        read_chunk(chunkid, req);
    }else {
        evbuffer_add(evb, "...", 3);
        evbuffer_add_printf(evb, "%s, %"PRIx64, op, chunkid);
        evhttp_send_reply(req, HTTP_OK, "OK", evb);
        evbuffer_free(evb);
    }
}

int main(int argc, char **argv){
    hdd_init("etc/hdd.conf");
    /*do_ping(&this_machine, &mgr_machine);*/

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
    /*evhttp_set_cb(httpd, "/_put", post_handler, NULL);*/
    /*evhttp_set_cb(httpd, "/_get", get_handler, NULL);*/
    evhttp_set_cb(httpd, "/shutdown", shutdown_handler, NULL);
    evhttp_set_gencb(httpd, gen_handler, NULL);
    event_dispatch();

    evhttp_free(httpd);
    return 0;
}
