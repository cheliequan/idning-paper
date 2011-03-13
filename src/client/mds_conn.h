#include "protocol.gen.h"
#include <fuse_lowlevel.h>


void mds_conn_init(void);

int ping_send_request(void);
int ls_send_request(fuse_ino_t ino, struct file_stat * stat_arr);
int stat_send_request(fuse_ino_t * ino_arr, int len, struct file_stat * stat_arr);

int lookup_send_request(fuse_ino_t parent_ino, const char * name, struct file_stat *o_stat );
int mknod_send_request(fuse_ino_t parent_ino, const char * name, int type, int mode, struct file_stat *o_stat );
int unlink_send_request(fuse_ino_t parent_ino, const char * name );
int setattr_send_request(struct file_stat * stat_arr);
