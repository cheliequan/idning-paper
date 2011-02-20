
#include <evutil.h>
#include <event.h>

typedef struct http_response{
    int status_code;
    struct evbuffer * headers;
    struct evbuffer * body;
}http_response;

struct http_response * http_get(char * ip, int port, char * path);
struct http_response * http_post(char * ip, int port, char * path, char * data);
