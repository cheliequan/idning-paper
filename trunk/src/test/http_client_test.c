#include<stdio.h>
#include<unistd.h>
#include<assert.h>

#include "http_client.h"
#include "log.h"






int main(){
    char*  p;
    //http://www.baidu.com/search/error.html
    http_response * response = http_get("220.181.111.85", 80, "/");  // xiaonei.com/home
    printf("%d\n", response -> status_code);

    while( p = evbuffer_readline(response->headers)){
        printf("%s\n", p);
        free(p);
    }

    while( p = evbuffer_readline(response->body)){
        printf("%s\n", p);
        free(p);
    }


    http_response_free(response);

}


