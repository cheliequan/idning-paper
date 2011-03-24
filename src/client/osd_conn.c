#include <event.h>
#include <evhttp.h>
#include <stdlib.h>
#include <string.h>
#include <fuse_lowlevel.h>
#include <sys/queue.h>


#include "protocol.gen.h"
#include "protocol.h"
#include "log.h"
#include "dlist.h"
#include "http_client.h"
#include "cluster.h"

#define CFG_BLOCK_SIZE 524288000
/* 只保存连续buffer, 不连续的马上flush.
 *
 * 否则的话，就要保存: {offse, size, buffer,}这样的结构数组，而且osd得支持这样的写法，好像不太现实.
 */
struct write_buf{
    fuse_ino_t ino;
    off_t offset;
    size_t size;
    struct evbuffer * evb;

    struct dlist_t hash_dlist; 

};
static void flush_write_buf(struct file_stat * stat, struct write_buf * b);

#define BUF_HASH_BITS (10)  
#define BUF_HASH_SIZE (1<<BUF_HASH_BITS) 
#define BUF_HASH_POS(nodeid) ((nodeid)&(BUF_HASH_SIZE-1))

struct write_buf * buf_hash[BUF_HASH_SIZE];

struct write_buf * write_buf_new(){
    struct write_buf * rst = malloc(sizeof(struct write_buf));
    rst->evb =  evbuffer_new();
    return rst;
}

void write_buf_free(struct write_buf * b){
    if (b->evb){
        evbuffer_free(b->evb); 
    }
    free(b);
}


struct write_buf * write_buf_hash_insert(struct write_buf* b) {
    DBG();
    uint32_t pos = BUF_HASH_POS(b->ino);
    if (NULL == buf_hash[pos]){
        buf_hash[pos] = write_buf_new();
        dlist_t * pl = &(buf_hash[pos] -> hash_dlist);
        dlist_init(pl);
    }
    struct write_buf * bucket = buf_hash[pos];

    dlist_t * head = &(bucket->hash_dlist);
    dlist_t * pl = &(b->hash_dlist);
    dlist_insert_head(head, pl);
    logging(LOG_DEUBG, "after dlist_insert_head");

    logging(LOG_DEUBG, "pl : %p, pl->next: %p ; head: %p; head->next: %p;", pl, pl->next, head, head->next);
    
    return b;
}

struct write_buf* write_buf_hash_find(uint32_t ino) {
    uint32_t pos = BUF_HASH_POS(ino);
    struct write_buf * bucket = buf_hash[pos];
    if (bucket == NULL)
        return NULL;
    struct write_buf * p;
    dlist_t * head = &(bucket->hash_dlist);
    dlist_t * pl;
    for (pl = head->next; pl!=head; pl=pl->next){
        logging(LOG_DEUBG, "pl : %p, pl->next: %p ; head: %p; head->next: %p;", pl, pl->next, head, head->next);
        p = dlist_data(pl, struct write_buf, hash_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

struct write_buf* write_buf_hash_remove(struct write_buf * p) {
    DBG();
    dlist_t * pl = & (p->hash_dlist);
    dlist_remove(pl);
    return 0;
}

void buffered_write(struct file_stat * stat, uint64_t offset, uint32_t size, const uint8_t * buff){
    DBG();
    struct write_buf * b = write_buf_hash_find(stat->ino);
    if (!b){
        b= write_buf_new(stat->ino);
        b->ino = stat->ino;
        b->offset = offset;
        b->size = size;
        evbuffer_add(b->evb, buff, size);
        write_buf_hash_insert(b);
    }else{
        if( ( b->size > CFG_BLOCK_SIZE )  // 一个块满了.
          ||(  offset != ( b->offset+b->size)  ) //不连续块.
          ){

            flush_write_buf(stat, b);

            write_buf_hash_remove(b);
            write_buf_free(b);
            buffered_write(stat, offset, size, buff);

        }else{ //连续块，追加在它后面
            b->size += size;
            evbuffer_add(b->evb, buff, size);
        }

    }
}

int buffered_write_flush(struct file_stat * stat){
    DBG();
    struct write_buf * b = write_buf_hash_find(stat->ino);
    if (NULL == b)
        return 0;
    flush_write_buf(stat, b);
    write_buf_hash_remove(b);
    write_buf_free(b);
    return b->offset+b->size;

}

/*
 * I do this after read the src of libevent 2.0.1
 * */
static struct evbuffer * evbuffer_dup(struct evbuffer * orig){
    int len = evbuffer_get_length(orig);
    char * data  = malloc(sizeof(len)); // this maybe large!!!!!
    if (!data)
        logging(LOG_ERROR, "ning: no memory on evbuffer_dup");
    evbuffer_copyout(orig, data, len); //被吃掉，orig现在为空了.
    struct evbuffer * new_buf = evbuffer_new();
    evbuffer_add(new_buf, data, len);
    /*evbuffer_add(orig   , data, len);  //放回去.*/

    logging(LOG_INFO, "len1 : %d", evbuffer_get_length(orig));
    logging(LOG_INFO, "len2 : %d", evbuffer_get_length(new_buf));

    return new_buf;
}

static void flush_write_buf(struct file_stat * stat, struct write_buf * b){
    DBG();
    int i;
    for(i = 0; i< 2; i++){
        struct evbuffer * buf = evbuffer_dup(b->evb); 

        int osd = stat->pos_arr[i];
        struct machine * m = cluster_get_machine_by_mid(osd);


        char url[256];
        sprintf(url, "http://%s:%d/put/%lu", m->ip, m->port, b->ino);

        struct evkeyvalq * headers = (struct evkeyvalq *) malloc( sizeof(struct evkeyvalq));
        TAILQ_INIT(headers);
        char range[255];
        sprintf(range, "bytes=%d-%d", (int)b->offset, (int)b->offset+b->size-1);
        logging(LOG_DEUBG, range);

        evhttp_add_header(headers, "Range", range);

        http_response * response = http_post(url, headers, buf);
        evbuffer_get_length(response->body);
        free(headers);
    }

}

void osd_conn_init(){
    DBG();
}

