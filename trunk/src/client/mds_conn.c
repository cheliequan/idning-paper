#include <event.h>
#include <evhttp.h>
#include <stdlib.h>
#include <string.h>
#include <fuse_lowlevel.h>


#include "protocol.gen.h"
#include "protocol.h"
#include "log.h"
#include "cluster.h"
#include "mds_conn.h"
#include "cfg.h"

struct evrpc_pool *pool = NULL;

static void stat_cb(struct evrpc_status *status, struct stat_request *request , struct stat_response * response , void *arg) {
    event_loopexit(NULL);
}

static void ls_cb(struct evrpc_status *status, struct ls_request *request , struct ls_response * response , void *arg){
    event_loopexit(NULL);
}
static void mknod_cb(struct evrpc_status *status, struct mknod_request *request , struct mknod_response * response , void *arg){
    event_loopexit(NULL);
}

static void lookup_cb(struct evrpc_status *status, struct lookup_request *request , struct lookup_response * response , void *arg){
    event_loopexit(NULL);
}

static void unlink_cb(struct evrpc_status *status, struct unlink_request *request , struct unlink_response * response , void *arg){
    event_loopexit(NULL);
}

static void setattr_cb(struct evrpc_status *status, struct setattr_request *request , struct setattr_response * response , void *arg){
    event_loopexit(NULL);
}

static void statfs_cb(struct evrpc_status *status, struct statfs_request *request , struct statfs_response* response , void *arg){
    event_loopexit(NULL);
}

int setattr_send_request(struct file_stat * stat_arr)
{
    DBG();
    struct setattr_request * req = setattr_request_new();
    struct setattr_response * response = setattr_response_new();

    struct file_stat * t = EVTAG_ARRAY_ADD(req, stat_arr);
    EVTAG_ASSIGN(t, ino, stat_arr->ino); // 不然它不认..
    EVTAG_ASSIGN(t, size , stat_arr->size); // 不然它不认..

    EVRPC_MAKE_REQUEST(rpc_setattr, pool, req, response,  setattr_cb, NULL);
    event_dispatch();
    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    if (cnt!=1){
        logging(LOG_ERROR, "setattr_send_request return cnt != 1, cnt= %d", cnt);
        return -1;
    }
    return 0;
}

int stat_send_request(fuse_ino_t * ino_arr, int len, struct file_stat * stat_arr)
{
    DBG();
    struct stat_request * req = stat_request_new();
    struct stat_response * response = stat_response_new();
    int i;
    for(i=0; i<len; i++){
        EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino_arr[i]);
    }

    EVRPC_MAKE_REQUEST(rpc_stat, pool, req, response,  stat_cb, NULL);
    event_dispatch();
    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    if (cnt!=len)
        return -1;
    for (i=0; i< len; i++){
        struct file_stat * stat = stat_arr +i;
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        stat_arr[i].size = stat-> size;
        stat_arr[i].ino = stat-> ino;
        stat_arr[i].type = stat-> type;
        stat_arr[i].mode = stat-> mode;

        stat_arr[i].pos_arr = stat-> pos_arr; //  FIXME, 如果后面free response，这就会出错.

    }
    return 0;
}


//seams same as stat_send_request
int ls_send_request(fuse_ino_t ino, struct file_stat * stat_arr)
{
    DBG();
    struct ls_request * req = ls_request_new();
    struct ls_response * response = ls_response_new();

    EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino);

    EVRPC_MAKE_REQUEST(rpc_ls, pool, req, response,  ls_cb, NULL);
    event_dispatch();

    int i;
    struct file_stat * stat = file_stat_new();
    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    for (i=0; i< cnt; i++){
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        stat_arr[i].size = stat-> size;
        stat_arr[i].ino = stat-> ino;
        stat_arr[i].name = strdup(stat-> name);
        stat_arr[i].type = stat-> type;
        stat_arr[i].mode = stat-> mode;
    }
    return cnt;
}

int mknod_send_request(fuse_ino_t parent_ino, const char * name, int type, int mode, struct file_stat *o_stat )
{
    DBG();
    struct mknod_request * req = mknod_request_new();

    struct mknod_response * response = mknod_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);
    EVTAG_ASSIGN(req, type, type);
    EVTAG_ASSIGN(req, mode, mode);


    EVRPC_MAKE_REQUEST(rpc_mknod, pool, req, response,  mknod_cb, NULL);
    event_dispatch();

    struct file_stat * stat = file_stat_new();
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    o_stat->size = stat->size;
    o_stat->ino = stat->ino;

    o_stat->pos_arr = stat-> pos_arr; //  FIXME, 如果后面free response，这就会出错.

    int pos1 = o_stat->pos_arr[0];
    int pos2 = o_stat->pos_arr[1];
    logging(LOG_DEUBG, "pos: [%d, %d]", pos1, pos2);

    EVTAG_ARRAY_GET(response, stat_arr, 0, &o_stat);
    return 0;
}



int lookup_send_request(fuse_ino_t parent_ino, const char * name , struct file_stat *o_stat )
{
    DBG();
    struct lookup_request* req = lookup_request_new();

    struct lookup_response * response = lookup_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    EVRPC_MAKE_REQUEST(rpc_lookup, pool, req, response,  lookup_cb, NULL);
    event_dispatch();

    struct file_stat * stat = file_stat_new();
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    o_stat->size = stat->size;
    o_stat->ino = stat->ino;

    return 0;
}



int unlink_send_request(fuse_ino_t parent_ino, const char * name )
{
    DBG();
    struct unlink_request* req = unlink_request_new();

    struct unlink_response * response = unlink_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    EVRPC_MAKE_REQUEST(rpc_unlink, pool, req, response,  unlink_cb, NULL);
    event_dispatch();

    return 0;
}





int statfs_send_request(int * total_space, int * avail_space, int * inode_cnt)
{
    DBG();
    struct statfs_request * req = statfs_request_new();
    struct statfs_response * response =  statfs_response_new();

    EVTAG_ASSIGN(req, nothing, 1);

    EVRPC_MAKE_REQUEST(rpc_statfs, pool, req, response,  statfs_cb, NULL);

    event_dispatch();

    EVTAG_GET(response, total_space, total_space);
    EVTAG_GET(response, avail_space, avail_space);
    EVTAG_GET(response, inode_cnt, inode_cnt);

    return 0;
}


void mds_conn_init(){
    DBG();
    event_init();
    rpc_client_setup("client", 0, MACHINE_CLIENT);


    struct machine * mds = cluster_get_machine_of_type(MACHINE_MDS);
    pool = evrpc_pool_new(NULL);
    struct evhttp_connection *evcon = evhttp_connection_new(mds->ip, mds->port);
    evrpc_pool_add_connection(pool, evcon);

}

