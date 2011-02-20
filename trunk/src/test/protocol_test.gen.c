
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
	s1.msglength = 478;
	s1.msgid = 392;
	s1.version = 42;
	s1.operation = 80;
	s1.operation = MSG_MSG_HEADER;
	msg_header_pack(&s1, buffer, 100);
	msg_header_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
}

void mc_mkdir_request_test(){
	mc_mkdir_request s1;
	mc_mkdir_request s2;
	uint8_t buffer [10000];
	s1.msglength = 424;
	s1.msgid = 226;
	s1.version = 500;
	s1.operation = 266;
	s1.parent = 224;
	s1.namelength = 277;
	s1.name = (uint8_t *)randomstring;
	s1.mode = 363;
	s1.uid = 39;
	s1.gid = 60;
	s1.operation = MSG_MC_MKDIR_REQUEST;
	mc_mkdir_request_pack(&s1, buffer, 100);
	mc_mkdir_request_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.parent == s2.parent);
	assert(s1.namelength == s2.namelength);
	assert(strncmp((const char *)s1.name, (const char *)s2.name, s1.namelength) == 0);
	assert(s1.mode == s2.mode);
	assert(s1.uid == s2.uid);
	assert(s1.gid == s2.gid);
}

void mc_mkdir_response_test(){
	mc_mkdir_response s1;
	mc_mkdir_response s2;
	uint8_t buffer [10000];
	s1.msglength = 386;
	s1.msgid = 82;
	s1.version = 127;
	s1.operation = 251;
	s1.result = 43;
	s1.inode = 451;
	s1.operation = MSG_MC_MKDIR_RESPONSE;
	mc_mkdir_response_pack(&s1, buffer, 100);
	mc_mkdir_response_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.result == s2.result);
	assert(s1.inode == s2.inode);
}

void oc_request_test(){
	oc_request s1;
	oc_request s2;
	uint8_t buffer [10000];
	s1.msglength = 209;
	s1.msgid = 298;
	s1.version = 231;
	s1.operation = 17;
	s1.operation = MSG_OC_REQUEST;
	oc_request_pack(&s1, buffer, 100);
	oc_request_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
}

void oc_response_test(){
	oc_response s1;
	oc_response s2;
	uint8_t buffer [10000];
	s1.msglength = 453;
	s1.msgid = 349;
	s1.version = 431;
	s1.operation = 219;
	s1.operation = MSG_OC_RESPONSE;
	oc_response_pack(&s1, buffer, 100);
	oc_response_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
}

void machine_test(){
	machine s1;
	machine s2;
	uint8_t buffer [10000];
	s1.uuid = 139;
	s1.iplength = 239;
	s1.name = (uint8_t *)randomstring;
	s1.port = 108;
	s1.type = 437;
	s1.operation = MSG_MACHINE;
	machine_pack(&s1, buffer, 100);
	machine_unpack(&s2, buffer, 100);
	assert(s1.uuid == s2.uuid);
	assert(s1.iplength == s2.iplength);
	assert(strncmp((const char *)s1.ip, (const char *)s2.ip, s1.iplength) == 0);
	assert(s1.port == s2.port);
	assert(s1.type == s2.type);
}

void ping_test(){
	ping s1;
	ping s2;
	uint8_t buffer [10000];
	s1.msglength = 435;
	s1.msgid = 212;
	s1.version = 329;
	s1.operation = 426;
	s1.self_iplength = 215;
	s1.name = (uint8_t *)randomstring;
	s1.self_port = 182;
	s1.operation = MSG_PING;
	ping_pack(&s1, buffer, 100);
	ping_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.self_iplength == s2.self_iplength);
	assert(strncmp((const char *)s1.self_ip, (const char *)s2.self_ip, s1.self_iplength) == 0);
	assert(s1.self_port == s2.self_port);
}

void pong_test(){
	pong s1;
	pong s2;
	uint8_t buffer [10000];
	s1.msglength = 371;
	s1.msgid = 438;
	s1.version = 340;
	s1.operation = 480;
	s1.machine_arrlength = 118;
	s1.operation = MSG_PONG;
	pong_pack(&s1, buffer, 100);
	pong_unpack(&s2, buffer, 100);
	assert(s1.msglength == s2.msglength);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.machine_arrlength == s2.machine_arrlength);
}

int main(){
	msg_header_test();
	mc_mkdir_request_test();
	mc_mkdir_response_test();
	oc_request_test();
	oc_response_test();
	machine_test();
	ping_test();
	pong_test();
	return 0;
}

