#ifndef _MDS_CONN_H__
#define _MDS_CONN_H__

#include "sfs_common.h"

void mds_conn_init(void);

int ls_send_request(char *ip, int port, uint64_t ino,
                    struct file_stat ***o_stat_arr, int *o_cnt);

int stat_send_request(char *ip, int port, uint64_t * ino_arr, int len,
                      struct file_stat *stat_arr);

int lookup_send_request(char *ip, int port, uint64_t parent_ino,
                        const char *name, struct file_stat *o_stat);


int mknod_send_request(char *ip, int port,
                       uint64_t parent_ino, uint64_t ino, 
                       const char *name, int type,
                       int mode, struct file_stat *o_stat);

int symlink_send_request(char *ip, int port, uint64_t parent_ino, uint64_t ino, 
                         const char *name, const char *path,
                         struct file_stat *o_stat);

int unlink_send_request(char *ip, int port, uint64_t parent_ino,
                        const char *name);
int setattr_send_request(char *ip, int port, struct file_stat *stat_arr);

int statfs_send_request(char *ip, int port, int *total_space, int *avail_space,
                        int *inode_cnt);
const char *readlink_send_request(char *ip, int port, uint64_t ino);

int mkfs_send_request(int mds1, int mds2);

#endif
