#include "fs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

inline fsnode* fsnode_hash_insert(fsnode * n) {
    uint32_t pos = NODEHASHPOS(n->ino);
    if (NULL == nodehash[pos]){
        nodehash[pos] = fsnode_new();
        dlist_t * pl = &(nodehash[pos] -> hash_dlist);
        dlist_init(pl);
    }
    fsnode * bucket = nodehash[pos];

    dlist_t * head = &(bucket->hash_dlist);
    dlist_t * pl = &(n->hash_dlist);
    dlist_insert_head(head, pl);
}
inline fsnode* fsnode_hash_find(uint32_t ino) {
    uint32_t pos = NODEHASHPOS(ino);
    fsnode * bucket = nodehash[pos];
    fsnode * p;
    dlist_t * head = &(bucket->hash_dlist);
    dlist_t * pl;
    for (pl = head->next; pl!=head; pl=pl->next){
        p = dlist_data(pl, fsnode, hash_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

inline fsnode* fsnode_hash_remove(fsnode * p) {
    dlist_t * pl = & (p->hash_dlist);
    dlist_remove(pl);
}


inline fsnode* fsnode_new() {
    fsnode *p = (fsnode*)malloc(sizeof(fsnode));
    return p;
}

int fs_init(){
    root = fsnode_new();
    root -> ino = 8;
    
    dlist_t * p = &( root->tree_dlist );
    fsnode * r = dlist_data(p, fsnode, tree_dlist);
    fprintf(stderr, "root : %p \n", root);
    fprintf(stderr, "r : %p \n", r);
    fsnode_hash_insert(r);

    fsnode * n = fsnode_hash_find(8);
    fprintf(stderr, "n : %p \n", n);

    fsnode_hash_remove(n);
    n = fsnode_hash_find(8);
    fprintf(stderr, "n : %p \n", n);

}



int fs_stat(int ino, struct file_stat * st){
    switch (ino) {
        case 1:
            st -> inode = ino;
            st -> size = 2;
            break;

        case 2:
            st -> inode = ino;
            st -> size = 10;
            break;

        default:
            return -1;
    }
    return 0;
}
