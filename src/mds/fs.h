/*
the source code is mainly from mfs/mfsmaster/filesystem.c
*/
#include <stdio.h>
#include "protocol.gen.h"
#include "dlist.h"


typedef struct _fsnode {
    uint32_t ino;   //4
    uint32_t ctime,mtime,atime; //16
    uint8_t type; //
    uint8_t goal;  
    uint16_t mode;  // only 12 lowest bits are used for mode, in unix standard upper 4 are used for object type, but since there is field "type" this bits can be used as extra flags
    //20
    uint32_t uid;
    uint32_t gid;
    struct _fsnode * parent;

    uint16_t nlen;
    uint8_t * name;

    struct dlist_t tree_dlist; //在parent->children构成的链表 实际上是同父的众节点之间的兄弟关系，用于ls操作. 
    struct dlist_t hash_dlist; //在hash[]构成的链表中，用于根据id 找文件. lookup操作
    
    union _data {
        struct _ddata {             // type==TYPE_DIRECTORY
            struct _fsnode * children;
            uint32_t nlink;
            uint32_t elements;
        } ddata;
        //////////////////
        struct _sdata {             // type==TYPE_SYMLINK
            uint32_t pleng;
            uint8_t *path;
        } sdata;
        //////////////////
        uint32_t rdev;              // type==TYPE_BLOCKDEV ; type==TYPE_CHARDEV
        /////////////////
        struct _fdata {             // type==TYPE_FILE
            uint64_t length;
            //uint64_t *chunktab;
            //uint32_t chunks;
        } fdata;
        /////////////////
    } data; //52
} fsnode;

/* type for readdir command */
//#define TYPE_FILE             'f'
//#define TYPE_SYMLINK          'l'
//#define TYPE_DIRECTORY        'd'
//#define TYPE_FIFO             'q'
//#define TYPE_BLOCKDEV         'b'
//#define TYPE_CHARDEV          'c'
//#define TYPE_SOCKET           's'
//#define TYPE_UNKNOWN          '?'

static fsnode *root;

#define NODEHASHBITS (22)  
#define NODEHASHSIZE (1<<NODEHASHBITS) 
#define NODEHASHPOS(nodeid) ((nodeid)&(NODEHASHSIZE-1))

static fsnode* nodehash[NODEHASHSIZE];
static uint64_t version;


fsnode* fsnode_new() ;

//static inline fsnode* fsnode_create(uint32_t ts,fsnode* parent,uint16_t nleng,const uint8_t *name,uint8_t type,uint16_t mode,uint32_t uid,uint32_t gid) {
//}

//static inline fsnode* fsnode_hash_insert(fsnode *p) {
    //int nodepos = NODEHASHPOS(p->id);
    //p->next = nodehash[nodepos];
    //nodehash[nodepos] = p;
    //return p;
//}

//static inline fsnode* fsnodes_hash_del(fsnode * p) {
    //for (p=nodehash[nodepos]; p ; p=p->next ) {
        //if (p->id == id) {
            //return p;
        //}
    //}
    //return NULL;
//}


//static inline void fsnodes_remove_node(uint32_t ts,fsnode *toremove) {
    //uint32_t nodepos;
    //fsnode **ptr;
    //if (toremove->parents!=NULL) {
        //return;
    //}
//// remove from idhash
    //nodepos = NODEHASHPOS(toremove->id);
    //ptr = &(nodehash[nodepos]);
    //while (*ptr) {
        //if (*ptr==toremove) {
            //*ptr=toremove->next;
            //break;
        //}
        //ptr = &((*ptr)->next);
    //}
//// and free
    //nodes--;
    //if (toremove->type==TYPE_DIRECTORY) {
        //dirnodes--;
//#ifndef METARESTORE
        //if (toremove->data.ddata.quota) {
            //fsnodes_delete_quotanode(toremove->data.ddata.quota);
        //}
        //free(toremove->data.ddata.stats);
//#endif
    //}
    //if (toremove->type==TYPE_FILE ) {
        //uint32_t i;
        //uint64_t chunkid;
        //filenodes--;
        //for (i=0 ; i<toremove->data.fdata.chunks ; i++) {
            //chunkid = toremove->data.fdata.chunktab[i];
            //if (chunkid>0) {
                //if (chunk_delete_file(chunkid,toremove->id,i)!=STATUS_OK) {
                    //syslog(LOG_ERR,"structure error - chunk %016"PRIX64" not found (inode: %"PRIu32" ; index: %"PRIu32")",chunkid,toremove->id,i);
                //}
            //}
        //}
        //if (toremove->data.fdata.chunktab!=NULL) {
            //free(toremove->data.fdata.chunktab);
        //}
    //}
    //if (toremove->type==TYPE_SYMLINK) {
        //free(toremove->data.sdata.path);
    //}
    //fsnodes_free_id(toremove->id,ts);
    //free(toremove);
//}




//uint8_t fs_getrootinode(uint32_t *rootinode,const uint8_t *path) {
    //uint32_t nleng;
    //const uint8_t *name;
    //fsnode *p;
    //fsedge *e;

    //name = path;
    //p = root;
    //for (;;) {
        //while (*name=='/') {
            //name++;
        //}
        //if (*name=='\0') {
            //*rootinode = p->id;
            //return STATUS_OK;
        //}
        //nleng=0;
        //while (name[nleng] && name[nleng]!='/') {
            //nleng++;
        //}
        //if (fsnodes_namecheck(nleng,name)<0) {
            //return ERROR_EINVAL;
        //}
        //e = fsnodes_lookup(p,nleng,name);
        //if (!e) {
            //return ERROR_ENOENT;
        //}
        //p = e->child;
        //if (p->type!=TYPE_DIRECTORY) {
            //return ERROR_ENOTDIR;
        //}
        //name += nleng;
    //}
//}

//void fs_statfs(uint32_t rootinode,uint8_t sesflags,uint64_t *totalspace,uint64_t *availspace,uint64_t *trspace,uint64_t *respace,uint32_t *inodes) {
    //fsnode *rn;
    //quotanode *qn;
    //statsrecord sr;
    //(void)sesflags;
////  matocsserv_getspace(totalspace,availspace);
////  *inodes = nodes;
////  *trspace = trashspace;
////  *respace = reservedspace;
    //if (rootinode==MFS_ROOT_ID) {
        //*trspace = trashspace;
        //*respace = reservedspace;
        //rn = root;
    //} else {
        //*trspace = 0;
        //*respace = 0;
        //rn = fsnodes_id_to_node(rootinode);
    //}
    //if (!rn || rn->type!=TYPE_DIRECTORY) {
        //*totalspace = 0;
        //*availspace = 0;
        //*inodes = 0;
    //} else {
        //matocsserv_getspace(totalspace,availspace);
        //fsnodes_get_stats(rn,&sr);
        //*inodes = sr.inodes;
        //qn = rn->data.ddata.quota;
        //if (qn && (qn->flags&QUOTA_FLAG_HREALSIZE)) {
            //if (sr.realsize>=qn->hrealsize) {
                //*availspace = 0;
            //} else if (*availspace > qn->hrealsize - sr.realsize) {
                //*availspace = qn->hrealsize - sr.realsize;
            //}
            //if (*totalspace > qn->hrealsize) {
                //*totalspace = qn->hrealsize;
            //}
        //}
        //if (sr.realsize + *availspace < *totalspace) {
            //*totalspace = sr.realsize + *availspace;
        //}
    //}
    //stats_statfs++;
//}


int fs_stat(int ino, struct file_stat * stat);
int fs_setattr(int ino, struct file_stat * st);

fsnode * fs_mknod(int parent_ino, char * name, int type, int mode);
fsnode * fs_unlink(int parent_ino, char * name);

int fs_mkdir();
int fs_create();
int fs_rmdir();
int fs_rename();
int fs_link();
int fs_get_goal();
int fs_set_goal();
int fs_init();
int fs_load();
int fs_store();




