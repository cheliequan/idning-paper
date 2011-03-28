#include "sfs_common.h"
#include "mds_conn.h"
#include "attr_cache.h"

ConnectionPool *conn_pool = NULL;

typedef void (*marshal_func) (struct evbuffer *, void *);
typedef int (*unmarshal_func) (void *, struct evbuffer *);

static void general_req(char *ip, int port, const char *rpcname,
                        void *req, marshal_func req_marshal,
                        void *resp, unmarshal_func resp_unmarshal);

static void rpc_request_gen_cb(struct evhttp_request *req, void *arg);
static void file_stat_copy(struct file_stat *dst, struct file_stat *src);

///////////////////
int setattr_send_request(struct file_stat *stat_arr)
{
    DBG();
    int ret = 0;
    struct setattr_request *req = setattr_request_new();
    struct setattr_response *response = setattr_response_new();

    struct file_stat *t = EVTAG_ARRAY_ADD(req, stat_arr);
    EVTAG_ASSIGN(t, ino, stat_arr->ino);
    EVTAG_ASSIGN(t, size, stat_arr->size);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_setattr",
                req, (marshal_func) setattr_request_marshal,
                response, (unmarshal_func) setattr_response_unmarshal);

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    if (cnt != 1) {
        logging(LOG_ERROR, "setattr_send_request return cnt != 1, cnt= %d",
                cnt);
        ret = -1;
        goto done;
    }
  done:
    setattr_request_free(req);
    setattr_response_free(response);
    return ret;

}

int stat_send_request_to(uint64_t * ino_arr, int len, struct file_stat *stat_arr, char * ip, int port){
    DBG();
    int ret = 0;
    struct stat_request *req = stat_request_new();
    struct stat_response *response = stat_response_new();
    int i;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino_arr[i]);
    }

    general_req(ip, port, "/.rpc.rpc_stat",
                req, (marshal_func) stat_request_marshal,
                response, (unmarshal_func) stat_response_unmarshal);

    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);
    if (cnt != len) {
        ret = -1;
        goto done;
    }
    struct file_stat *stat;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        if (stat->ino == 0){
            ret = -1;
            goto done;
        }
        file_stat_copy(stat_arr + i, stat);
    }
  done:
    stat_request_free(req);
    stat_response_free(response);
    return ret;
}
int stat_send_request(uint64_t * ino_arr, int len, struct file_stat *stat_arr)
{
    // it MUST be int the attr_cache
    struct file_stat * cached = attr_cache_lookup(ino_arr[0]);
    file_stat_copy(stat_arr, cached);
    return 0;

}

//seams same as stat_send_request
int ls_send_request(uint64_t ino, struct file_stat **o_stat_arr, int *o_cnt)
{
    DBG();
    struct ls_request *req = ls_request_new();
    struct ls_response *response = ls_response_new();

    EVTAG_ARRAY_ADD_VALUE(req, ino_arr, ino);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_ls",
                req, (marshal_func) ls_request_marshal,
                response, (unmarshal_func) ls_response_unmarshal);
    int cnt = EVTAG_ARRAY_LEN(response, stat_arr);

    int i;
    struct file_stat *stat_arr = calloc(sizeof(struct file_stat), cnt);

    struct file_stat *stat = file_stat_new();

    for (i = 0; i < cnt; i++) {
        EVTAG_ARRAY_GET(response, stat_arr, i, &stat);
        file_stat_copy(stat_arr + i, stat);
    }
    ls_request_free(req);
    ls_response_free(response);

    *o_stat_arr = stat_arr;
    *o_cnt = cnt;

    return cnt;
}

int mknod_send_request(uint64_t parent_ino, const char *name, int type,
                       int mode, struct file_stat *o_stat)
{
    DBG();
    struct mknod_request *req = mknod_request_new();

    struct mknod_response *response = mknod_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);
    EVTAG_ASSIGN(req, type, type);
    EVTAG_ASSIGN(req, mode, mode);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_mknod",
                req, (marshal_func) mknod_request_marshal,
                response, (unmarshal_func) mknod_response_unmarshal);

    struct file_stat *stat;
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    file_stat_copy(o_stat, stat);

    mknod_request_free(req);
    mknod_response_free(response);
    return 0;
}

static void file_stat_copy(struct file_stat *dst, struct file_stat *src)
{
    dst->size = src->size;
    dst->ino = src->ino;
    dst->type = src->type;
    dst->mode = src->mode;
    if (src->name)
        dst->name = strdup(src->name);  //FIXME : free me!

    int len = EVTAG_ARRAY_LEN(src, pos_arr);
    int i = 0, pos;
    for (i = 0; i < len; i++) {
        EVTAG_ARRAY_GET(src, pos_arr, i, &pos);
        EVTAG_ARRAY_ADD_VALUE(dst, pos_arr, pos);
    }
}

int lookup_send_request(uint64_t parent_ino, const char *name,
                        struct file_stat *o_stat)
{
    DBG();
    struct lookup_request *req = lookup_request_new();

    struct lookup_response *response = lookup_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_lookup",
                req, (marshal_func) lookup_request_marshal,
                response, (unmarshal_func) lookup_response_unmarshal);

    struct file_stat *stat;
    EVTAG_ARRAY_GET(response, stat_arr, 0, &stat);
    o_stat->size = stat->size;
    o_stat->ino = stat->ino;

    lookup_request_free(req);
    lookup_response_free(response);

    return 0;
}

int unlink_send_request(uint64_t parent_ino, const char *name)
{
    DBG();
    struct unlink_request *req = unlink_request_new();
    struct unlink_response *response = unlink_response_new();
    EVTAG_ASSIGN(req, parent_ino, parent_ino);
    EVTAG_ASSIGN(req, name, name);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_unlink",
                req, (marshal_func) unlink_request_marshal,
                response, (unmarshal_func) unlink_response_unmarshal);
    unlink_request_free(req);
    unlink_response_free(response);
    return 0;
}

int statfs_send_request(int *total_space, int *avail_space, int *inode_cnt)
{
    DBG();
    struct statfs_request *req = statfs_request_new();
    struct statfs_response *response = statfs_response_new();

    EVTAG_ASSIGN(req, nothing, 1);

    general_req("127.0.0.1", 9528, "/.rpc.rpc_statfs",
                req, (marshal_func) statfs_request_marshal,
                response, (unmarshal_func) statfs_response_unmarshal);

    EVTAG_GET(response, total_space, total_space);
    EVTAG_GET(response, avail_space, avail_space);
    EVTAG_GET(response, inode_cnt, inode_cnt);
    statfs_request_free(req);
    statfs_response_free(response);
    return 0;
}

int mkfs_send_request(int mds1, int mds2)
{
   DBG();
   struct mkfs_request *req = mkfs_request_new();
   struct mkfs_response *response = mkfs_response_new();


   EVTAG_ARRAY_ADD_VALUE(req, pos_arr, mds1);
   EVTAG_ARRAY_ADD_VALUE(req, pos_arr, mds2);

   struct machine * m = cluster_get_machine_by_mid(mds1);

   general_req(m->ip, m->port, "/.rpc.rpc_mkfs",
               req, (marshal_func) mkfs_request_marshal,
               response, (unmarshal_func) mkfs_response_unmarshal);

   m = cluster_get_machine_by_mid(mds2);

   general_req(m->ip, m->port, "/.rpc.rpc_mkfs",
               req, (marshal_func) mkfs_request_marshal,
               response, (unmarshal_func) mkfs_response_unmarshal);

   mkfs_request_free(req);
   mkfs_response_free(response);
    return 0;
}

static void general_req(char *ip, int port, const char *rpcname,
                        void *req, marshal_func req_marshal,
                        void *resp, unmarshal_func resp_unmarshal)
{
    struct evhttp_connection *evcon =
        connection_pool_get_or_create_conn(conn_pool, ip, port);
    struct evhttp_request *evreq = evhttp_request_new(rpc_request_gen_cb, NULL);
    evhttp_request_own(evreq);  // this means that I should free it my self

    req_marshal(evreq->output_buffer, req);
    if (evhttp_make_request(evcon, evreq, EVHTTP_REQ_POST, rpcname))
        logging(LOG_ERROR, "error on make_request");

    event_dispatch();
    if (resp_unmarshal(resp, evreq->input_buffer)) {
        logging(LOG_ERROR, "error on statfs_response_unmarshal");
    }
    connection_pool_insert(conn_pool, ip, port, evcon);
    evhttp_request_free(evreq);
}

static void rpc_request_gen_cb(struct evhttp_request *req, void *arg)
{
    if (req->response_code != HTTP_OK) {
        fprintf(stderr, "FAILED (response code)\n");
        exit(1);
    }
    event_loopexit(NULL);
}

void mds_conn_init()
{
    DBG();
    event_init();
    rpc_client_setup("client", 0, MACHINE_CLIENT);

    //struct machine *mds = cluster_get_machine_of_type(MACHINE_MDS);

    conn_pool = connection_pool_new();

}
