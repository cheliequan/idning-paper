#include "protocol.gen.h"

void mds_conn_init(void);
int stat_send_request(int * ino_arr, int len, struct file_stat * );

int ping_send_request(void);
int ls_send_request(int ino, struct file_stat * stat_arr);
