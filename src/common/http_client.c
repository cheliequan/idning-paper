#include "http_client.h"
#include "network.h"

/*#include <event.h>*/
/*#include <evhttp.h>*/


static struct evbuffer * http_req(char * ip, int port, char * verb, char * path, char * data);

struct evbuffer * http_get(char * ip, int port, char * path){
    return http_req(ip, port, "GET", path, "");
}
struct evbuffer * http_post(char * ip, int port, char * path, char * data){
    return http_req(ip, port, "POST", path, data);
}

/*
 *
    should use evbuffer_free(evb); to free this evbuffer
 * */
static struct evbuffer * http_req(char * ip, int port, char * verb, char * path, char * data){

    char port_str[6];
    sprintf(port_str, "%d", port);

    int cs = client_socket(ip, port_str);
    char * request_buffer = alloca(100 + strlen(path)); // in stack do not need free
    char response_buffer[1024*8];

    sprintf(request_buffer, "%s %s HTTP/1.0\r\n\r\n%s", verb, path, data);
    //sprintf(request_buffer, "GET %s HTTP/1.0\r\nUser-Agent: curl/7.19.7 (i486-pc-linux-gnu) libcurl/7.19.7 OpenSSL/0.9.8k zlib/1.2.3.3 libidn/1.15\r\n\r\n", path);
    printf(request_buffer);

    int rst = tcptowrite(cs, request_buffer, strlen(request_buffer) , 30000);
    printf("tcptowrite rst: %d", rst);
    int readed = 0;

    struct evbuffer *evb = evbuffer_new();
    /*evbuffer_read(evb, cs, 30);*/

    while ((readed = tcptoread(cs, response_buffer, 1024*8, 30000)) > 0){
        printf("readed len: %d \n", readed);
        evbuffer_add(evb, response_buffer, readed);
    }
    printf("readed out of where is : %d \n", readed);
    struct evbuffer_ptr evbptr = evbuffer_search(evb, "\r\n\r\n", 4, NULL);
    evbuffer_drain(evb, evbptr.pos);
    tcpclose(cs);
    return evb;
    return 0;
}

