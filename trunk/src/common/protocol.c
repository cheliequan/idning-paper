#include<protocol.h>

EVRPC_GENERATE(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_stat, stat_request, stat_response)
EVRPC_GENERATE(rpc_ls, ls_request, ls_response)
