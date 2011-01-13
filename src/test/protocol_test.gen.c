
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "test.h"
char * randomstring="01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890"; 
void msg_header_test(){
	msg_header s1;
	msg_header s2;
	uint8_t buffer [10000];
	s1.msgid = 417;
	s1.version = 455;
	s1.operation = 226;
	s1.msglength = 362;
	s1.operation = MSG_MSG_HEADER;
	msg_header_pack(&s1, buffer, 100);
	msg_header_unpack(&s2, buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
}

void mc_mkdir_request_test(){
	mc_mkdir_request s1;
	mc_mkdir_request s2;
	uint8_t buffer [10000];
	s1.msgid = 315;
	s1.version = 292;
	s1.operation = 440;
	s1.msglength = 415;
	s1.parent = 153;
	s1.namelength = 320;
	s1.name = (uint8_t *)randomstring;
	s1.mode = 308;
	s1.uid = 296;
	s1.gid = 100;
	s1.operation = MSG_MC_MKDIR_REQUEST;
	mc_mkdir_request_pack(&s1, buffer, 100);
	mc_mkdir_request_unpack(&s2, buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
	assert(s1.parent == s2.parent);
	assert(s1.namelength == s2.namelength);
	assert(strncmp((const char *)s1.name, (const char *)s2.name, s1.namelength) == 0);
	assert(s1.mode == s2.mode);
	assert(s1.uid == s2.uid);
	assert(s1.gid == s2.gid);
}

void oc_request_test(){
	oc_request s1;
	oc_request s2;
	uint8_t buffer [10000];
	s1.msgid = 435;
	s1.version = 441;
	s1.operation = 484;
	s1.msglength = 305;
	s1.operation = MSG_OC_REQUEST;
	oc_request_pack(&s1, buffer, 100);
	oc_request_unpack(&s2, buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
}

void oc_response_test(){
	oc_response s1;
	oc_response s2;
	uint8_t buffer [10000];
	s1.msgid = 228;
	s1.version = 35;
	s1.operation = 433;
	s1.msglength = 28;
	s1.operation = MSG_OC_RESPONSE;
	oc_response_pack(&s1, buffer, 100);
	oc_response_unpack(&s2, buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
}

int main(){
	msg_header_test();
	mc_mkdir_request_test();
	oc_request_test();
	oc_response_test();
	return 0;
}

