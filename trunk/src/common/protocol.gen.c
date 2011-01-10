
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "protocol.gen.h"

void msg_header_pack(struct msg_header * s, uint8_t * data, uint32_t len){
	put32bit(&data, s -> msgid);
	put32bit(&data, s -> version);
	put32bit(&data, s -> operation);
	put32bit(&data, s -> msglength);
}
void msg_header_unpack(struct msg_header * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
}
void mc_mkdir_request_pack(struct mc_mkdir_request * s, uint8_t * data, uint32_t len){
	put32bit(&data, s -> msgid);
	put32bit(&data, s -> version);
	put32bit(&data, s -> operation);
	put32bit(&data, s -> msglength);
	put32bit(&data, s -> parent);
	put32bit(&data, s -> namelength);
	putstr(&data, s -> namelength, s -> name);
	put32bit(&data, s -> mode);
	put32bit(&data, s -> uid);
	put32bit(&data, s -> gid);
}
void mc_mkdir_request_unpack(struct mc_mkdir_request * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
	s -> parent = get32bit(&data);
	s -> namelength = get32bit(&data);
	getstr(&data, s -> namelength, &(s -> name));
	s -> mode = get32bit(&data);
	s -> uid = get32bit(&data);
	s -> gid = get32bit(&data);
}
void oc_request_pack(struct oc_request * s, uint8_t * data, uint32_t len){
	put32bit(&data, s -> msgid);
	put32bit(&data, s -> version);
	put32bit(&data, s -> operation);
	put32bit(&data, s -> msglength);
}
void oc_request_unpack(struct oc_request * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
}
void oc_response_pack(struct oc_response * s, uint8_t * data, uint32_t len){
	put32bit(&data, s -> msgid);
	put32bit(&data, s -> version);
	put32bit(&data, s -> operation);
	put32bit(&data, s -> msglength);
}
void oc_response_unpack(struct oc_response * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
}
