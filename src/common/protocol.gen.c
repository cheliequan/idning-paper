
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "protocol.gen.h"

int msg_header_pack(struct msg_header * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int msg_header_unpack(struct msg_header * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	return data-orig;
}
char* msg_header_tostring(struct msg_header * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
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
void msg_header_free(msg_header * s){
	free(s);
}
int mc_mkdir_request_pack(struct mc_mkdir_request * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> parent);
	put32bit(&p, s -> namelength);
	if (s->namelength <= 0)
	   s->namelength = strlen(s->name);
	putstr(&p, s -> namelength, s -> name);
	put32bit(&p, s -> mode);
	put32bit(&p, s -> uid);
	put32bit(&p, s -> gid);
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int mc_mkdir_request_unpack(struct mc_mkdir_request * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> parent = get32bit(&data);
	s -> namelength = get32bit(&data);
	char * tmp = getstr(&data, s -> namelength);
	s -> name = strdup(tmp); 
	s -> mode = get32bit(&data);
	s -> uid = get32bit(&data);
	s -> gid = get32bit(&data);
	return data-orig;
}
char* mc_mkdir_request_tostring(struct mc_mkdir_request * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
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
void mc_mkdir_request_free(mc_mkdir_request * s){
	free( s -> name);
	free(s);
}
int mc_mkdir_response_pack(struct mc_mkdir_response * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> result);
	put32bit(&p, s -> inode);
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int mc_mkdir_response_unpack(struct mc_mkdir_response * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> result = get32bit(&data);
	s -> inode = get32bit(&data);
	return data-orig;
}
char* mc_mkdir_response_tostring(struct mc_mkdir_response * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
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
void mc_mkdir_response_free(mc_mkdir_response * s){
	free(s);
}
int oc_request_pack(struct oc_request * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int oc_request_unpack(struct oc_request * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	return data-orig;
}
char* oc_request_tostring(struct oc_request * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
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
void oc_request_free(oc_request * s){
	free(s);
}
int oc_response_pack(struct oc_response * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int oc_response_unpack(struct oc_response * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	return data-orig;
}
char* oc_response_tostring(struct oc_response * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
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
void oc_response_free(oc_response * s){
	free(s);
}
int machine_pack(struct machine * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	put32bit(&p, s -> uuid);
	put32bit(&p, s -> iplength);
	if (s->iplength <= 0)
	   s->iplength = strlen(s->ip);
	putstr(&p, s -> iplength, s -> ip);
	put32bit(&p, s -> port);
	put32bit(&p, s -> type);
	return p-data;
}
int machine_unpack(struct machine * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> uuid = get32bit(&data);
	s -> iplength = get32bit(&data);
	char * tmp = getstr(&data, s -> iplength);
	s -> ip = strdup(tmp); 
	s -> port = get32bit(&data);
	s -> type = get32bit(&data);
	return data-orig;
}
char* machine_tostring(struct machine * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "uuid", s->uuid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "iplength", s->iplength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %s\n", "ip", s->ip);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "port", s->port);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "type", s->type);
	while (*ptr) ptr++;
	return str;
}
machine * machine_new(){
	machine * p = (machine * )malloc (sizeof(machine));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	return p;
}
void machine_free(machine * s){
	free( s -> ip);
	free(s);
}
int ping_pack(struct ping * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> self_iplength);
	if (s->self_iplength <= 0)
	   s->self_iplength = strlen(s->self_ip);
	putstr(&p, s -> self_iplength, s -> self_ip);
	put32bit(&p, s -> self_port);
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int ping_unpack(struct ping * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> self_iplength = get32bit(&data);
	char * tmp = getstr(&data, s -> self_iplength);
	s -> self_ip = strdup(tmp); 
	s -> self_port = get32bit(&data);
	return data-orig;
}
char* ping_tostring(struct ping * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "self_iplength", s->self_iplength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %s\n", "self_ip", s->self_ip);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "self_port", s->self_port);
	while (*ptr) ptr++;
	return str;
}
ping * ping_new(){
	ping * p = (ping * )malloc (sizeof(ping));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_PING ;
	return p;
}
void ping_free(ping * s){
	free( s -> self_ip);
	free(s);
}
int pong_pack(struct pong * s, uint8_t * data, uint32_t len){
	uint8_t * p = data;
	s->msglength = 88888888;
	put32bit(&p, s -> msglength);
	put32bit(&p, s -> msgid);
	put32bit(&p, s -> version);
	put32bit(&p, s -> operation);
	put32bit(&p, s -> machine_arrlength);
	int i;
	for(i=0; i<s->machine_arrlength; i++){
	   int plen = machine_pack(s->machine_arr+i, p, 0);
	   p += plen;
	}
	s->msglength = p-data;
	uint8_t *p2 = data ;
	put32bit_width(&p2, s->msglength, 8);
	return p-data;
}
int pong_unpack(struct pong * s, const uint8_t * data, uint32_t len){
	const uint8_t* orig = data;
	s -> msglength = get32bit(&data);
	s -> msgid = get32bit(&data);
	s -> version = get32bit(&data);
	s -> operation = get32bit(&data);
	s -> machine_arrlength = get32bit(&data);
	int i;
	s->machine_arr = (machine *) malloc (sizeof(machine) * s->machine_arrlength);
	for(i=0; i<s->machine_arrlength; i++){
	   int plen = machine_unpack(s->machine_arr+i, data, 0);
	   data += plen;
	}
	return data-orig;
}
char* pong_tostring(struct pong * s){
	static char str[10240];
	char * ptr = str;
	sprintf(ptr, "\t%s = %d\n", "msglength", s->msglength);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "msgid", s->msgid);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "version", s->version);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "operation", s->operation);
	while (*ptr) ptr++;
	sprintf(ptr, "\t%s = %d\n", "machine_arrlength", s->machine_arrlength);
	while (*ptr) ptr++;
	return str;
}
pong * pong_new(){
	pong * p = (pong * )malloc (sizeof(pong));

    if (p == NULL){
        logging(LOG_ERROR, "TODO: Out of Memory");
        exit(1);
    }
    
	p->operation = MSG_PONG ;
	return p;
}
void pong_free(pong * s){
	free( s -> machine_arr);
	free(s);
}
