#include <event.h>
#include <evhttp.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.gen.h"
#include "protocol.h"
/*#include "fs.h"*/


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
    if (cnt == 0)
        return;
    fprintf(stderr, "msg->stat_arr_set : %d\n", response->stat_arr_set);
    fprintf(stderr, "msg->: %d\n", response->stat_arr[0] -> ino);
    fprintf(stderr, "msg->: %d\n", response->stat_arr[0] -> size);

    for (i=0; i< cnt; i++){
        struct file_stat * stat = file_stat_new();
        fprintf(stderr, "stat_arr[%d].ino : %d\n", i, stat->ino);
        fprintf(stderr, "stat_arr[%d].size: %d\n", i, stat->size);

        if (0 != EVTAG_ARRAY_GET(response, stat_arr, i, &stat))
            fprintf(stderr, "something error on EVTAG_ARRAY_GET");

        fprintf(stderr, "stat_arr[%d].ino : %d\n", i, stat->ino);
        fprintf(stderr, "stat_arr[%d].size: %d\n", i, stat->size);
    }
    event_loopexit(NULL);
}

static void ls_cb(struct evrpc_status *status,
    struct ls_request *request , struct ls_response * response , void *arg){

    fprintf(stderr, "%s: called\n", __func__);
    event_loopexit(NULL);
}

struct evrpc_pool *pool = NULL;

int stat_send_request(int * ino_arr, int len, struct file_stat * stat_arr)
{
    fprintf(stderr, "%s: called\n", __func__);
    struct stat_request * req = stat_request_new();
    struct stat_response * response = stat_response_new();
    int i;
    for(i=0; i<len; i++){
        EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino_arr[i]);
        fprintf(stderr, "add value: %d", ino_arr[i]);
    }

    int rst = EVRPC_MAKE_REQUEST(rpc_stat, pool, req, response,  stat_cb, NULL);
    fprintf(stderr, "rst: %d\n", rst);
    event_dispatch();

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    fprintf(stderr, "after dispatch cnt: %d\n", cnt);
    if (cnt!=len)
        return -1;
    for (i=0; i< len; i++){
        struct file_stat * stat = stat_arr +i;
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        fprintf(stderr, "stat_arr[%d].ino : %d\n", i, stat->ino);
        fprintf(stderr, "stat_arr[%d].size: %d\n", i, stat->size);

        fprintf(stderr, "stat_arr+i: %p", stat_arr+i);
        fprintf(stderr, "stat : %p", stat);

        stat_arr[i].size = stat-> size;
        stat_arr[i].ino = stat-> ino;
    }
    return 0;
}


//seams same as stat_send_request
int ls_send_request(int ino, struct file_stat * stat_arr)
{
    fprintf(stderr, "%s: called\n", __func__);
    struct ls_request * req = ls_request_new();
    struct ls_response * response = ls_response_new();

    EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino);

    EVRPC_MAKE_REQUEST(rpc_ls, pool, req, response,  ls_cb, NULL);
    event_dispatch();

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    fprintf(stderr, "after dispatch cnt: %d\n", cnt);
    /*struct file_stat ** arr = malloc(cnt*sizeof(struct file_stat *));*/

    int i;
    struct file_stat * stat = file_stat_new();
    for (i=0; i< cnt; i++){
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        stat_arr[i].size = stat-> size;
        stat_arr[i].ino = stat-> ino;
        stat_arr[i].name = strdup(stat-> name);
        stat_arr[i].type = stat-> type;
    }
    return cnt;
}

int ping_send_request(void)
{
    fprintf(stderr, "%s: called\n", __func__);
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
    return 0;
}


void mds_conn_init(){
    fprintf(stderr, "%s: called\n", __func__);
    event_init();
    ev_uint16_t port = 9527;
    struct evhttp_connection *evcon;

    pool = evrpc_pool_new(NULL); 

    evcon = evhttp_connection_new("127.0.0.1", port);
    /*evcon = evhttp_connection_new("192.168.1.102", port);*/
    fprintf(stderr, "evcon : %p\n", evcon);

    evrpc_pool_add_connection(pool, evcon);
}

/*int main(){*/
    /*event_init();*/
    /*rpc_basic_client();*/
    /*doit();*/
/*}*/

