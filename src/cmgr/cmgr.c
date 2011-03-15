#include <event.h>
#include <evhttp.h>
#include <string.h>
#include <stdlib.h>

#include "protocol.gen.h"
#include "protocol.h"
#include "log.h"
#include "app.h"
#include "cluster.h"


static void
rpc_setup()
{
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    char *listen_host = cfg_getstr("CMGR_LISTEN_HOST","*");
    int port = cfg_getint32("CMGR_LISTEN_PORT", 9000);

    http = evhttp_start(listen_host, port);
    /*http = evhttp_start("192.168.1.102", port);*/
    if (!http){
        perror("can't start server!");
        exit(-1);
    }
    printf("Start cmgr at %s:%d\n", listen_host, port);

    base = evrpc_init(http);

    EVRPC_REGISTER(base, rpc_ping, ping, pong, ping_handler, NULL);

}

void usage(const char* appname) {
    
}

int main(int argc, char ** argv)
{
    event_init();
    init_app(argc, argv, "cmgr");

    rpc_setup();
    event_dispatch();
    return 0;
}



