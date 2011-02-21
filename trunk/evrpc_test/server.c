#include <event.h>
#include <evhttp.h>
#include "test.gen.h"

EVRPC_HEADER(rpc_ping, ping, pong)
EVRPC_GENERATE(rpc_ping, ping, pong)


static struct evhttp *
http_setup(ev_uint16_t *pport)
{
    struct evhttp *myhttp;
    ev_uint16_t port;
    struct evhttp_bound_socket *sock;

    myhttp = evhttp_new(NULL);
    if (!myhttp)
        event_errx(1, "Could not start web server");

    /* Try a few different ports */
    sock = evhttp_bind_socket_with_handle(myhttp, "127.0.0.1", 0);
    if (!sock)
        event_errx(1, "Couldn't open web port");

    port = regress_get_socket_port(evhttp_bound_socket_get_fd(sock));

    *pport = port;
    return (myhttp);
}

static void
ping_cb(EVRPC_STRUCT(rpc_ping)* rpc, void *arg)
{
    /*struct kill* kill_reply = rpc->reply;*/

    /*if (need_input_hook) {*/
        /*struct evhttp_request* req = EVRPC_REQUEST_HTTP(rpc);*/
        /*const char *header = evhttp_find_header(*/
            /*req->input_headers, "X-Hook");*/
        /*assert(strcmp(header, "input") == 0);*/
    /*}*/

    /*[> we just want to fill in some non-sense <]*/
    /*EVTAG_ASSIGN(kill_reply, weapon, "dagger");*/
    /*EVTAG_ASSIGN(kill_reply, action, "wave around like an idiot");*/

    /*[> no reply to the RPC <]*/
    /*EVRPC_REQUEST_DONE(rpc);*/
}

static void
rpc_setup(struct evhttp **phttp, ev_uint16_t *pport, struct evrpc_base **pbase)
{
    ev_uint16_t port;
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;

    http = http_setup(&port);
    base = evrpc_init(http);

    EVRPC_REGISTER(base, rpc_ping, ping, ping, ping_cb, NULL);

    *phttp = http;
    *pport = port;
    *pbase = base;

}

static void
rpc_basic_client(void)
{
    ev_uint16_t port;
    struct evhttp *http = NULL;
    struct evrpc_base *base = NULL;
    struct evrpc_pool *pool = NULL;
    struct msg *msg = NULL;
    struct kill *kill = NULL;

    rpc_setup(&http, &port, &base);

    assert(evrpc_add_hook(base, EVRPC_INPUT, rpc_hook_add_header, (void*)"input")
        != NULL);
    assert(evrpc_add_hook(base, EVRPC_OUTPUT, rpc_hook_add_header, (void*)"output")
        != NULL);

    pool = rpc_pool_with_connection(port);

    assert(evrpc_add_hook(pool, EVRPC_OUTPUT, rpc_hook_add_meta, NULL));
    assert(evrpc_add_hook(pool, EVRPC_INPUT, rpc_hook_remove_header, (void*)"output"));

    /* set up the basic message */
    msg = msg_new();
    EVTAG_ASSIGN(msg, from_name, "niels");
    EVTAG_ASSIGN(msg, to_name, "tester");

    kill = kill_new();

    EVRPC_MAKE_REQUEST(Message, pool, msg, kill,  GotKillCb, NULL);

    test_ok = 0;

    event_dispatch();

    tt_assert(test_ok == 1);

    /* we do it twice to make sure that reuse works correctly */
    kill_clear(kill);

    EVRPC_MAKE_REQUEST(Message, pool, msg, kill,  GotKillCb, NULL);

    event_dispatch();

    tt_assert(test_ok == 2);

    /* we do it trice to make sure other stuff works, too */
    kill_clear(kill);

    {
        struct evrpc_request_wrapper *ctx =
            EVRPC_MAKE_CTX(Message, msg, kill,
            pool, msg, kill, GotKillCb, NULL);
        evrpc_make_request(ctx);
    }

    event_dispatch();

    rpc_teardown(base);

    tt_assert(test_ok == 3);

end:
    if (msg)
        msg_free(msg);
    if (kill)
        kill_free(kill);

    if (pool)
        evrpc_pool_free(pool);
    if (http)
        evhttp_free(http);

    need_input_hook = 0;
    need_output_hook = 0;
}


