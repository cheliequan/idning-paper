
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "test.h"
char * randomstring="01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890"; 
void mc_mkdir_request_test(){
	mc_mkdir_request s1;
	mc_mkdir_request s2;
	uint8_t buffer [10000];
	s1.msgid = 204;
	s1.version = 354;
	s1.operation = 188;
	s1.msglength = 327;
	s1.parent = 459;
	s1.namelength = 155;
	s1.name = randomstring;
	s1.mode = 86;
	s1.uid = 308;
	s1.gid = 358;
	mc_mkdir_request_pack(&s1, &buffer, 100);
	mc_mkdir_request_unpack(&s2, &buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
	assert(s1.parent == s2.parent);
	assert(s1.namelength == s2.namelength);
	assert(strncmp(s1.name, s2.name, s1.namelength) == 0);
	assert(s1.mode == s2.mode);
	assert(s1.uid == s2.uid);
	assert(s1.gid == s2.gid);
}

void oc_request_test(){
	oc_request s1;
	oc_request s2;
	uint8_t buffer [10000];
	s1.msgid = 236;
	s1.version = 355;
	s1.operation = 48;
	s1.msglength = 335;
	oc_request_pack(&s1, &buffer, 100);
	oc_request_unpack(&s2, &buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
}

void oc_response_test(){
	oc_response s1;
	oc_response s2;
	uint8_t buffer [10000];
	s1.msgid = 405;
	s1.version = 384;
	s1.operation = 331;
	s1.msglength = 295;
	oc_response_pack(&s1, &buffer, 100);
	oc_response_unpack(&s2, &buffer, 100);
	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
}

int main(){
	mc_mkdir_request_test();
	oc_request_test();
	oc_response_test();
}

