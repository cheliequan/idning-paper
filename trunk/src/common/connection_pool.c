#include "connection_pool.h"
#include "dlist.h"


ConnectionPool * connection_pool_new(){
    ConnectionPool * pool = (ConnectionPool * )malloc(sizeof(ConnectionPool ));
    Hashtable *ht = hashtable_new(hash_int_hash, hash_int_equal, 1024);
    pool->mid_to_conn_hashtable = ht;
    return pool;
};

ConnectionPool * connection_pool_free(ConnectionPool * pool){
    assert(0);

}


void connection_pool_insert(ConnectionPool * pool, int mid, char * host, int port, struct evhttp_connection * conn){
    struct PoolEntry * e = malloc(sizeof(struct PoolEntry));      
    e -> mid = mid;
    e -> host = strdup(host);
    e -> port = port;
    dlist_t * pl = &( e -> dlist);
    dlist_init(pl);
    
    struct PoolEntry * old = hashtable_lookup(pool->mid_to_conn_hashtable, &(e->mid));
    if (NULL == old){
        hashtable_insert(pool->mid_to_conn_hashtable, &(e->mid), e);
        return;
    }

    dlist_t * headpl= &( old -> dlist);
    dlist_insert_head(headpl, pl);
}

struct evhttp_connection * connection_pool_get_free_conn(ConnectionPool * pool , int mid){
    struct PoolEntry * old = hashtable_lookup(pool->mid_to_conn_hashtable, &(mid));
    if (NULL == old){
        return NULL;
    }


    struct PoolEntry* p;
    dlist_t * head = &(old->dlist);
    dlist_t * pl;
    for (pl = head->next; pl!=head; pl=pl->next){
        p = dlist_data(pl, struct PoolEntry, dlist);
        dlist_remove(pl);
        return p->conn;
    }
    return NULL;
}
