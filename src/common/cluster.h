
#include "protocol.gen.h"
#include "protocol.h"


#ifndef _CLUSTER_H__
#define _CLUSTER_H__

void ping_handler(EVRPC_STRUCT(rpc_ping)* rpc, void *arg);
int ping_send_request(struct evrpc_pool * pool);

#endif
