#include <event.h>
#include <evhttp.h>
#include "protocol.gen.h"

#ifndef _PROTOCOL_H__
#define _PROTOCOL_H__

    EVRPC_HEADER(rpc_ping, ping, pong)
    EVRPC_HEADER(rpc_stat, stat_request, stat_response)
    EVRPC_HEADER(rpc_ls, ls_request, ls_response)

    EVRPC_HEADER(rpc_mknod, mknod_request, mknod_response)
    EVRPC_HEADER(rpc_unlink, unlink_request, unlink_response)

    EVRPC_HEADER(rpc_statfs, statfs_request, statfs_response)

    EVRPC_HEADER(rpc_lookup, lookup_request, lookup_response)
    EVRPC_HEADER(rpc_setattr, setattr_request, setattr_response)
    EVRPC_HEADER(rpc_mkfs, mkfs_request, mkfs_response)
#endif
