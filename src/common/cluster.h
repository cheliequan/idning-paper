#include "protocol.gen.h"
#include "protocol.h"

#ifndef _CLUSTER_H__
#define _CLUSTER_H__

#define MACHINE_CMGR    1
#define MACHINE_MDS     2
#define MACHINE_OSD     3
#define MACHINE_CLIENT  4

void ping_handler(EVRPC_STRUCT(rpc_ping) * rpc, void *arg);
struct machine *cluster_get_machine_of_type(int type);
int select_osd();

int ping_send_request();
void rpc_client_setup(char *self_host, int self_port, int self_type);
struct machine *cluster_get_machine_by_mid(int mid);

#endif
