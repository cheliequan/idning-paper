#include <event.h>
#include <evhttp.h>
#include <stdlib.h>
#include "protocol.gen.h"
#include "protocol.h"


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

    event_loopexit(NULL);
}

static void stat_cb(struct evrpc_status *status,
    struct stat_request *request , struct stat_response * response , void *arg)
{
    fprintf(stderr, "%s: called\n", __func__);
    int i;
    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    for (i=0; i< cnt; i++){
        fprintf(stderr, "i : %d\n", i);
        /*struct file_stat * s;*/
        /*EVTAG_ARRAY_GET(pong, machines, 0, &m);*/
        /*printf("machine %d: \n", i);*/
        
        /*printf("m->port : %d \n", m->port);*/
        /*printf("m->ip : %s \n", m->ip);*/
    }

    event_loopexit(NULL);
}

struct evrpc_pool *pool = NULL;

int stat_send_request(int * ino_arr, int len, struct file_stat * stat_arr)
{
    struct stat_request * req = stat_request_new();
    struct stat_response * response = stat_response_new();
    int i;
    for(i=0; i<len; i++){
        int * t = EVTAG_ARRAY_ADD(response, stat_arr);
        *t = ino_arr[i];
    }

    int rst = EVRPC_MAKE_REQUEST(rpc_stat, pool, req, response,  stat_cb, NULL);
    fprintf(stderr, "rst: %d\n", rst);
    event_dispatch();

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    fprintf(stderr, "after dispatch cnt: %d\n", cnt);
    if (cnt!=len)
        return -1;
    for (i=0; i< len; i++){
        EVTAG_ARRAY_GET(response, stat_arr, i, stat_arr + i);
        fprintf(stderr, "stat_arr[%d].size: ", i, stat_arr[i].size);
        /*printf("machine %d: \n", i);*/
        
        /*printf("m->port : %d \n", m->port);*/
        /*printf("m->ip : %s \n", m->ip);*/
    }
    return 0;
}

int ping_send_request(void)
{
    struct ping * ping = ping_new();
    EVTAG_ASSIGN(ping, version, 7);
    EVTAG_ASSIGN(ping, self_ip, "127.0.0.2");
    EVTAG_ASSIGN(ping, self_port, 8080);
    struct pong * pong =  pong_new();

    int rst = EVRPC_MAKE_REQUEST(rpc_ping, pool, ping , pong,  ping_cb, NULL);
    fprintf(stderr, "rst: %d\n", rst);
    event_dispatch();

    int v;
    EVTAG_GET(pong, version, &v);
    fprintf(stderr, "after event_dispatch: version is : %d\n", v);
}


void mds_conn_init(){
    event_init();
    ev_uint16_t port = 9527;
    struct evhttp_connection *evcon;

    pool = evrpc_pool_new(NULL); 

    evcon = evhttp_connection_new("127.0.0.1", port);
    fprintf(stderr, "evcon : %p\n", evcon);

    evrpc_pool_add_connection(pool, evcon);
}

/*int main(){*/
    /*event_init();*/
    /*rpc_basic_client();*/
    /*doit();*/
/*}*/

