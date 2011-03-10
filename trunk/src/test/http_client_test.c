#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include<event.h>
#include<event2/buffer.h>
#include <evutil.h>
#include <evhttp.h>


#include "http_client.h"
#include "protocol.gen.h"
#include "log.h"





void test_get(){
    char*  p;
    //http://www.baidu.com/search/error.html
    http_response * response = http_get("http://127.0.0.1:6006/get/11", NULL);  // xiaonei.com/home
    printf("%d\n", response -> status_code);

    while(( p = evbuffer_readline(response->headers))){
        printf("%s\n", p);
        free(p);
    }

    while(( p = evbuffer_readline(response->body))){
        printf("%s\n", p);
        free(p);
    }
    http_response_free(response);
}

void test_post(){
    char*  p;
    struct evbuffer * buffer = evbuffer_new();
    evbuffer_add_printf(buffer, "email=idning@gmail.com&password=88888");
    http_response * response = http_post("http://127.0.0.1:6006/put/2",  NULL, buffer);  // xiaonei.com/home
    printf("%d\n", response -> status_code);

    while(( p = evbuffer_readline(response->headers))){
        printf("%s\n", p);
        free(p);
    }

    while(( p = evbuffer_readline(response->body))){
        printf("%s\n", p);
        free(p);
    }
    http_response_free(response);
}


int main(){
    event_init();
    //test_get();
    test_post();
    test_post();
    return 0;
}
