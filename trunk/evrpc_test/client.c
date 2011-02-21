#include <event.h>
#include <evhttp.h>
#include "test.gen.h"

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


    /*if (status->error != EVRPC_STATUS_ERR_NONE)*/
        /*goto done;*/

    /*if (EVTAG_GET(kill, weapon, &weapon) == -1) {*/
        /*fprintf(stderr, "get weapon\n");*/
        /*goto done;*/
    /*}*/
    /*if (EVTAG_GET(kill, action, &action) == -1) {*/
        /*fprintf(stderr, "get action\n");*/
        /*goto done;*/
    /*}*/

    /*if (strcmp(weapon, "dagger"))*/
        /*goto done;*/

    /*if (strcmp(action, "wave around like an idiot"))*/
        /*goto done;*/

    /*test_ok += 1;*/

done:
    event_loopexit(NULL);
}



static void
rpc_basic_client(void)
{
    ev_uint16_t port = 9527;
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    struct evrpc_pool *pool = NULL;

    /* set up the basic message */
    struct ping * ping = ping_new();
    EVTAG_ASSIGN(ping, version, 7);
    EVTAG_ASSIGN(ping, ip, "127.0.0.1");
    EVTAG_ASSIGN(ping, port, 8080);
    struct pong * pong =  pong_new();

    struct evhttp_connection *evcon;

    pool = evrpc_pool_new(NULL); 

    evcon = evhttp_connection_new("127.0.0.1", port);
    fprintf(stderr, "evcon : %p\n", evcon);

    evrpc_pool_add_connection(pool, evcon);

    int rst = EVRPC_MAKE_REQUEST(rpc_ping, pool, ping , pong,  ping_cb, NULL);
    fprintf(stderr, "rst: %d\n", rst);



//////////////////


    event_dispatch();

}

int main(){
    event_init();
    rpc_basic_client();
}
