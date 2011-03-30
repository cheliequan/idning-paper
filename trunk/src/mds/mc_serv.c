#include "sfs_common.h"
#include "fs.h"
static void fsnode_to_stat_copy(struct file_stat * t, fsnode * n);

static void setattr_handler(EVRPC_STRUCT(rpc_setattr) * rpc, void *arg)
{
    DBG();
    struct setattr_request *request = rpc->request;
    struct setattr_response *response = rpc->reply;

    struct file_stat *stat;
    EVTAG_ARRAY_GET(request, stat_arr, 0, &stat);

    logging(LOG_DEUBG,
            "setattr(%" PRIu64 ", name=%s, size=%" PRIu64 ", mode=%04o)",
            stat->ino, stat->name, stat->size, stat->mode);
    fs_setattr(stat->ino, stat);

    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
    EVTAG_ASSIGN(t, ino, stat->ino);
    EVTAG_ASSIGN(t, size, stat->size);

    EVRPC_REQUEST_DONE(rpc);
}

static void stat_handler(EVRPC_STRUCT(rpc_stat) * rpc, void *arg)
{
    DBG();
    struct stat_request *request = rpc->request;
    struct stat_response *response = rpc->reply;

    int cnt = EVTAG_ARRAY_LEN(request, ino_arr);
    int i;
    for (i = 0; i < cnt; i++) {
        uint64_t ino;
        EVTAG_ARRAY_GET(request, ino_arr, i, &ino);
        logging(LOG_DEUBG, "stat(%" PRIu64 ")", ino);

        struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
        //TODO;
        
        fsnode *n = fsnode_hash_find(ino);
        fsnode_to_stat_copy(t, n);
        log_file_stat("stat return : ", t);
    }
    EVRPC_REQUEST_DONE(rpc);
}

static void ls_handler(EVRPC_STRUCT(rpc_stat) * rpc, void *arg)
{
    DBG();
    struct stat_request *request = rpc->request;
    struct stat_response *response = rpc->reply;

    uint64_t ino;
    EVTAG_ARRAY_GET(request, ino_arr, 0, &ino);
    logging(LOG_DEUBG, "ls(%" PRIu64 ")", ino);

    fsnode *p = fsnode_hash_find(ino);
    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);

    EVTAG_ASSIGN(t, ino, p->ino);
    EVTAG_ASSIGN(t, size, 4096);
    EVTAG_ASSIGN(t, name, ".");
    EVTAG_ASSIGN(t, type, p->type);
    EVTAG_ASSIGN(t, mode, p->mode);

    p = p->parent;

    t = EVTAG_ARRAY_ADD(response, stat_arr);
    EVTAG_ASSIGN(t, ino, p->ino);
    EVTAG_ASSIGN(t, size, 4096);
    EVTAG_ASSIGN(t, name, "..");
    EVTAG_ASSIGN(t, type, p->type);
    EVTAG_ASSIGN(t, mode, p->mode);

    fsnode *n = fs_ls(ino);
    if (n != NULL) {
        dlist_t *head = &(n->tree_dlist);
        dlist_t *pl;
        for (pl = head->next; pl != head; pl = pl->next) {

            p = dlist_data(pl, fsnode, tree_dlist);
            t = EVTAG_ARRAY_ADD(response, stat_arr);
            fsnode_to_stat_copy(t, p);
            log_file_stat("ls return : ", t);
        }
    }
    EVRPC_REQUEST_DONE(rpc);
}

static void statfs_handler(EVRPC_STRUCT(rpc_statfs) * rpc, void *arg)
{
    DBG();

    struct statfs_response *response = rpc->reply;
    int avail_space;
    int total_space;
    int inode_cnt;

    fs_statfs(&total_space, &avail_space, &inode_cnt);

    EVTAG_ASSIGN(response, total_space, total_space);
    EVTAG_ASSIGN(response, avail_space, avail_space);
    EVTAG_ASSIGN(response, inode_cnt, inode_cnt);

    EVRPC_REQUEST_DONE(rpc);
}

static void fsnode_to_stat_copy(struct file_stat * t, fsnode * n){

    if (NULL == n) {
        EVTAG_ASSIGN(t, ino, 0);
        EVTAG_ASSIGN(t, parent_ino, 0);
        EVTAG_ASSIGN(t, size, 0);
        EVTAG_ASSIGN(t, name, "");
        return ;
    } 

    EVTAG_ASSIGN(t, ino, n->ino);
    EVTAG_ASSIGN(t, parent_ino, n->parent->ino);
    EVTAG_ASSIGN(t, size, n->data.fdata.length);
    EVTAG_ASSIGN(t, name, n->name);
    EVTAG_ASSIGN(t, mode, n->mode);
    EVTAG_ASSIGN(t, type, n->type);
    EVTAG_ARRAY_ADD_VALUE(t, pos_arr, n->pos_arr[0]);
    EVTAG_ARRAY_ADD_VALUE(t, pos_arr, n->pos_arr[1]);

}


static void mknod_handler(EVRPC_STRUCT(rpc_mknod) * rpc, void *arg)
{
    DBG();
    //ping cmgr, get current cluster_map
    ping_send_request();

    struct mknod_request *request = rpc->request;
    struct mknod_response *response = rpc->reply;
    fsnode *n = fs_mknod(request->parent_ino, request->name, request->type,
                         request->mode);

    logging(LOG_DEUBG, "mknod(parent=%" PRIu64 ", name=%s, type=%d, mode=%04o)",
            request->parent_ino, request->name, request->type, request->mode);

    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);
    fsnode_to_stat_copy(t, n);
    /*EVTAG_ARRAY_ADD_VALUE(t, pos_arr, 8); */
    /*EVTAG_ARRAY_ADD_VALUE(t, pos_arr, 9); */

    log_file_stat("mknode return ", t);

    EVRPC_REQUEST_DONE(rpc);
}



static void symlink_handler(EVRPC_STRUCT(rpc_symlink) * rpc, void *arg)
{
    DBG();
    //ping cmgr, get current cluster_map
    ping_send_request();

    struct symlink_request *request = rpc->request;
    struct symlink_response *response = rpc->reply;
    fsnode *n = fs_symlink(request->parent_ino, request->name, request->path);

    logging(LOG_DEUBG, "symlink(parent=%" PRIu64 ", name=%s, path =%s)",
            request->parent_ino, request->name, request->path);

    struct file_stat *t ;
    EVTAG_GET(response, stat, &t);
    fsnode_to_stat_copy(t, n);
    /*EVTAG_ARRAY_ADD_VALUE(t, pos_arr, 8); */
    /*EVTAG_ARRAY_ADD_VALUE(t, pos_arr, 9); */
    log_file_stat("symlinke return ", t);

    EVRPC_REQUEST_DONE(rpc);
}

static void readlink_handler(EVRPC_STRUCT(rpc_readlink) * rpc, void *arg)
{
    DBG();
    //ping cmgr, get current cluster_map
    ping_send_request();

    struct readlink_request *request = rpc->request;
    struct readlink_response *response = rpc->reply;
    char * p = fs_readlink(request->ino);

    logging(LOG_DEUBG, "readlink(ino =%" PRIu64 ")", request->ino);
    logging(LOG_DEUBG, "readlink return : %s", p);

    EVTAG_ASSIGN(response, path, p);

    EVRPC_REQUEST_DONE(rpc);
}



static void lookup_handler(EVRPC_STRUCT(rpc_lookup) * rpc, void *arg)
{
    DBG();

    struct lookup_request *request = rpc->request;
    struct lookup_response *response = rpc->reply;

    logging(LOG_DEUBG, "lookup(parent=%" PRIu64 ", name=%s)",
            request->parent_ino, request->name);

    fsnode *n = fs_lookup(request->parent_ino, request->name);
    struct file_stat *t = EVTAG_ARRAY_ADD(response, stat_arr);

    fsnode_to_stat_copy(t, n);

    log_file_stat("lookup return ", t);
    EVRPC_REQUEST_DONE(rpc);
}

static void unlink_handler(EVRPC_STRUCT(rpc_unlink) * rpc, void *arg)
{
    DBG();

    struct unlink_request *request = rpc->request;
    //struct unlink_response * response = rpc->reply;

    logging(LOG_DEUBG, "unlink (parent=%" PRIu64 ", name=%s)",
            request->parent_ino, request->name);

    fs_unlink(request->parent_ino, request->name);

    EVRPC_REQUEST_DONE(rpc);
}



static void mkfs_handler(EVRPC_STRUCT(rpc_mkfs) * rpc, void *arg)
{
    DBG();

    struct mkfs_request * request = rpc->request;
    /*struct mkfs_response * response = rpc->reply;*/

    int mds1, mds2;
    EVTAG_ARRAY_GET(request, pos_arr, 0, &mds1);
    EVTAG_ARRAY_GET(request, pos_arr, 1, &mds2);

    logging(LOG_DEUBG, "mkfs (mds1=%d, mds2=%d)", mds1, mds2);
    fs_mkfs(mds1, mds2);
    EVRPC_REQUEST_DONE(rpc);
}

static void rpc_setup()
{
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    char *listen_host = cfg_getstr("MDS2CLIENT_LISTEN_HOST", "*");
    int port = cfg_getint32("MDS2CLIENT_LISTEN_PORT", 9527);

    http = evhttp_start(listen_host, port);
    /*http = evhttp_start("192.168.1.102", port); */
    if (!http) {
        perror("can't start server!");
        exit(-1);
    }
    printf("Start mds at %s:%d\n", listen_host, port);

    base = evrpc_init(http);

    EVRPC_REGISTER(base, rpc_ping, ping, pong, ping_handler, NULL);

    EVRPC_REGISTER(base, rpc_stat, stat_request, stat_response, stat_handler,
                   NULL);
    EVRPC_REGISTER(base, rpc_ls, ls_request, ls_response, ls_handler, NULL);
    EVRPC_REGISTER(base, rpc_mknod, mknod_request, mknod_response,
                   mknod_handler, NULL);
    EVRPC_REGISTER(base, rpc_lookup, lookup_request, lookup_response,
                   lookup_handler, NULL);
    EVRPC_REGISTER(base, rpc_setattr, setattr_request, setattr_response,
                   setattr_handler, NULL);
    EVRPC_REGISTER(base, rpc_unlink, unlink_request, unlink_response,
                   unlink_handler, NULL);

    EVRPC_REGISTER(base, rpc_statfs, statfs_request, statfs_response,
                   statfs_handler, NULL);
    EVRPC_REGISTER(base, rpc_mkfs, mkfs_request, mkfs_response,
                   mkfs_handler, NULL);
    EVRPC_REGISTER(base, rpc_symlink, symlink_request, symlink_response,
                   symlink_handler, NULL);
    EVRPC_REGISTER(base, rpc_readlink, readlink_request, readlink_response,
                   readlink_handler, NULL);

}

void usage(const char *appname)
{

}

int main(int argc, char **argv)
{
    init_app(argc, argv, "mds");
    event_init();

    fs_init();
    rpc_setup();
    char *self_host = cfg_getstr("MDS2CLIENT_LISTEN_HOST", "*");
    int self_port = cfg_getint32("MDS2CLIENT_LISTEN_PORT", 9527);
    rpc_client_setup(self_host, self_port, MACHINE_MDS);

    event_dispatch();
    return 0;
}
