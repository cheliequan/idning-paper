#include <event.h>
#include <evhttp.h>
#include "protocol.gen.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_HEADER(rpc_stat, stat_request, stat_response)
