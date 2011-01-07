#include "protocol.h"

void mc_mkdir_request_pack(struct mc_mkdir_request * req, uint8_t * data, uint32_t len){
    put32bit(&data, req -> msgid);
    put32bit(&data, req -> version);
    put32bit(&data, req -> operation);
    put32bit(&data, req -> msglength);

    put32bit(&data, req -> parent);
    put32bit(&data, req -> namelength);
    putstr(&data, req->namelength, req->name);
    put32bit(&data, req -> mode);
    put32bit(&data, req -> uid);
    put32bit(&data, req -> gid);
}

void mc_mkdir_request_unpack(struct mc_mkdir_request * req, const uint8_t * data, uint32_t len){
    req -> msgid = get32bit(&data);
    req -> version = get32bit(&data);
    req -> operation = get32bit(&data);
    req -> msglength = get32bit(&data);

    req -> parent = get32bit(&data);
    req -> namelength = get32bit(&data);
    getstr(&data, req->namelength, &(req->name));
    req -> mode = get32bit(&data);
    req -> uid = get32bit(&data);
    req -> gid = get32bit(&data);
}

void oc_request_pack(struct oc_request * req, uint8_t * data, uint32_t len){

}
void oc_request_unpack(struct oc_request * req, uint8_t * data, uint32_t len){

}
