#include <event.h>
#include <evhttp.h>
#include <event2/rpc.h>
//#include <event2/rpc_struct.h>


#include <stdlib.h>
#include <string.h>

#include "protocol.gen.h"
#include "protocol.h"
#include "log.h"


#define MAX_MACHINE_CNT 256

static struct machine machines [MAX_MACHINE_CNT];
static int machine_cnt = 0;
static uint64_t cluster_version = 0;

void cluster_init();
int cluster_add(char * ip, int port, char type);
void cluster_remove(char * ip, int port);
void cluster_dump();
//evrpc server side
void
ping_handler(EVRPC_STRUCT(rpc_ping)* rpc, void *arg)
{
    DBG();

    struct ping * ping = rpc->request;
    struct pong * pong = rpc->reply;

    int ping_version = ping->version;
    //char * self_ip = ping->self_ip;
    char * remote_ip ;//= rpc->http_req->evcon->address;
    int remote_port ;//= rpc->http_req->evcon->address;
    evhttp_connection_get_peer(rpc->http_req->evcon, &remote_ip, &remote_port);
    remote_port = ping->self_port;
    int type = ping->self_type;

    cluster_add(remote_ip, remote_port, type);
    if (cluster_version > ping_version ){// new machine added to cluster
        EVTAG_ASSIGN(pong, version, cluster_version);
        int i;
        for(i=0; i < machine_cnt; i++){
            EVTAG_ARRAY_ADD(pong, machines);  // alloc space for machines
            pong_machines_assign(pong, i, machines+i);
        }
    }else{
        EVTAG_ASSIGN(pong, version, ping_version); //version not change!
    }
    EVRPC_REQUEST_DONE(rpc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//evrpc client side
void ping_cb(struct evrpc_status *status, struct ping *ping , struct pong * pong, void *arg)
{
    event_loopexit(NULL);
}



int ping_send_request(struct evrpc_pool * pool, const char * self_ip,int self_port, int self_type)
{
    DBG();
    struct ping * ping = ping_new();
    struct pong * pong =  pong_new();

    EVTAG_ASSIGN(ping, version, cluster_version);
    EVTAG_ASSIGN(ping, self_ip, self_ip);
    EVTAG_ASSIGN(ping, self_port, self_port);
    EVTAG_ASSIGN(ping, self_type, self_type);

    EVRPC_MAKE_REQUEST(rpc_ping, pool, ping , pong,  ping_cb, NULL);
    event_dispatch();

    int pong_version;
    EVTAG_GET(pong, version, &pong_version);

    printf("get pong version is : %d \n", pong_version);
    if (pong_version == cluster_version){
        logging(LOG_INFO, "cluster not change!");
        return 0;
    }
    logging(LOG_INFO, "cluster changed!");
    int cnt = EVTAG_ARRAY_LEN(pong, machines);
    int i;
    machine_cnt = 0;
    for (i=0; i< cnt; i++){
        struct machine * m;
        EVTAG_ARRAY_GET(pong, machines, 0, &m);
        /*loggging(LOG_DEUBG, "pong: machines[x].type")*/
        cluster_add(m->ip, m->port, m->type);
    }
    cluster_printf("after pong::");
    return 0;
}


void cluster_printf(char * hint){
    char tmp[36000];
    char * p = tmp;
    int i;
    for (i=0; i< machine_cnt; i++){
        sprintf(p, "%s:%d _ %d \n", machines[i].ip, machines[i].port, machines[i].type);
        while(*p)
            p++;
    }
    logging(LOG_DEUBG," %s clusters: \n--------------------------------\n%s\n----------------------------\n", hint, tmp);
}

void cluster_init(){
    //pong_p = pong_new();
    cluster_dump();
}

struct machine * cluster_get_machine_of_type(int type){
    logging(LOG_DEUBG, "cluster_get_machine_of_type(%d)", type);
    int i;
    for(i=0; i < machine_cnt; i++){
        if (machines[i].type == type) 
            return machines+i;
    }
    return NULL;
}

int cluster_add(char * ip, int port, char type){
    DBG();
    cluster_printf("before cluster_add");
    int i;
    for(i=0; i < machine_cnt; i++){
        if (machines[i].port == port && (0 == strcmp((char *)machines[i].ip, ip)) ) //already in array
            return 0;
    }
    logging(LOG_DEUBG, "add machine %s:%d @ %d\n", ip, port, machine_cnt);
    
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
    return 1;
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

