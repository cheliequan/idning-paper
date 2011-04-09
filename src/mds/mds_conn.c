#include "sfs_common.h"
#include "fs.h"


struct evrpc_pool *mds_mds_conn_pool;
struct event_base *mds_mds_ev_base;

// server side
//
//

inline void replace_pos(fsnode * n, int from, int to){
    int i = 0;
    for(i=0; i< 2; i++){
        if(n->pos_arr[i] == from)
            n->pos_arr[i] = to;
    }
}

void migrate_handler(EVRPC_STRUCT(rpc_migrate) * rpc, void *arg)
{
    DBG();

    struct migrate_request * req = rpc->request;
    struct migrate_response * response = rpc->reply;
    
    int cnt = EVTAG_ARRAY_LEN(req, stat_arr);
    struct file_stat * stat;
    int i;


    EVTAG_ARRAY_GET(req, stat_arr, 0, &stat);
    fsnode * n = fsnode_new();
    stat_to_fsnode_copy(n, stat);
    log_file_stat("migrate_handler get ", stat);
    
    fsnode_hash_insert(n);
    n->parent = fsnode_hash_find(0);
    fsnode_tree_insert(n->parent, n);
    replace_pos(n, req->from_mds, req->to_mds);

    for (i = 1; i < cnt; i++) {
        EVTAG_ARRAY_GET(req, stat_arr, i, &stat);

        fsnode * n = fsnode_new();
        stat_to_fsnode_copy(n, stat);
        log_file_stat("migrate_handler get ", stat);
        
        fsnode_hash_insert(n);
        n->parent = fsnode_hash_find(stat->parent_ino);
        fsnode_tree_insert(n->parent, n);
        replace_pos(n, req->from_mds, req->to_mds);
    }
    EVTAG_ASSIGN(response, rst, 1);

    EVRPC_REQUEST_DONE(rpc);
}

static void unlink_cb(struct evrpc_status *status, struct unlink_request *request , struct unlink_response * response , void *arg){
    event_loopexit(NULL);
}


//
/////////////////////////////////client side
void migrate_cb(struct evrpc_status *status, struct migrate_request *req, 
                    struct migrate_response *response, void *arg)
{
    fsnode * root = (fsnode * )arg;
    if(root->modifiy_flag){
        /*invalid it at dest mds*/
        /*assert(0);*/

        struct unlink_request *unlink_req = unlink_request_new();
        struct unlink_response *unlink_response = unlink_response_new();
        EVTAG_ASSIGN(unlink_req, parent_ino, 0); //0号节点搜集无根节点.
        EVTAG_ASSIGN(unlink_req, name, root->name);

        EVRPC_MAKE_REQUEST(rpc_unlink, mds_mds_conn_pool, unlink_req, unlink_response, unlink_cb, root);
        /*rpc_grneral_request(ip, port, "/.rpc.rpc_unlink",*/
                            /*req, (marshal_func) unlink_request_marshal,*/
                            /*response, (unmarshal_func) unlink_response_unmarshal);*/

    }else{
        replace_pos(root, req->from_mds, req->to_mds);
        event_base_loopexit(mds_mds_ev_base, NULL);
    }
}


void migrate_add_fsnode_dfs(struct migrate_request * req, fsnode * root){
    struct file_stat * stat = EVTAG_ARRAY_ADD(req, stat_arr);

    fsnode_to_stat_copy(stat, root);
    log_file_stat("migrate_add_fsnode_dfs", stat);

    dlist_t *head ;
    dlist_t *pl;
    fsnode *p;

    if (S_ISDIR(root->mode) && root->data.ddata.children){
        fsnode *children_head = root->data.ddata.children;
        head = &(children_head->tree_dlist);
        for (pl = head->next; pl != head; pl = pl->next) {
            p = dlist_data(pl, fsnode, tree_dlist);
            migrate_add_fsnode_dfs(req, p);
        }
    }
}

int migrate_send_request(char * ip, int port, fsnode * root, int from_mds, int to_mds)
{
    DBG();
    struct migrate_request * req= migrate_request_new();
    struct migrate_response * response = migrate_response_new();
    EVTAG_ASSIGN(req, from_mds, from_mds);
    EVTAG_ASSIGN(req, to_mds, to_mds);

    migrate_add_fsnode_dfs(req, root);

// temp connection TODO
    mds_mds_ev_base = event_base_new();
    mds_mds_conn_pool = evrpc_pool_new(mds_mds_ev_base);
    struct evhttp_connection * evcon = evhttp_connection_new(ip, port);
    evrpc_pool_add_connection(mds_mds_conn_pool, evcon);


    EVRPC_MAKE_REQUEST(rpc_migrate, mds_mds_conn_pool, req, response, migrate_cb, root);
    event_base_dispatch(mds_mds_ev_base);
    return 0;
}

/*void do_migrate(){*/


/*}*/

