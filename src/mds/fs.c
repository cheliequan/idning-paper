#include "fs.h"
static uint64_t cur_ino = 3;
static fsnode *nodehash[NODEHASHSIZE];
static uint64_t version;

void fsnode_tree_insert(fsnode * p, fsnode * n)
{
    if (NULL == p->data.ddata.children) {
        fsnode *new_node = fsnode_new();
        dlist_t *pl = &(new_node->tree_dlist);
        dlist_init(pl);
        p->data.ddata.children = new_node;
    }
    fsnode *bucket = p->data.ddata.children;

    dlist_t *head = &(bucket->tree_dlist);
    dlist_t *pl = &(n->tree_dlist);
    dlist_insert_head(head, pl);
}

fsnode *fsnode_tree_find(fsnode * p, uint64_t ino)
{
    fsnode *children_head = p->data.ddata.children;
    dlist_t *head = &(children_head->tree_dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, fsnode, tree_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

/* useless 
 * */
fsnode *fsnode_tree_lookup(fsnode * p, char *name)
{
    // todo : copy from fsnode_tree_find
    return NULL;
}

//把这个node 从父节点的众儿子中删除，解除兄弟关系 ---??解除父子关系
inline fsnode *fsnode_tree_remove(fsnode * n)
{
    dlist_t *pl = &(n->tree_dlist);
    dlist_remove(pl);
    return n;
}

inline fsnode *fsnode_hash_insert(fsnode * n)
{
    uint32_t pos = NODEHASHPOS(n->ino);
    if (NULL == nodehash[pos]) {
        nodehash[pos] = fsnode_new();
        dlist_t *pl = &(nodehash[pos]->hash_dlist);
        dlist_init(pl);
    }
    fsnode *bucket = nodehash[pos];

    dlist_t *head = &(bucket->hash_dlist);
    dlist_t *pl = &(n->hash_dlist);
    dlist_insert_head(head, pl);
    return n;
}

inline fsnode *fsnode_hash_find(uint64_t ino)
{
    uint32_t pos = NODEHASHPOS(ino);
    fsnode *bucket = nodehash[pos];

    if (bucket == NULL)
        return NULL;

    fsnode *p;
    dlist_t *head = &(bucket->hash_dlist);
    dlist_t *pl;
    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, fsnode, hash_dlist);
        if (p->ino == ino)
            return p;
    }
    return NULL;
}

inline fsnode *fsnode_hash_remove(fsnode * p)
{
    dlist_t *pl = &(p->hash_dlist);
    dlist_remove(pl);
    return 0;
}

inline fsnode *fsnode_new()
{
    fsnode *p = (fsnode *) malloc(sizeof(fsnode));
    return p;
}

int fs_init()
{
    DBG();

    fsnode *vroot;              // for those fsnode whose parent is not in the same mds
    vroot = fsnode_new();
    vroot->ino = 0;
    vroot->mode = S_IFDIR;
    vroot->name = "-";
    vroot->nlen = strlen(vroot->name);
    vroot->parent = vroot;
    vroot->data.ddata.children = NULL;

    fsnode_hash_insert(vroot);
    return 0;
}

int fs_setattr(uint64_t ino, struct file_stat *st)
{
    logging(LOG_DEUBG, "fs_setattr(%" PRIu64 ")", ino);

    fsnode *n = fsnode_hash_find(ino);
    n->data.fdata.length = st->size;
    version++;
    return 0;
}

int fs_stat(uint64_t ino, struct file_stat *st)
{
    logging(LOG_DEUBG, "fs_stat(%" PRIu64 ")", ino);
    fsnode *n = fsnode_hash_find(ino);
    if (n == NULL) {
        st->ino = 0;
        return 1;
    }
    st->ino = ino;
    st->size = n->data.fdata.length;
    st->mode = n->mode;
    EVTAG_ARRAY_ADD_VALUE(st, pos_arr, n->pos_arr[0]);
    EVTAG_ARRAY_ADD_VALUE(st, pos_arr, n->pos_arr[1]);
    return 0;
}

//返回的是children链表上的一个元素，链表中所有元素为兄弟.
fsnode *fs_ls(uint64_t ino)
{
    logging(LOG_DEUBG, "fs_ls(%" PRIu64 ")", ino);
    fsnode *n = fsnode_hash_find(ino);
    return n->data.ddata.children;
}

fsnode *fs_lookup(uint64_t parent_ino, char *name)
{
    logging(LOG_DEUBG, "fs_lookup (parent_ino = %" PRIu64 " , name = %s)",
            parent_ino, name);

    fsnode *n = fsnode_hash_find(parent_ino);
    n = n->data.ddata.children;
    if (!n)
        return NULL;

    fsnode *p;

    dlist_t *head = &(n->tree_dlist);
    if (NULL == head)
        return NULL;
    dlist_t *pl;

    for (pl = head->next; pl != head; pl = pl->next) {
        p = dlist_data(pl, fsnode, tree_dlist);

        if (0 == strcmp(name, p->name)) {
            return p;
        }
    }
    logging(LOG_DEUBG,
            "fs_lookup (parent_ino = %" PRIu64 " , name = %s) return NULL!!",
            parent_ino, name);
    return NULL;
}

fsnode *fs_unlink(uint64_t parent_ino, char *name)
{
    logging(LOG_DEUBG, "fs_unlink(parent_ino = %" PRIu64 " , name = %s)",
            parent_ino, name);

    fsnode *s = fs_lookup(parent_ino, name);
    fsnode_tree_remove(s);
    fsnode_hash_remove(s);
    free(s);
    version++;
    return NULL;
}

fsnode *fs_mknod(uint64_t parent_ino, char *name, int type, int mode)
{
    logging(LOG_DEUBG, "fs_mknod(parent_ino = %" PRIu64 " , name = %s)",
            parent_ino, name);
    fsnode *n = fsnode_new();
    n->parent = fsnode_hash_find(parent_ino);
    n->ino = cur_ino++;
    n->type = type;
    n->mode = mode;
    n->name = strdup(name);
    n->nlen = strlen(n->name);
    if (n->mode & S_IFREG) {    //is file 
        n->data.fdata.length = 0;
        n->pos_arr[0] = select_osd();
        n->pos_arr[1] = select_osd();
    } else if (n->mode & S_IFDIR) {
        n->pos_arr[0] = n->parent->pos_arr[0];
        n->pos_arr[1] = n->parent->pos_arr[1];
        n->data.ddata.children = NULL;
    }

    fsnode_hash_insert(n);
    fsnode_tree_insert(n->parent, n);
    version++;
    return n;
}

fsnode *fs_symlink(uint64_t parent_ino, const char *name, const char *path)
{
    logging(LOG_DEUBG,
            "fs_symlink(parent_ino = %" PRIu64 " , name = %s, path = %s)",
            parent_ino, name, path);
    fsnode *n = fsnode_new();
    n->ino = cur_ino++;
    n->type = 255;              //TODO

    n->mode = S_IFLNK;
    n->name = strdup(name);
    n->nlen = strlen(n->name);
    n->data.sdata.path = strdup(path);
    n->data.sdata.pleng = strlen(path);

    n->parent = fsnode_hash_find(parent_ino);

    n->pos_arr[0] = n->parent->pos_arr[0];
    n->pos_arr[1] = n->parent->pos_arr[1];
    fsnode_hash_insert(n);
    fsnode_tree_insert(n->parent, n);
    version++;
    return n;
}

char *fs_readlink(uint64_t ino)
{
    fsnode *n = fsnode_hash_find(ino);
    if (n && n->mode == S_IFLNK)
        return n->data.sdata.path;
    return NULL;
}

/*
 * make root of a whole fs, call by mkfs.sfs
 * */
int fs_mkfs(int mds1, int mds2)
{
    DBG();

    fsnode *root;               // for those fsnode whose parent is not in the same mds
    root = fsnode_new();
    root->ino = 1;
    root->mode = S_IFDIR;
    root->name = "/";
    root->nlen = strlen(root->name);
    root->parent = root;
    root->data.ddata.children = NULL;

    root->pos_arr[0] = mds1;
    root->pos_arr[1] = mds2;

    fsnode_hash_insert(root);

    return 0;
}

void fs_statfs(int *total_space, int *avail_space, int *inode_cnt)
{
    *total_space = 1024;
    *avail_space = 1024;
    *inode_cnt = 8888;
}
