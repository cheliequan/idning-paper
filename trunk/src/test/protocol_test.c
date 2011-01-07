#include "test.h"

int main(){

    mc_mkdir_request req ;
    mc_mkdir_request req2 ;
    req.msgid = 1;
    req.version = 2;
    req.operation = 3;
    req.msglength = 20;

    req.parent = 88;
    req.namelenght = 3;
    req.name = "ab";
    req.mode = 87;
    req.uid = 502;
    req.gid = 520;
    uint8_t buffer [10000];
    mc_mkdir_request_pack(&req, &buffer, 100);
    mc_mkdir_request_unpack(&req2, &buffer, 100);
    assert(req.msgid == req2.msgid);
    assert(req.version == req2.version);
    assert(req.msglength == req2.msglength);
    assert(req.namelenght == req2.namelenght);
    assert(req.gid == req2.gid);
    assert(strcmp(req.name, req2.name) == 0);
    
    printf("req.msgid: %d\n", req.msgid);
    printf("req2.msgid: %d\n", req2.msgid);

    printf("req.name: %s\n", req.name);
    printf("req2.name: %s\n", req2.name);
}

