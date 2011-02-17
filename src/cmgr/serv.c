#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <event.h>
#include <evhttp.h>
#include <evutil.h>

#include "network.h"
#include "log.h"
#include "dlist.h"
#include "datapack.h"


void shutdown_handler(struct evhttp_request *req, void * arg){
    logging(LOG_DEUBG, "%s: called\n", __func__);

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        fprintf(stdout, "FAILED (put type)\n");
        exit(1);
    }
    exit(0);
}

void ping_handler(struct evhttp_request *req, void * arg){
    logging(LOG_DEUBG, "%s: called\n", __func__);
    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
        logging(LOG_INFO, "Request is not POST");
        exit(1);
    }
    char * line = evbuffer_readline(req->input_buffer);
}



typedef struct cluster_machine{
    uint64_t uuid;
    char * ip;
    int port;
    char type;

    struct cluster_machine * next;
    struct cluster_machine * prev;
}cluster_machine;

#define CLUSTER_MACHINE_TYPE_MDS 1
#define CLUSTER_MACHINE_TYPE_OSD 2
#define CLUSTER_MACHINE_TYPE_CLIENT 3

cluster_machine cluster_head_v;
cluster_machine * cluster_head = & cluster_head_v;

cluster_machine * cluster_machine_new(){
    cluster_machine * m = (cluster_machine * )malloc(sizeof(cluster_machine));
    memset(m, 0, sizeof(cluster_machine));
}

static uint64_t cluster_version = 0;

void cluster_init(){
    dlist_init(cluster_head);
}

void cluster_add(char * ip, int port, char type){
    cluster_machine *m = cluster_machine_new();
    m -> ip = ip;
    m -> port = port;
    m -> type = type;
    dlist_insert_tail(cluster_head, m);
}

void cluster_remove(char * ip, int port){
    /*dlist_insert_tail(head, entries+i );*/
}

void cluster_map(){

}

void cluster_dump(){

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

    /*if (strcmp(op, "put") == 0){*/
        /*write_chunk(chunkid, req);*/
    /*}else if (strcmp(op, "get") == 0){*/
        /*read_chunk(chunkid, req);*/
    /*}else {*/
        /*evbuffer_add(evb, "...", 3);*/
        /*evbuffer_add_printf(evb, "%s, %"PRIx64, op, chunkid);*/
        /*evhttp_send_reply(req, HTTP_OK, "OK", evb);*/
        /*evbuffer_free(evb);*/
    /*}*/
}

int main(int argc, char **argv){

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
    evhttp_set_cb(httpd, "/shutdown", shutdown_handler, NULL);
    evhttp_set_gencb(httpd, gen_handler, NULL);
    event_dispatch();

    evhttp_free(httpd);
    return 0;
}
