#include "sfs_common.h"
#include "fs.h"

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
        fs_stat(ino, t);
        EVTAG_ASSIGN(t, ino, t->ino);   
        EVTAG_ASSIGN(t, size, t->size); 
        EVTAG_ASSIGN(t, type, t->type); 
        EVTAG_ASSIGN(t, mode, t->mode); 

        logging(LOG_DEUBG,
                "stat(%" PRIu64 ") return {ino: %" PRIu64 ", size: %" PRIu64
                ", type : %d, mode : %04o, pos [%d, %d]}", ino, t->ino, t->size,
                t->type, t->mode, t->pos_arr[0], t->pos_arr[1]);
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
            EVTAG_ASSIGN(t, ino, p->ino);   
            EVTAG_ASSIGN(t, size, p->data.fdata.length);    
            EVTAG_ASSIGN(t, name, p->name); 
            EVTAG_ASSIGN(t, type, p->type);
            EVTAG_ASSIGN(t, mode, p->mode);
            logging(LOG_DEUBG,
                    "ls(%" PRIu64 ") return {ino: %" PRIu64
                    ", name: %s, size: %" PRIu64 ", mode: %04o}", ino, t->ino,
                    t->name, t->size, t->mode);
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

    EVTAG_ASSIGN(t, ino, n->ino);   
    EVTAG_ASSIGN(t, size, n->data.fdata.length);    
    EVTAG_ASSIGN(t, name, n->name); 
    EVTAG_ASSIGN(t, mode, n->mode); 
    EVTAG_ASSIGN(t, type, n->type); 
    EVTAG_ARRAY_ADD_VALUE(t, pos_arr, n->pos_arr[0]);
    EVTAG_ARRAY_ADD_VALUE(t, pos_arr, n->pos_arr[1]);
    /*EVTAG_ARRAY_ADD_VALUE(t, pos_arr, 8); */
    /*EVTAG_ARRAY_ADD_VALUE(t, pos_arr, 9); */

    logging(LOG_DEUBG,
            "mknod(%s) return {ino: %" PRIu64 ", name: %s, size: %" PRIu64
            ", mode: %04o}", request->name, t->ino, t->name, t->size, t->mode);

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
    if (NULL == n) {
        EVTAG_ASSIGN(t, ino, 0);    
        EVTAG_ASSIGN(t, size, 0);   
        EVTAG_ASSIGN(t, name, "");  
    } else {
        EVTAG_ASSIGN(t, ino, n->ino);   
        EVTAG_ASSIGN(t, size, n->data.fdata.length);    
        EVTAG_ASSIGN(t, name, n->name); 
    }
    logging(LOG_DEUBG,
            "lookup(%s) return {ino: %" PRIu64 ", name: %s, size: %" PRIu64 "}",
            request->name, t->ino, t->name, t->size);

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
