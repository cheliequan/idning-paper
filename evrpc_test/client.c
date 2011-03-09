#include <event.h>
#include <evhttp.h>
#include <stdlib.h>
#include "test.gen.h"
#include "common.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_ping, ping, pong)

static void
rpc_postrequest_failure(struct evhttp_request *req, void *arg)
{
    if (req->response_code != HTTP_SERVUNAVAIL) {

        fprintf(stderr, "FAILED (response code)\n");
        exit(1);
    }
    event_loopexit(NULL);
}

static void ping_cb(struct evrpc_status *status,
    struct ping *ping , struct pong * pong, void *arg)
{

    fprintf(stderr, "%s: called\n", __func__);
    int v;
    EVTAG_GET(pong, version, &v);

    printf("get version is : %d \n", v);
    int cnt = EVTAG_ARRAY_LEN(pong, machines);
    int i;
    for (i=0; i< cnt; i++){
        struct machine * m;
        EVTAG_ARRAY_GET(pong, machines, 0, &m);
        printf("machine %d: \n", i);
        
        printf("m->port : %d \n", m->port);
        printf("m->ip : %s \n", m->ip);
    }


done:
    event_loopexit(NULL);
}


struct evrpc_pool *pool = NULL;

static void
rpc_basic_client(void)
{
    ev_uint16_t port = 9527;
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;


    /* set up the basic message */
    struct ping * ping = ping_new();
    EVTAG_ASSIGN(ping, version, 7);
    EVTAG_ASSIGN(ping, self_ip, "127.0.0.2");
    EVTAG_ASSIGN(ping, self_port, 8080);
    struct pong * pong =  pong_new();

    struct evhttp_connection *evcon;

    pool = evrpc_pool_new(NULL); 

    evcon = evhttp_connection_new("127.0.0.1", port);
    fprintf(stderr, "evcon : %p\n", evcon);

    evrpc_pool_add_connection(pool, evcon);

    int rst = EVRPC_MAKE_REQUEST(rpc_ping, pool, ping , pong,  ping_cb, NULL);
    fprintf(stderr, "rst: %d\n", rst);

    event_dispatch();
}

void doit(){
    struct ping * ping = ping_new();
    EVTAG_ASSIGN(ping, version, 7);
    EVTAG_ASSIGN(ping, self_ip, "127.0.0.2");
    EVTAG_ASSIGN(ping, self_port, 8080);
    struct pong * pong =  pong_new();


    int rst = EVRPC_MAKE_REQUEST(rpc_ping, pool, ping , pong,  ping_cb, NULL);
    event_dispatch();

}

/*int main(){*/
    /*event_init();*/
    /*rpc_basic_client();*/
    /*doit();*/
/*}*/
