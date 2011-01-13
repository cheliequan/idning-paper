#include<stdio.h>
#include<unistd.h>
#include<assert.h>

#include "network.h"
#include "log.h"
#include "protocol.gen.h"


struct mc_mkdir_request * create_mkdir_request(){
    mc_mkdir_request * req = (mc_mkdir_request * )malloc (sizeof(mc_mkdir_request));
    req->msgid = 1;
    req->version = 2;
    req->operation = MSG_MC_MKDIR_REQUEST;
    req->msglength = 20;

    req->parent = 88;
    req->namelength = 3;
    req->name = (uint8_t* )"ab";
    req->mode = 87;
    req->uid = 502;
    req->gid = 520;
    return req;
}

int client(){
    int cs = client_socket("127.0.0.1", "9991");
    uint8_t buffer [10000];
    mc_mkdir_request * req = create_mkdir_request();
    int len = mc_mkdir_request_pack(req, buffer, 10000);
    printf("tcptowrite %d", len);

    if (tcptowrite(cs, buffer, len, 1000) <= 0)
        perror("error on tcptowrite");

    /*printf("write ok %d \n", n);*/
    tcpclose(cs);
    return 0;
}


int main(){
    return client();
}


