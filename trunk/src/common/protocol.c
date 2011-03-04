#include<protocol.h>

EVRPC_GENERATE(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_stat, stat_request, stat_response)
EVRPC_GENERATE(rpc_ls, ls_request, ls_response)

EVRPC_GENERATE(rpc_mknod, mknod_request, mknod_response)
EVRPC_GENERATE(rpc_rm, rm_request, rm_response)
EVRPC_GENERATE(rpc_statfs, statfs_request, statfs_response)
EVRPC_GENERATE(rpc_lookup, lookup_request, lookup_response)

/*EVRPC_GENERATE(rpc_, _request, _response)*/
/*EVRPC_GENERATE(rpc_, _request, _response)*/
