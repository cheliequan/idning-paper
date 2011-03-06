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


static void error_reply(struct evhttp_request *req, int err_code, char * fmt, ...){
    DBG();
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf) -30 , fmt, ap);
    va_end(ap);

    logging(LOG_ERROR, "error_reply : %s", buf);

    /*struct evbuffer *evb = evbuffer_new();*/
    /*evhttp_send_reply(req, err_code, buf, evb);*/
    evhttp_send_error(req, err_code, buf);
}

void shutdown_handler(struct evhttp_request *req, void * arg){
    DBG();

    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        error_reply(req, HTTP_BADREQUEST, "should call with GET");
        return;
    }
    exit(0);
}


void write_chunk(uint64_t chunkid, struct evhttp_request *req){
    DBG();
    struct evbuffer *input;
    struct evbuffer *evb = evbuffer_new();

    if (evhttp_request_get_command(req) != EVHTTP_REQ_POST) {
        error_reply(req, HTTP_BADREQUEST, "should call write with POST");
        return;
    }
    int start=0, end;
    const char * range = evhttp_find_header(req->input_headers, "Range");

    logging(LOG_DEUBG, "write range : %s", range);
    if (range){
        sscanf(range, "bytes=%d-%d", &start, &end);
    }

    input = req->input_buffer;
    hdd_chunk * chunk = hdd_create_chunk(chunkid, 0);//TODO

    int fd = open(chunk -> path, O_WRONLY|O_CREAT, 0755);
    logging(LOG_INFO, "write seek to : %d ", start);
    lseek(fd, start, SEEK_SET);

    if (-1 == fd) {
        error_reply(req, HTTP_INTERNAL, "could not open file : %s", chunk->path);
        return;
    }

    evbuffer_write(input, fd);
    close(fd);

    evbuffer_add(evb, "success", strlen("success"));
    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}

void read_chunk(uint64_t chunkid, struct evhttp_request *req){
    DBG();
    struct evbuffer *evb = evbuffer_new();
    hdd_chunk * chunk = chunk_hashtable_get(chunkid);

    if (chunk == NULL){
        error_reply(req, HTTP_NOTFOUND, "not found chunk %"PRIx64 ";", chunkid);
        return ;
    }

    hdd_chunk_printf(chunk);
    
    int fd = open(chunk -> path, O_RDONLY);
    struct stat st;

    if (fstat(fd, &st)<0) {
        error_reply(req, HTTP_NOTFOUND, "file not exist : %s", chunk->path);
        return ;
    }
    evbuffer_add_file(evb, fd, 0, st.st_size);

    evhttp_send_reply(req, HTTP_OK, "OK", evb);
    evbuffer_free(evb);
}



void gen_handler(struct evhttp_request *req, void * arg){
    DBG();
    /*struct evbuffer *evb = evbuffer_new();*/

    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded_uri = NULL;
    const char *path;
    char *decoded_path;
    

    /* Decode the URI */
    decoded_uri = evhttp_uri_parse(uri);
    if (!decoded_uri) {
        error_reply(req, HTTP_BADREQUEST, "Bad URI: %s", uri);
        return;
    }

    /* Let's see what path the user asked for. */
    path = evhttp_uri_get_path(decoded_uri);
    if (!path) {
        logging(LOG_INFO, "request path is nil, replace it as /");
        path = "/";
    }

    /* We need to decode it, to see what path the user really wanted. */
    decoded_path = evhttp_uridecode(path, 0, NULL);

    uint64_t chunkid;
    char * slash = strchr(path+1, '/');
    *slash = '\0';
    const char * op = path + 1; 
    sscanf(slash+1, "%"SCNx64, &chunkid);

    logging(LOG_INFO, "%s, %"PRIx64, op, chunkid);

    if (strcmp(op, "put") == 0){
        write_chunk(chunkid, req);
    }else if (strcmp(op, "get") == 0){
        read_chunk(chunkid, req);
    }else {
        error_reply(req, HTTP_NOTFOUND, "not found: %s", uri);
        return;
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
        logging(LOG_ERROR, "start server error %m");
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
