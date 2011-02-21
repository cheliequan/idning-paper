#include <event.h>
#include <evhttp.h>
#include "test.gen.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_ping, ping, pong)


/*static struct evhttp * http_setup(ev_uint16_t *pport)*/
/*{*/
    /*struct evhttp *myhttp;*/
    /*ev_uint16_t port;*/
    /*struct evhttp_bound_socket *sock;*/

    /*myhttp = evhttp_new(NULL);*/
    /*if (!myhttp)*/
        /*event_errx(1, "Could not start web server");*/

    /*[> Try a few different ports <]*/
    /*sock = evhttp_bind_socket_with_handle(myhttp, "127.0.0.1", 0);*/
    /*if (!sock)*/
        /*event_errx(1, "Couldn't open web port");*/

    /*port = regress_get_socket_port(evhttp_bound_socket_get_fd(sock));*/

    /**pport = port;*/
    /*return (myhttp);*/
/*}*/

static void
ping_handler(EVRPC_STRUCT(rpc_ping)* rpc, void *arg)
{
    fprintf(stderr, "%s: called\n", __func__);

    struct ping * ping = rpc->request;
    struct pong * pong = rpc->reply;

#define EV_GET(msg, member) \
        (msg->member##_data)

    int ping_version = EV_GET(ping, version);


    EVTAG_ASSIGN(pong, version, ping_version+1);
    EVTAG_ASSIGN(pong, xx, 8);
    EVRPC_REQUEST_DONE(rpc);
    /*struct kill* kill_reply = rpc->reply;*/

    /*if (need_input_hook) {*/
        /*struct evhttp_request* req = EVRPC_REQUEST_HTTP(rpc);*/
        /*const char *header = evhttp_find_header(*/
            /*req->input_headers, "X-Hook");*/
        /*assert(strcmp(header, "input") == 0);*/
    /*}*/

    /*[> we just want to fill in some non-sense <]*/
    /*EVTAG_ASSIGN(kill_reply, weapon, "dagger");*/
    /*EVTAG_ASSIGN(kill_reply, action, "wave around like an idiot");*/

    /*[> no reply to the RPC <]*/
    /*EVRPC_REQUEST_DONE(rpc);*/
}

static void
rpc_setup(struct evhttp **phttp, ev_uint16_t *pport, struct evrpc_base **pbase)
{
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    int port = 9527;
    http = evhttp_start("0.0.0.0", port);

    base = evrpc_init(http);

    EVRPC_REGISTER(base, rpc_ping, ping, pong, ping_handler, NULL);

    *phttp = http;
    *pport = port;
    *pbase = base;

}

int main()
{
    ev_uint16_t port;
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;
    struct evrpc_pool *pool = NULL;

    event_init();
    rpc_setup(&http, &port, &base);
    event_dispatch();

}


