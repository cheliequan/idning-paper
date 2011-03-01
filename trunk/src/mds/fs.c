#include "fs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void fsnode_tree_insert(fsnode * p, fsnode * n) {
    if (NULL == p->data.ddata.children){
        fsnode * new_node = fsnode_new();
        dlist_t * pl = &(new_node -> tree_dlist);
        dlist_init(pl);
        p->data.ddata.children = new_node;
    }
    fsnode * bucket = p->data.ddata.children;

    dlist_t * head = &(bucket->tree_dlist);
    dlist_t * pl = &(n->tree_dlist);
    dlist_insert_head(head, pl);
}

fsnode* fsnode_tree_find(fsnode * p, uint32_t ino) {
    fsnode * children_head = p->data.ddata.children;
    dlist_t * head = &(children_head -> tree_dlist);
    dlist_t * pl;
    for (pl = head->next; pl!=head; pl=pl->next){
        p = dlist_data(pl, fsnode, tree_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

fsnode* fsnode_tree_lookup(fsnode * p, char * name) {
     // todo : copy from fsnode_tree_find
}

//把这个node 从父节点的众儿子中删除，解除兄弟关系 ---??解除父子关系
inline fsnode* fsnode_tree_remove(fsnode * n) {
    dlist_t * pl = & (n->tree_dlist);
    dlist_remove(pl);
}

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
    root -> ino = 0;
    
    dlist_t * p = &( root->tree_dlist );
    fsnode * r = dlist_data(p, fsnode, tree_dlist);
    fprintf(stderr, "root : %p \n", root);
    fprintf(stderr, "r : %p \n", r);
    fsnode_hash_insert(r);

    fsnode * n = fsnode_hash_find(0);
    fprintf(stderr, "n : %p \n", n);

    fsnode_hash_remove(n);
    n = fsnode_hash_find(0);
    fprintf(stderr, "n : %p \n", n);


    fsnode * node2 = fsnode_new();
    fprintf(stderr, "node2 : %p \n", node2);
    node2 -> ino = 2;
    node2 -> type = TYPE_FILE;
    node2 -> name = "hello_";
    node2 -> nlen = strlen(node2->name);
    node2 -> parent = root;
    node2 -> data.fdata.length = 10;
    fsnode_hash_insert(node2);
    fsnode_tree_insert(root, node2);

    fsnode * node3 = fsnode_tree_find(root, 2);
    fprintf(stderr, "node3 : %p \n", node3);
    fsnode_tree_remove(node3);

    node3 = fsnode_tree_find(root, 2);
    fprintf(stderr, "after remove from tree _ node3 : %p \n", node3);
}

int fs_stat(int ino, struct file_stat * st){
    fsnode *n = fsnode_hash_find(ino);
    st->inode = ino;
    st->size = n-> data.fdata.length;
    return 0;
}
