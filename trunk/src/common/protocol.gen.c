
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
	s->msglength = p-data;
	uint8_t *p2 = data + 12;
	put32bit(&p2, s->msglength);
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
msg_header * msg_header_new(){
	msg_header * p = (msg_header * )malloc (sizeof(msg_header));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_MSG_HEADER ;
	return p;
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
	s->msglength = p-data;
	uint8_t *p2 = data + 12;
	put32bit(&p2, s->msglength);
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
mc_mkdir_request * mc_mkdir_request_new(){
	mc_mkdir_request * p = (mc_mkdir_request * )malloc (sizeof(mc_mkdir_request));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_MC_MKDIR_REQUEST ;
	return p;
}
int mc_mkdir_response_pack(struct mc_mkdir_response * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> result);
	put32bit(&p, s -> inode);
	s->msglength = p-data;
	uint8_t *p2 = data + 12;
	put32bit(&p2, s->msglength);
	return p-data;
}
void mc_mkdir_response_unpack(struct mc_mkdir_response * s, const uint8_t * data, uint32_t len){
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> msglength = get32bit(&data);
	s -> result = get32bit(&data);
	s -> inode = get32bit(&data);
}
char* mc_mkdir_response_tostring(struct mc_mkdir_response * s){
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
	sprintf(ptr, "\t%s = %d\n", "result", s->result);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "inode", s->inode);
	while (*ptr) ptr++;
	return str;
}
mc_mkdir_response * mc_mkdir_response_new(){
	mc_mkdir_response * p = (mc_mkdir_response * )malloc (sizeof(mc_mkdir_response));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_MC_MKDIR_RESPONSE ;
	return p;
}
int oc_request_pack(struct oc_request * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	s->msglength = p-data;
	uint8_t *p2 = data + 12;
	put32bit(&p2, s->msglength);
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
oc_request * oc_request_new(){
	oc_request * p = (oc_request * )malloc (sizeof(oc_request));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_OC_REQUEST ;
	return p;
}
int oc_response_pack(struct oc_response * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> msglength);
	s->msglength = p-data;
	uint8_t *p2 = data + 12;
	put32bit(&p2, s->msglength);
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
oc_response * oc_response_new(){
	oc_response * p = (oc_response * )malloc (sizeof(oc_response));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_OC_RESPONSE ;
	return p;
}
