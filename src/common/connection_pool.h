#ifndef _CONN_POLL
#define _CONN_POLL
#include "hashtable.h"
#include "dlist.h"


/*
 * a map of int -> list 
 *
 * actually it's a mutiMap
 * */
typedef struct ConnectionPool{
    Hashtable * mid_to_conn_hashtable; // map from machine id to conn;
}ConnectionPool;

/*
 * this is a list
 * */
struct PoolEntry{
    int mid;
    char * host;
    int port;

    struct evhttp_connection *conn;
    struct dlist_t dlist;
};

ConnectionPool * connection_pool_new();
ConnectionPool * connection_pool_free(ConnectionPool * pool);


void connection_pool_insert(ConnectionPool * pool, int mid, char * host, int port, struct evhttp_connection * conn);

struct evhttp_connection * connection_pool_get_free_conn(ConnectionPool * pool , int mid);


#endif /* _CONN_POLL*/
