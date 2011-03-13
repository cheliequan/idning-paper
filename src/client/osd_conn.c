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
static void flush_write_buf(struct write_buf * b);

#define BUF_HASH_BITS (22)  
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
        //evbuffer_free(b->evb); //TODO. is this freeed by http_post
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

void buffered_write(uint32_t ino, uint64_t offset, uint32_t size, const uint8_t * buff){
    DBG();
    struct write_buf * b = write_buf_hash_find(ino);
    if (!b){
        b= write_buf_new(ino);
        b->ino = ino;
        b->offset = offset;
        b->size = size;
        evbuffer_add(b->evb, buff, size);
        write_buf_hash_insert(b);
    }else{
        if (offset == ( b->offset+b->size) ){
            b->size += size;
            evbuffer_add(b->evb, buff, size);
            //write_buf_hash_insert(b);
        }else{ //不连续
            flush_write_buf(b);

            write_buf_hash_remove(b);
            write_buf_free(b);
            buffered_write(ino, offset, size, buff);
        }

    }
}

int buffered_write_flush(uint32_t ino){
    DBG();
    struct write_buf * b = write_buf_hash_find(ino);
    if (NULL == b)
        return ;
    flush_write_buf(b);
    write_buf_hash_remove(b);
    write_buf_free(b);
    return b->offset+b->size;

}

static void flush_write_buf(struct write_buf * b){
    DBG();

    char url[256];
    sprintf(url, "http://127.0.0.1:6006/put/%lu", b->ino);

    struct evkeyvalq * headers = (struct evkeyvalq *) malloc( sizeof(struct evkeyvalq));
    TAILQ_INIT(headers);
    char range[255];
    sprintf(range, "bytes=%d-%d", (int)b->offset, (int)b->offset+b->size-1);
    logging(LOG_DEUBG, range);

    evhttp_add_header(headers, "Range", range);

    http_response * response = http_post(url, headers, b->evb);
    int len = evbuffer_get_length(response->body);
    free(headers);

}

void osd_conn_init(){
    DBG();
}

/*int main(){*/
    /*event_init();*/
    /*rpc_basic_client();*/
    /*doit();*/
/*}*/

