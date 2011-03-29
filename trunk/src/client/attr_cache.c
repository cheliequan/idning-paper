#include "sfs_common.h"

static Hashtable *  attr_ht;

int attr_cache_init(){
    attr_ht = hashtable_new(hash_int64, equal_int64, 1024);
    return 0;
}
int attr_cache_add(struct file_stat * st){
    assert(st->ino != 0);
    assert(st->parent_ino != 0);
    log_file_stat("attr_cache_add: ", st);

    hashtable_insert(attr_ht, &(st->ino), st);
    return 0;
}
int attr_cache_del(uint64_t ino){
    assert(0);
}
struct file_stat * attr_cache_lookup(uint64_t ino){
    struct file_stat * st = hashtable_lookup(attr_ht, &ino);
    return st;
}

