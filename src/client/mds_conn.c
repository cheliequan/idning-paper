#include "sfs_common.h"
#include "mds_conn.h"

ConnectionPool * conn_pool = NULL;

struct evrpc_pool *pool = NULL;

typedef void (*marshal_func)(struct evbuffer *, void *);
typedef int (*unmarshal_func)(void * , struct evbuffer *);

static void general_req(char * ip, int port, const char *rpcname,
    void * req, marshal_func req_marshal,
    void * resp, unmarshal_func resp_unmarshal );

static void rpc_request_gen_cb(struct evhttp_request *req, void *arg);

/*static void stat_cb(struct evrpc_status *status, struct stat_request *request , struct stat_response * response , void *arg) {*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void ls_cb(struct evrpc_status *status, struct ls_request *request , struct ls_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/
/*static void mknod_cb(struct evrpc_status *status, struct mknod_request *request , struct mknod_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void lookup_cb(struct evrpc_status *status, struct lookup_request *request , struct lookup_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void unlink_cb(struct evrpc_status *status, struct unlink_request *request , struct unlink_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void setattr_cb(struct evrpc_status *status, struct setattr_request *request , struct setattr_response * response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

/*static void statfs_cb(struct evrpc_status *status, struct statfs_request *request , struct statfs_response* response , void *arg){*/
    /*event_loopexit(NULL);*/
/*}*/

int setattr_send_request(struct file_stat * stat_arr)
{
    DBG();
    struct setattr_request * req = setattr_request_new();
    struct setattr_response * response = setattr_response_new();

    struct file_stat * t = EVTAG_ARRAY_ADD(req, stat_arr);
    EVTAG_ASSIGN(t, ino, stat_arr->ino); // 不然它不认..
    EVTAG_ASSIGN(t, size , stat_arr->size); // 不然它不认..

    general_req("127.0.0.1", 9528, "/.rpc.rpc_setattr",
            req, (marshal_func)setattr_request_marshal,
            response, (unmarshal_func)setattr_response_unmarshal
            );

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    if (cnt!=1){
        logging(LOG_ERROR, "setattr_send_request return cnt != 1, cnt= %d", cnt);
        return -1;
    }
    return 0;
}

int stat_send_request(uint64_t * ino_arr, int len, struct file_stat * stat_arr)
{
    DBG();
    struct stat_request * req = stat_request_new();
    struct stat_response * response = stat_response_new();
    int i;
    for(i=0; i<len; i++){
        EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino_arr[i]);
    }

    general_req("127.0.0.1", 9528, "/.rpc.rpc_stat",
            req, (marshal_func)stat_request_marshal,
            response, (unmarshal_func)stat_response_unmarshal
            );

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
int ls_send_request(uint64_t ino, struct file_stat * stat_arr)
{
    DBG();
    struct ls_request * req = ls_request_new();
    struct ls_response * response = ls_response_new();

    EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino);


    general_req("127.0.0.1", 9528, "/.rpc.rpc_ls",
            req, (marshal_func)ls_request_marshal,
            response, (unmarshal_func)ls_response_unmarshal
            );
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

int mknod_send_request(uint64_t parent_ino, const char * name, int type, int mode, struct file_stat *o_stat )
{
    DBG();
    struct mknod_request * req = mknod_request_new();

    struct mknod_response * response = mknod_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);
    EVTAG_ASSIGN(req, type, type);
    EVTAG_ASSIGN(req, mode, mode);


    general_req("127.0.0.1", 9528, "/.rpc.rpc_mknod",
            req, (marshal_func)mknod_request_marshal,
            response, (unmarshal_func)mknod_response_unmarshal
            );

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



int lookup_send_request(uint64_t parent_ino, const char * name , struct file_stat *o_stat )
{
    DBG();
    struct lookup_request* req = lookup_request_new();

    struct lookup_response * response = lookup_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_lookup",
            req, (marshal_func)lookup_request_marshal,
            response, (unmarshal_func)lookup_response_unmarshal
            );

    struct file_stat * stat = file_stat_new();
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    o_stat->size = stat->size;
    o_stat->ino = stat->ino;

    return 0;
}



int unlink_send_request(uint64_t parent_ino, const char * name )
{
    DBG();
    struct unlink_request* req = unlink_request_new();
    struct unlink_response * response = unlink_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);


    general_req("127.0.0.1", 9528, "/.rpc.rpc_unlink",
            req, (marshal_func)unlink_request_marshal,
            response, (unmarshal_func)unlink_response_unmarshal
            );

    return 0;
}





int statfs_send_request(int * total_space, int * avail_space, int * inode_cnt)
{
    DBG();
    struct statfs_request * req = statfs_request_new();
    struct statfs_response * response =  statfs_response_new();

    EVTAG_ASSIGN(req, nothing, 1);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_statfs",
            req, (marshal_func)statfs_request_marshal,
            response, (unmarshal_func)statfs_response_unmarshal
            );

    EVTAG_GET(response, total_space, total_space);
    EVTAG_GET(response, avail_space, avail_space);
    EVTAG_GET(response, inode_cnt, inode_cnt);
    return 0;
}

static void general_req(char * ip, int port, const char *rpcname,
    void * req, marshal_func req_marshal,
    void * resp, unmarshal_func resp_unmarshal )
{
    struct evhttp_connection *evcon = connection_pool_get_or_create_conn(conn_pool, ip, port);
    struct evhttp_request *evreq = evhttp_request_new(rpc_request_gen_cb, NULL);
    evhttp_request_own(evreq); // this means that I should free it my self

    req_marshal(evreq->output_buffer, req);
    if ( evhttp_make_request(evcon, evreq, EVHTTP_REQ_POST, rpcname))
        logging(LOG_ERROR, "error on make_request");

    event_dispatch();
    if (resp_unmarshal(resp, evreq -> input_buffer)){
        logging(LOG_ERROR, "error on statfs_response_unmarshal");
    }
    connection_pool_insert(conn_pool, ip, port, evcon);


}

static void rpc_request_gen_cb(struct evhttp_request *req, void *arg)
{
    if (req->response_code != HTTP_OK) {
        fprintf(stderr, "FAILED (response code)\n");
        exit(1);
    }

    logging(LOG_DEUBG, "evbuffer_get_length(evreq->output_buffer): %d", evbuffer_get_length(req->output_buffer));
    logging(LOG_DEUBG, "evbuffer_get_length(evreq->input_buffer): %d", evbuffer_get_length(req->input_buffer));

    event_loopexit(NULL);

    /*struct statfs_response * response =  statfs_response_new();*/
    /*if (statfs_response_unmarshal(response, req -> input_buffer)){*/
        /*logging(LOG_ERROR, " in error on statfs_response_unmarshal");*/
    /*}*/
    /*logging(LOG_DEUBG, "yy %d", response->total_space);*/
    /*logging(LOG_DEUBG, "yy %d", response->avail_space);*/
    /*logging(LOG_DEUBG, "yy %d", response->inode_cnt);*/
    


}

void mds_conn_init(){
    DBG();
    event_init();
    rpc_client_setup("client", 0, MACHINE_CLIENT);

    struct machine * mds = cluster_get_machine_of_type(MACHINE_MDS);
    pool = evrpc_pool_new(NULL);
    struct evhttp_connection *evcon = evhttp_connection_new(mds->ip, mds->port);
    evrpc_pool_add_connection(pool, evcon);

    
    conn_pool = connection_pool_new();

}

