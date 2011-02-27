#include <event.h>
#include <evhttp.h>
#include "test.gen.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_ping, ping, pong)

#define MAX_MACHINE_CNT 256

static struct machine machines [MAX_MACHINE_CNT];
static int machine_cnt = 0;
static uint64_t cluster_version = 0;

static void
ping_handler(EVRPC_STRUCT(rpc_ping)* rpc, void *arg)
{
    fprintf(stderr, "%s: called\n", __func__);

    struct ping * ping = rpc->request;
    struct pong * pong = rpc->reply;

#define EV_GET(msg, member) \
        (msg->member##_data)

    int ping_version = EV_GET(ping, version);
    cluster_add((char *)ping->self_ip, ping->self_port, 0);


    EVTAG_ASSIGN(pong, version, ping_version+1);
    EVTAG_ASSIGN(pong, xx, 8);
    /*EVTAG_ASSIGN(pong, machines, 8, machines);*/
    pong_machines_assign(pong, 8, machines);

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


