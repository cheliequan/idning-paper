
#include <evutil.h>
#include <event.h>

typedef struct http_response{
    int status_code;
    struct evbuffer * headers;
    struct evbuffer * body;
}http_response;

void http_client_init();
void http_response_free(http_response * r);

struct http_response *http_get(const char *url,  struct evkeyvalq* headers);
struct http_response *http_post(const char *url, struct evkeyvalq* headers, struct evbuffer * postdata);



/**this should be in http.h , but sometimes it's not there
 */
struct evhttp_uri {
    char *scheme; /* scheme; e.g http, ftp etc */
    char *userinfo; /* userinfo (typically username:pass), or NULL */
    char *host; /* hostname, IP address, or NULL */
    int  port; /* port, or zero */
    char *path; /* path, or "". */
    char *query; /* query, or NULL */
    char *fragment; /* fragment or NULL */
};