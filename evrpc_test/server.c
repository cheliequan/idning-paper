#include <event.h>
#include <evhttp.h>
#include "test.gen.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_ping, ping, pong)


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


