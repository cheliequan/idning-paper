#include "http_client.h"
#include "network.h"

/*#include <event.h>*/
/*#include <evhttp.h>*/


http_response * http_response_new(int status_code, struct evbuffer * headers, struct evbuffer * body){
    http_response * r = (http_response *)malloc(sizeof(http_response));
    r -> status_code = status_code;
    r -> headers = headers;
    r -> body = body;
    return r;
};
void http_response_free(http_response * r){
    evbuffer_free(r -> headers);
    evbuffer_free(r -> body);
    free(r);
}


static struct http_response * http_req(char * ip, int port, char * verb, char * path, char * data);

struct http_response * http_get(char * ip, int port, char * path){
    return http_req(ip, port, "GET", path, "");
}
struct http_response * http_post(char * ip, int port, char * path, char * data){
    return http_req(ip, port, "POST", path, data);
}

static int parse_response_status_code(struct evbuffer * evb){
    char * line = evbuffer_readline(evb);
    int code;
    sscanf(line, "HTTP/%*d.%*d %d", & code);
    free(line);
    return code;
}
/*
 *
    should use evbuffer_free(evb); to free this evbuffer
 * */
static struct http_response * http_req(char * ip, int port, char * verb, char * path, char * data){

    char port_str[6];
    sprintf(port_str, "%d", port);

    int cs = client_socket(ip, port_str);
    char * request_buffer = alloca(100 + strlen(path)); // in stack do not need free
    char response_buffer[1024*8];

    sprintf(request_buffer, "%s %s HTTP/1.0\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n", verb, path, strlen(data), data);
    //sprintf(request_buffer, "GET %s HTTP/1.0\r\nUser-Agent: curl/7.19.7 (i486-pc-linux-gnu) libcurl/7.19.7 OpenSSL/0.9.8k zlib/1.2.3.3 libidn/1.15\r\n\r\n", path);
    fprintf(stderr, "%s", request_buffer);

    int rst = tcptowrite(cs, request_buffer, strlen(request_buffer) , 30000);
    printf("tcptowrite rst: %d", rst);
    int readed = 0;

    struct evbuffer *evb = evbuffer_new();
    /*evbuffer_read(evb, cs, 30);*/

    while ((readed = tcptoread(cs, response_buffer, 1024*8, 30000)) > 0){
        printf("readed len: %d \n", readed);
        evbuffer_add(evb, response_buffer, readed);
    }

    struct evbuffer_ptr evbptr = evbuffer_search(evb, "\r\n\r\n", 4, NULL);

    struct evbuffer *headers = evbuffer_new();
    evbuffer_remove_buffer(evb, headers, evbptr.pos);
    /*evbuffer_drain(evb, evbptr.pos);*/
    tcpclose(cs);
    int code = parse_response_status_code(headers);
    return http_response_new(code, headers, evb);
}

