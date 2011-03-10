#include "http_client.h"
#include "log.h"

#include <event.h>
#include <evhttp.h>
#include <evutil.h>
#include <event2/buffer.h>
#include <unistd.h>
#include <assert.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>

static http_response * http_response_new(int status_code, struct evbuffer * headers, struct evbuffer * body);
void http_response_free(http_response * r);
struct http_response *http_get(const char *url,  struct evkeyvalq* headers);
struct http_response *http_post(const char *url, struct evkeyvalq* headers, struct evbuffer * postdata);

struct request_context {
    struct evhttp_uri *uri;
    struct evhttp_connection *conn;
    struct evhttp_request *req;

    struct evbuffer *buffer;

    int method;
    struct evkeyvalq * req_headers;
    struct evbuffer *postdata_buffer;
    int ok;
};

TAILQ_HEAD(connection_queue, connection_wrapper) s_connections;
struct connection_queue * connection_pool = & s_connections;
struct connection_wrapper {
    TAILQ_ENTRY(connection_wrapper) queue_entry;
    struct evhttp_connection *conn;
    /*char * host;*/
    /*int port;*/
    
};

static void connection_pool_init(){
    TAILQ_INIT(connection_pool); 

}

static void connection_pool_insert( struct evhttp_connection * conn){
    DBG();
    struct connection_wrapper * cw = malloc(sizeof(struct connection_wrapper));      
    cw->conn = conn;
    TAILQ_INSERT_HEAD(connection_pool, cw, queue_entry);
}

static struct evhttp_connection * connection_pool_get_free_conn( ){
    DBG();
    if (connection_pool->tqh_first != NULL){
        struct connection_wrapper * cw = connection_pool -> tqh_first;
        struct evhttp_connection * conn = cw->conn;
        TAILQ_REMOVE(connection_pool, connection_pool->tqh_first, queue_entry);
        free(cw);
        logging(LOG_DEUBG, "connection_pool_get_free_conn return a conn!!");
        return conn;
    }
    logging(LOG_DEUBG, "connection_pool_get_free_conn return NULL!!");
    return NULL;
}

static void client_callback(struct evhttp_request *req, void *arg);

static int client_renew_request(struct request_context *ctx);

static void client_callback(struct evhttp_request *req, void *arg)
{
    struct request_context *ctx = (struct request_context *)arg;
    struct evhttp_uri *new_uri = NULL;
    const char *new_location = NULL;

    switch(req->response_code)
    {
    case HTTP_OK:
        event_loopexit( 0);
        break;

    case HTTP_MOVEPERM:
    case HTTP_MOVETEMP:
        new_location = evhttp_find_header(req->input_headers, "Location");
        if (!new_location)
            return;

        new_uri = evhttp_uri_parse(new_location);
        if (!new_uri)
            return;

        evhttp_uri_free(ctx->uri);
        ctx->uri = new_uri;

        client_renew_request(ctx);
        return;

    default:
        /* FAILURE */
        event_loopexit(0);
        return;
    }

    evbuffer_add_buffer(ctx->buffer, req->input_buffer);

    /* SUCCESS */
    ctx->ok = 1;
}

struct request_context *context_new (const char *url, int verb, struct evkeyvalq* headers, struct evbuffer * data) 
{
    struct request_context *ctx = 0;
    ctx = calloc(1, sizeof(*ctx));
    if (!ctx)
        return 0;

    ctx->uri = evhttp_uri_parse(url);
    ctx -> method = verb;
    ctx -> postdata_buffer = data;
    ctx -> req_headers = headers;

    fprintf(stderr, "DEBUG: url-> path : %s \n",ctx->uri->path );
    fprintf(stderr, "DEBUG: url-> method : %s \n",ctx->uri->path );
    if (!ctx->uri)
        return 0;

    ctx->buffer = evbuffer_new();

    client_renew_request(ctx);

    return ctx;
}

void context_free(struct request_context *ctx)
{

    connection_pool_insert(ctx->conn);
    /*evhttp_connection_free(ctx->conn);*/

    if (ctx->buffer)
        evbuffer_free(ctx->buffer);

    evhttp_uri_free(ctx->uri);
    free(ctx);
}

static int client_renew_request(struct request_context *ctx)
{
    /* free connections & request */
    if (ctx->conn)
        evhttp_connection_free(ctx->conn);
    struct evhttp_connection * conn = connection_pool_get_free_conn();
    if (conn!=NULL){
        ctx->conn = conn;
        //fprintf(stderr, " conn->state: %d\n", conn->state);

        /*conn*/
        /*evhttp_connection_reset(ctx->conn); no !!! this will send FIN . what should I do*/
    }else{
        ctx->conn = evhttp_connection_new( ctx->uri->host, ctx->uri->port > 0 ? ctx->uri->port : 80);
        //fprintf(stderr, " conn->state: %d\n", conn->state);
    }

    ctx->req = evhttp_request_new(client_callback, ctx);


    struct evkeyval *header;

    if (ctx->req_headers != NULL){
        struct evkeyval *header;
        TAILQ_FOREACH(header, ctx->req_headers, next) {
            evhttp_add_header(ctx->req->output_headers, header->key, header->value);
        }
    }



    fprintf(stderr, " .... \n");
    TAILQ_FOREACH(header, ctx->req->output_headers, next) {
        fprintf(stderr, "headers : %s ; %s\n", header->key, header->value);
    }
    fprintf(stderr, " .... \n");

    evhttp_add_header(ctx->req->output_headers,
                            "Host", ctx->uri->host);
    evhttp_add_header(ctx->req->output_headers,
                            "tt", "v_of_tt");
    
    if (ctx->method == EVHTTP_REQ_POST){
        ctx->req->output_buffer = ctx->postdata_buffer;
        evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_POST, ctx->uri->path ? ctx->uri->path : "/");

    }else if (ctx->method == EVHTTP_REQ_GET){
        evhttp_make_request(ctx->conn, ctx->req, EVHTTP_REQ_GET, ctx->uri->path ? ctx->uri->path : "/");
    }else {
        assert(0);
    }
    
    return 0;
}

static struct http_response *http_request(const char *url, int verb, struct evkeyvalq* headers, struct evbuffer * data)
{
    fprintf(stderr, "http_request: %s, %d\n",url, verb );
    struct request_context *ctx = context_new(url, verb, headers, data);
    fprintf(stderr, "ctx->req: %p\n", ctx->req);
    fprintf(stderr, "ctx->req->output_headers: %p\n", ctx->req->output_headers);


    /*fprintf(stderr, " after add headers ----------------\n");*/

    /*TAILQ_FOREACH(header, ctx->req->output_headers, next) {*/
        /*fprintf(stderr, "headers : %s ; %s\n", header->key, header->value);*/
    /*}*/

    /*fprintf(stderr, "http_request: method : %d\n", ctx->method);*/
    event_dispatch();

    struct evbuffer *body = 0;
    int response_code = ctx->req->response_code;

    if (ctx->ok)
    {
        body = ctx->buffer;
        ctx->buffer = 0;
        context_free(ctx);
        struct evbuffer * header = evbuffer_new();

        /*evbuffer_drain(body, evbuffer_get_length(body)+10);*/

        return http_response_new(200, header, body);

    }else{
        logging(LOG_INFO, "http_request error %d : %s ", response_code, url );
        return NULL;
    }
}

struct http_response *http_get(const char *url,  struct evkeyvalq* headers){
    return http_request(url, EVHTTP_REQ_GET, headers, NULL);
}

struct http_response *http_post(const char *url, struct evkeyvalq* headers, struct evbuffer * postdata){
    return http_request(url, EVHTTP_REQ_POST, headers, postdata);
}

http_response * http_response_new(int status_code, struct evbuffer * headers, struct evbuffer * body){
    http_response * r = (http_response *)malloc(sizeof(http_response));
    r -> status_code = status_code;
    r -> headers = headers;
    r -> body = body;
    return r;
};


void http_response_free(http_response * r){
    if (r->headers)
        evbuffer_free(r -> headers);
    if(r->body)
        evbuffer_free(r -> body);
    free(r);
}

void http_client_init(){
    connection_pool_init();
}
