#include "http_client.h"

/*
 * Requires uri parsing helpers from:
 * http://sourceforge.net/tracker/?func=detail&aid=3037660&group_id=50884&atid=461324
 */

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


/*typedef struct http_response{*/
    /*int status_code;*/
    /*struct evbuffer * headers;*/
    /*struct evbuffer * body;*/
/*}http_response;*/

static http_response * http_response_new(int status_code, struct evbuffer * headers, struct evbuffer * body);
void http_response_free(http_response * r);
struct http_response * http_get(const char * url);
struct http_response *http_post(const char *url, struct evbuffer * postdata);

void http_response_free(http_response * r){
    evbuffer_free(r -> headers);
    evbuffer_free(r -> body);
    free(r);
}
    



/**this should be in http.h
 */
struct evhttp_uri {
    char *scheme; /* scheme; e.g http, ftp etc */
    char *userinfo; /* userinfo (typically username:pass), or NULL */
    char *host; /* hostname, IP address, or NULL */
    int port; /* port, or zero */
    char *path; /* path, or "". */
    char *query; /* query, or NULL */
    char *fragment; /* fragment or NULL */
};

struct request_context
{
    struct evhttp_uri *uri;
    struct evhttp_connection *cn;
    struct evhttp_request *req;

    struct evbuffer *buffer;

    int method;
    struct evbuffer *postdata_buffer;
    int ok;
};

static
void download_callback(struct evhttp_request *req, void *arg);

static
int download_renew_request(struct request_context *ctx);

static
void download_callback(struct evhttp_request *req, void *arg)
{
    struct request_context *ctx = (struct request_context *)arg;
    struct evhttp_uri *new_uri = NULL;
    const char *new_location = NULL;
    switch(req->response_code)
    {
    case HTTP_OK:
        /* 
         * Response is received. No futher handling is required.
         * Finish
         */
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

        download_renew_request(ctx);
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

struct request_context *context_new (const char *url, int verb, struct evbuffer * data) 
{
    struct request_context *ctx = 0;
    ctx = calloc(1, sizeof(*ctx));
    if (!ctx)
        return 0;

    ctx->uri = evhttp_uri_parse(url);
    ctx -> method = verb;
    ctx -> postdata_buffer = data;

    fprintf(stderr, "DEBUG: url-> path : %s \n",ctx->uri->path );
    fprintf(stderr, "DEBUG: url-> method : %s \n",ctx->uri->path );
    if (!ctx->uri)
        return 0;

    ctx->buffer = evbuffer_new();

    download_renew_request(ctx);

    return ctx;
}

void context_free(struct request_context *ctx)
{
    evhttp_connection_free(ctx->cn);

    if (ctx->buffer)
        evbuffer_free(ctx->buffer);

    evhttp_uri_free(ctx->uri);
    free(ctx);
}

static
int download_renew_request(struct request_context *ctx)
{
    /* free connections & request */
    if (ctx->cn)
        evhttp_connection_free(ctx->cn);

    ctx->cn = evhttp_connection_new( ctx->uri->host, ctx->uri->port > 0 ? ctx->uri->port : 80);

    ctx->req = evhttp_request_new(download_callback, ctx);
    
    if (ctx->method == EVHTTP_REQ_POST){
        fprintf(stderr, "EVHTTP_REQ_POST\n");
        ctx->req->output_buffer = ctx->postdata_buffer;
        evhttp_make_request(ctx->cn, ctx->req, EVHTTP_REQ_POST, ctx->uri->path ? ctx->uri->path : "/");
    }else if (ctx->method == EVHTTP_REQ_GET){
        fprintf(stderr, "EVHTTP_REQ_GET\n");
        
        evhttp_make_request(ctx->cn, ctx->req, EVHTTP_REQ_GET, ctx->uri->path ? ctx->uri->path : "/");
    }else {
        assert(0);
    }

    evhttp_add_header(ctx->req->output_headers,
                            "Host", ctx->uri->host);
    return 0;
}

struct http_response *http_request(const char *url, int verb, struct evbuffer * data)
{
    fprintf(stderr, "http_request: %s, %d\n",url, verb );
    struct request_context *ctx = context_new(url, verb, data);
    /* do all of the job */
    fprintf(stderr, "http_request: method : %d\n", ctx->method);
    event_dispatch();

    struct evbuffer *body = 0;
    if (ctx->ok)
    {
        body = ctx->buffer;
        ctx->buffer = 0;
    }

    context_free(ctx);

    struct evbuffer * header = evbuffer_new();

    return http_response_new(200, header, body);
}

struct http_response *http_get(const char *url){
    return http_request(url, EVHTTP_REQ_GET, NULL);
}

struct http_response *http_post(const char *url, struct evbuffer * postdata){
    return http_request(url, EVHTTP_REQ_POST, postdata);
}

int main_test(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: %s http://example.com/\n", argv[0]);
        return 1;
    }

    event_init();
    struct http_response * resp = http_get(argv[1]);
    struct evbuffer *data = resp->body;

    printf("got %d bytes\n", data ? evbuffer_get_length(data) : -1);

    if (data)
    {
        const char *joined = evbuffer_pullup(data, -1);
        printf("data itself:\n====================\n");
        write(1, joined, evbuffer_get_length(data));
        printf("\n====================\n");

        evbuffer_free(data);
    }

    return 0;
}

static http_response * http_response_new(int status_code, struct evbuffer * headers, struct evbuffer * body){
    http_response * r = (http_response *)malloc(sizeof(http_response));
    r -> status_code = status_code;
    r -> headers = headers;
    r -> body = body;
    return r;
};


