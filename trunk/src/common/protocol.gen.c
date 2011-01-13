
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "protocol.gen.h"

int msg_header_pack(struct msg_header * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	return p-data;
}
void msg_header_unpack(struct msg_header * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
}
char* msg_header_tostring(struct msg_header * s){
	static char str[1024];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	return str;
}
int mc_mkdir_request_pack(struct mc_mkdir_request * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> parent);
	put32bit(&p, s -> namelength);
	putstr(&p, s -> namelength, s -> name);
	put32bit(&p, s -> mode);
	put32bit(&p, s -> uid);
	put32bit(&p, s -> gid);
	return p-data;
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
char* mc_mkdir_request_tostring(struct mc_mkdir_request * s){
	static char str[1024];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "parent", s->parent);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "namelength", s->namelength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %s\n", "name", s->name);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "mode", s->mode);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "uid", s->uid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "gid", s->gid);
	while (*ptr) ptr++;
	return str;
}
int oc_request_pack(struct oc_request * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	return p-data;
}
void oc_request_unpack(struct oc_request * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
}
char* oc_request_tostring(struct oc_request * s){
	static char str[1024];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	return str;
}
int oc_response_pack(struct oc_response * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	return p-data;
}
void oc_response_unpack(struct oc_response * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
}
char* oc_response_tostring(struct oc_response * s){
	static char str[1024];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	return str;
}
