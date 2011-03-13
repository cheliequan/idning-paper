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


inline fsnode* fsnode_hash_find(uint32_t ino) ;

int fs_stat(int ino, struct file_stat * stat);
int fs_setattr(int ino, struct file_stat * st);

/*
 * create, mkdir
 * */
fsnode * fs_mknod(int parent_ino, char * name, int type, int mode);
/*
 *  rmdir 
 *  rm
 * */
fsnode * fs_unlink(int parent_ino, char * name);
fsnode * fs_lookup(int parent_ino, char * name);
fsnode * fs_ls(int ino);

int fs_rename();
int fs_link();
int fs_get_goal();
int fs_set_goal();
int fs_init();

int fs_load();
int fs_store();




