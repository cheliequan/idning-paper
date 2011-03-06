#include "fs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
static int cur_ino = 3;

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
    return 0;
}


inline fsnode* fsnode_new() {
    fsnode *p = (fsnode*)malloc(sizeof(fsnode));
    return p;
}

int fs_init(){
    root = fsnode_new();
    root -> ino = 1;
    root -> mode = S_IFDIR;
    root -> name = "/";
    root -> nlen = strlen(root ->name);
    root -> parent = root;
    root -> data.ddata.children = NULL;

    dlist_t * p = &( root->tree_dlist );
    fsnode_hash_insert(root);

    fsnode * n = fsnode_hash_find(1);
    fprintf(stderr, "n : %p \n", n);

    fsnode * node2 = fsnode_new();
    fprintf(stderr, "node2 : %p \n", node2);
    node2 -> ino = 2;
    node2 -> name = "hello_";
    node2 -> mode = S_IFREG;
    node2 -> nlen = strlen(node2->name);
    node2 -> parent = root;
    node2 -> data.fdata.length = 10;

    fsnode_hash_insert(node2);
    fsnode_tree_insert(root, node2);

    fsnode * node3 = fsnode_tree_find(root, 2);
    fprintf(stderr, "node3 : %p \n", node3);

    return 0;
}

int fs_setattr(int ino, struct file_stat * st){
    fsnode *n = fsnode_hash_find(ino);
    n->data.fdata.length = st->size;
    return 0;
}

int fs_stat(int ino, struct file_stat * st){
    fsnode *n = fsnode_hash_find(ino);
    st->ino = ino;
    st->size = n-> data.fdata.length;
    st->mode = n-> mode;
    return 0;
}
//返回的是children链表上的一个元素，链表中所有元素为兄弟.
fsnode * fs_ls(int ino){
    fsnode *n = fsnode_hash_find(ino);
    /*if (n->type != TYPE_DIRECTORY)*/
        /*return NULL;*/
    return n->data.ddata.children;
}

fsnode * fs_lookup(int parent_ino, char * name){
    fprintf(stderr, "fs_lookup : parent_ino: %d , name: %s\n", parent_ino, name);

    fsnode *n = fsnode_hash_find(parent_ino);
    n = n-> data.ddata.children;

    fsnode * p;

    dlist_t * head = &(n -> tree_dlist);
    if (NULL == head)
        return NULL;
    dlist_t * pl;

    for (pl = head->next; pl!=head; pl=pl->next){
        p = dlist_data(pl, fsnode, tree_dlist);
        fprintf(stderr, "pl : %p , p: %p\n", pl, p);

        if (0 == strcmp(name, p->name)){
            fprintf(stderr, "fs_lookup return p: %p, \n",  p);
            return p;
        }
    }
    /*if (n->type != TYPE_DIRECTORY)*/
        /*return NULL;*/
    fprintf(stderr, "fs_lookup return NULL \n");
    return NULL;
}

fsnode * fs_mknod(int parent_ino, char * name, int type, int mode){
    fsnode * n = fsnode_new();
    n -> ino = cur_ino++;
    n -> type = type;
    n -> mode = mode;
    n -> name = strdup(name);
    n -> nlen = strlen(n->name);
    if (n->mode & S_IFREG){ //is file 
        n->data.fdata.length = 10;
    }else if (n->mode & S_IFDIR){
        n->data.ddata.children = NULL;
    }

    n -> parent = fsnode_hash_find(parent_ino);

    fsnode_hash_insert(n);
    fsnode_tree_insert(n->parent, n);
    return n;
}
