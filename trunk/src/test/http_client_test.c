#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "http_client.h"
#include "protocol.gen.h"
#include "log.h"





void test_get(){
    char*  p;
    //http://www.baidu.com/search/error.html
    http_response * response = http_get("220.181.111.85", 80, "/");  // xiaonei.com/home
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
    http_response * response = http_post("123.125.44.242", 80, "/Plogin.do", 
            "email=idning@gmail.com&password=88888");  // xiaonei.com/home
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
    test_get();
    test_post();
    return 0;
}
