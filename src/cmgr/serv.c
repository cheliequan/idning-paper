#include <event.h>
#include <evhttp.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.gen.h"
#include "log.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_ping, ping, pong)

#define MAX_MACHINE_CNT 256

static struct machine machines [MAX_MACHINE_CNT];
static int machine_cnt = 0;
static uint64_t cluster_version = 0;

void cluster_init();
void cluster_add(char * ip, int port, char type);
void cluster_remove(char * ip, int port);
void cluster_dump();


static void
ping_handler(EVRPC_STRUCT(rpc_ping)* rpc, void *arg)
{
    fprintf(stderr, "%s: called\n", __func__);

    struct ping * ping = rpc->request;
    struct pong * pong = rpc->reply;

#define EV_GET(msg, member) \
        (msg->member)

    int ping_version = ping->version;
    


    fprintf(stderr, "ping->self_ip :%s\n", ping->self_ip);
    fprintf(stderr, "ping->self_port :%d\n", ping->self_port);
    cluster_add((char *)ping->self_ip, ping->self_port, 0);
    fprintf(stderr, "machine_cnt:%d\n", machine_cnt);
    
    EVTAG_ASSIGN(pong, version, ping_version+1);
    EVTAG_ASSIGN(pong, xx, 8);
    /*EVTAG_ASSIGN(pong, machines, 8, machines);*/

    int i;
    for(i=0; i < machine_cnt; i++){
        struct machine * m = EVTAG_ARRAY_ADD(pong, machines);  // alloc space for machines
        printf("before assign m->ip  : %s   m[i].ip: %s\n", m->ip, machines[i].ip);
        pong_machines_assign(pong, i, machines+i);
        printf("after assign m->ip  : %s   m[i].ip: %s\n", m->ip, machines[i].ip);
    }
    EVRPC_REQUEST_DONE(rpc);
}

static void
rpc_setup(struct evhttp **phttp, ev_uint16_t *pport, struct evrpc_base **pbase)
{
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    int port = 9527;
    http = evhttp_start("0.0.0.0", port);
    if (!http){
        perror("can't start server!");
        exit(-1);
    }


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
    event_init();
    rpc_setup(&http, &port, &base);
    event_dispatch();
    return 0;
}

void cluster_printf(char * hint){
    char tmp[36000];
    char * p = tmp;
    int i;
    for (i=0; i< machine_cnt; i++){
        sprintf(p, "%s:%d\n", machines[i].ip, machines[i].port);
        while(*p)
            p++;
    }
    printf(" %s clusters: \n----------------------------------------------\n%s\n---------------------------------------\n", hint, tmp);
}

void cluster_init(){
    //pong_p = pong_new();
    cluster_dump();
}

void cluster_add(char * ip, int port, char type){
    //logging(LOG_DEUBG, "%s: called\n", __func__);
    cluster_printf("before cluster_add");
    int i;
    for(i=0; i < machine_cnt; i++){
        if (machines[i].port == port && (0 == strcmp((char *)machines[i].ip, ip)) ) //already in array
            return;
    }
    fprintf(stderr, "add machine %s:%d @ %d\n", ip, port, machine_cnt);
    
    machines[machine_cnt].ip = strdup(ip);
    machines[machine_cnt].port = port;
    machines[machine_cnt].type = type;
    machine_cnt ++;
    cluster_printf("after cluster_add");
    //logging(LOG_DEUBG, "current machine_cnt : %d", machine_cnt);

    /*cluster_machine *m = cluster_machine_new();*/
    /*m -> ip = ip;*/
    /*m -> port = port;*/
    /*m -> type = type;*/
    /*dlist_insert_tail(cluster_head, m);*/
    cluster_version ++ ;
    cluster_dump();
}

void cluster_remove(char * ip, int port){
    /*dlist_insert_tail(head, entries+i );*/

    cluster_version ++ ;
    cluster_dump();
}

void cluster_dump(){
    /*pong_p -> machine_arrlength = machine_cnt;*/
    /*pong_p -> machine_arr = machines;*/

    /*pong_buffer_len = pong_pack(pong_p, pong_buffer, 0);*/

    /*pong_buffer[pong_buffer_len] = 0;*/
}



