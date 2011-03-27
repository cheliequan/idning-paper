#include <event.h>
#include <evhttp.h>
#include <event2/rpc.h>
//#include <event2/rpc_struct.h>

#include <stdlib.h>
#include <string.h>

#include "protocol.gen.h"
#include "protocol.h"
#include "log.h"

#include "cluster.h"

#define MAX_MACHINE_CNT 256

static struct machine machines[MAX_MACHINE_CNT];
static int machine_cnt = 0;
static uint32_t cluster_version = 0;

static int mds_arr[MAX_MACHINE_CNT];
static int mds_cnt = 0;
static int osd_arr[MAX_MACHINE_CNT];
static int osd_cnt = 0;

static struct evrpc_pool *cmgr_conn_pool;
struct event_base *to_cmgr_ev_base;
struct machine self_machine;

void cluster_init();
int cluster_add(char *ip, int port, char type, int mid);
void cluster_remove(char *ip, int port);
void cluster_printf(char *hint);
void cluster_dump();
static int cluster_uuid();

static char *cluster_type_str(int type)
{
    switch (type) {
    case MACHINE_CMGR:
        return "MGR";
    case MACHINE_MDS:
        return "MDS";
    case MACHINE_OSD:
        return "OSD";
    case MACHINE_CLIENT:
        return "client";
    }
    return NULL;
}

//evrpc server side
void ping_handler(EVRPC_STRUCT(rpc_ping) * rpc, void *arg)
{
    DBG();

    struct ping *ping = rpc->request;
    struct pong *pong = rpc->reply;

    int ping_version = ping->version;
    //char * self_ip = ping->self_ip;
    char *remote_ip;            //= rpc->http_req->evcon->address;
    int remote_port;            //= rpc->http_req->evcon->address;
    evhttp_connection_get_peer(rpc->http_req->evcon, &remote_ip, &remote_port);
    remote_port = ping->self_port;
    int type = ping->self_type;
    int mid = ping->mid;
    logging(LOG_INFO, "ping_handler(ip=%s, port=%d, type=%s)", remote_ip,
            remote_port, cluster_type_str(type));

    if (ping->mid == 0) {
        mid = cluster_uuid();
        EVTAG_ASSIGN(pong, mid, mid);
    }

    cluster_add(remote_ip, remote_port, type, mid);

    if (cluster_version > ping_version) {   // new machine added to cluster
        EVTAG_ASSIGN(pong, version, cluster_version);
        int i;
        for (i = 0; i < machine_cnt; i++) {
            EVTAG_ARRAY_ADD(pong, machines);    // alloc space for machines
            pong_machines_assign(pong, i, machines + i);
        }
    } else {
        EVTAG_ASSIGN(pong, version, ping_version);  //version not change!
    }
    EVRPC_REQUEST_DONE(rpc);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//evrpc client side
void ping_cb(struct evrpc_status *status, struct ping *ping, struct pong *pong,
             void *arg)
{
    event_base_loopexit(to_cmgr_ev_base, NULL);
}

int ping_send_request()
{
    DBG();
    struct ping *ping = ping_new();
    struct pong *pong = pong_new();

    EVTAG_ASSIGN(ping, version, cluster_version);
    EVTAG_ASSIGN(ping, self_ip, self_machine.ip);
    EVTAG_ASSIGN(ping, self_port, self_machine.port);
    EVTAG_ASSIGN(ping, self_type, self_machine.type);
    EVTAG_ASSIGN(ping, mid, self_machine.mid);

    EVRPC_MAKE_REQUEST(rpc_ping, cmgr_conn_pool, ping, pong, ping_cb, NULL);
    event_base_dispatch(to_cmgr_ev_base);

    int pong_version;
    int pong_mid;
    EVTAG_GET(pong, version, &pong_version);
    EVTAG_GET(pong, mid, &pong_mid);

    printf("get pong version is : %d \n", pong_version);
    if (pong_version == cluster_version) {
        logging(LOG_INFO, "cluster not change!");
        goto done;
    }
    logging(LOG_INFO, "cluster changed!");
    int cnt = EVTAG_ARRAY_LEN(pong, machines);
    int i;
    machine_cnt = cnt;
    cluster_version = pong->version;
    for (i = 0; i < cnt; i++) {
        struct machine *m;
        EVTAG_ARRAY_GET(pong, machines, i, &m);

        if (machines[i].ip)
            free(machines[i].ip);
        machines[i].ip = strdup(m->ip);
        machines[i].port = m->port;
        machines[i].type = m->type;
        machines[i].mid = m->mid;
    }
    cluster_printf("after pong::");
    cluster_dump();

  done:
    ping_free(ping);
    pong_free(pong);

    return pong_mid;
}

void rpc_client_setup(char *self_host, int self_port, int self_type)
{
    struct evhttp_connection *evcon;

    to_cmgr_ev_base = event_base_new();
    cmgr_conn_pool = evrpc_pool_new(to_cmgr_ev_base);

    self_machine.ip = strdup(self_host);
    self_machine.port = self_port;
    self_machine.type = self_type;

    char *host = cfg_getstr("CMGR_HOST", "127.0.0.1");
    int port = cfg_getint32("CMGR_PORT", 9527);
    int i;
    for (i = 0; i < 2; i++) {   // 2 connections
        evcon = evhttp_connection_new(host, port);
        evrpc_pool_add_connection(cmgr_conn_pool, evcon);
    }

    int cluster_mid = cfg_getint32("CLUSTER_MID", 0);
    int new_mid = ping_send_request();
    if (cluster_mid == 0) {
        char tmp[32];
        sprintf(tmp, "CLUSTER_MID = %d", new_mid);
        cfg_append(tmp);
    }
    self_machine.mid = new_mid;
}

void cluster_printf(char *hint)
{
    char tmp[36000];
    char *p = tmp;
    int i;

    for (i = 0; i < machine_cnt; i++) {
        sprintf(p, "%s\t:%s:%d\t\t(%d)\n", cluster_type_str(machines[i].type)
                , machines[i].ip, machines[i].port, machines[i].mid);
        while (*p)
            p++;
    }
    logging(LOG_DEUBG, " %s clusters: v%d \n--------------\n%s\n------------\n",
            hint, cluster_version, tmp);
}

void cluster_init()
{
    //pong_p = pong_new();
    cluster_dump();
}

struct machine *cluster_get_machine_of_type(int type)
{
    logging(LOG_DEUBG, "cluster_get_machine_of_type(%d)", type);
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].type == type)
            return machines + i;
    }
    return NULL;
}

struct machine *cluster_get_machine_by_mid(int mid)
{
    logging(LOG_DEUBG, "cluster_get_machine_by_mid(%d)", mid);
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].mid == mid)
            return machines + i;
    }
    logging(LOG_INFO, "cluster_get_machine_by_mid(%d) return NULL!!", mid);
    return NULL;
}

int select_osd()
{
    static int i = 0;
    i = (i + 1) % osd_cnt;
    return osd_arr[i];
}

static int cluster_lookup(int mid)
{
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].mid == mid) //already in array
            return 1;
    }
    return 0;
}

static int cluster_uuid()
{
    int t = cluster_version;
    while (cluster_lookup(t))
        t++;
    return t;
}

int cluster_add(char *ip, int port, char type, int mid)
{
    DBG();
    cluster_printf("before cluster_add");
    int i;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].port == port && (0 == strcmp((char *)machines[i].ip, ip)))  //already in array
            return 0;
    }
    logging(LOG_DEUBG, "add machine %s:%d @ %d\n", ip, port, machine_cnt);

    machines[machine_cnt].ip = strdup(ip);
    machines[machine_cnt].port = port;
    machines[machine_cnt].type = type;
    machines[machine_cnt].mid = mid;
    machine_cnt++;
    cluster_printf("after cluster_add");
    //logging(LOG_DEUBG, "current machine_cnt : %d", machine_cnt);

    /*cluster_machine *m = cluster_machine_new(); */
    /*m -> ip = ip; */
    /*m -> port = port; */
    /*m -> type = type; */
    /*dlist_insert_tail(cluster_head, m); */
    cluster_version++;
    cluster_dump();
    return 1;
}

void cluster_remove(char *ip, int port)
{
    /*dlist_insert_tail(head, entries+i ); */

    cluster_version++;
    cluster_dump();
}

void cluster_dump()
{
    int i;
    mds_cnt = 0;
    osd_cnt = 0;
    for (i = 0; i < machine_cnt; i++) {
        if (machines[i].type == MACHINE_MDS)
            mds_arr[mds_cnt++] = machines[i].mid;
        else if (machines[i].type == MACHINE_OSD)
            osd_arr[osd_cnt++] = machines[i].mid;
    }

    /*pong_p -> machine_arrlength = machine_cnt; */
    /*pong_p -> machine_arr = machines; */

    /*pong_buffer_len = pong_pack(pong_p, pong_buffer, 0); */

    /*pong_buffer[pong_buffer_len] = 0; */
}
