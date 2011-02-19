#include<stdio.h>
#include<unistd.h>
#include<assert.h>

#include "http_client.h"
#include "log.h"






int main(){
    //http://www.baidu.com/search/error.html
    struct evbuffer * evb = http_get("220.181.111.85", 80, "/");  // xiaonei.com/home
    char*  p;
    while( p = evbuffer_readline(evb))
        printf("%s\n", p);


    evbuffer_free(evb);

}


