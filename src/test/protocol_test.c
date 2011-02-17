
/*
this file is auto generage by ./scripts/protocol_gen.py 
use common/protocol.input.h as input.
do not modify it directly
*/
#include "test.h"
char * randomstring="01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890 01234567890"; 

void pong_test(){
	pong s1;
	pong s2;
	uint8_t buffer [10000];
	s1.msgid = 179;
	s1.version = 236;
	s1.operation = 133;
	s1.msglength = 104;
	s1.machine_arrlength = 3;
	s1.operation = MSG_PONG;


    s1.machine_arr = (machine *)malloc(3* sizeof(machine));
 
	s1.machine_arr[0].uuid = 8;
	s1.machine_arr[0].iplength = 8;
	s1.machine_arr[0].ip = "1.1.1.1";
	s1.machine_arr[0].port = 8888;
	s1.machine_arr[0].type = 0;

	s1.machine_arr[1].uuid = 8;
	s1.machine_arr[1].iplength = 8;
	s1.machine_arr[1].ip = "2.2.2.2";
	s1.machine_arr[1].port = 8888;
	s1.machine_arr[1].type = 0;

	s1.machine_arr[2].uuid = 8;
	s1.machine_arr[2].iplength = 8;
	s1.machine_arr[2].ip = "3.3.3.3";
	s1.machine_arr[2].port = 8888;
	s1.machine_arr[2].type = 0;

	int i = pong_pack(&s1, buffer, 100);

    fprintf(stderr, "sizeof(machine)= %d\n", sizeof(machine));
    fprintf(stderr, "sizeof(pong)= %d\n", sizeof(pong));

    fprintf(stderr, "buffer size = %d\n", i);
    fprintf(stderr, "%s\n", buffer);
    fprintf(stderr, "%d\n", buffer[0]);

	pong_unpack(&s2, buffer, 100);

	assert(s1.msgid == s2.msgid);
	assert(s1.version == s2.version);
	assert(s1.operation == s2.operation);
	assert(s1.msglength == s2.msglength);
	assert(s1.machine_arrlength == s2.machine_arrlength);



	assert(s1.machine_arr[0].uuid == s2.machine_arr[0].uuid);
	assert(0 == strcmp(s1.machine_arr[0].ip ,s2.machine_arr[0].ip));
	assert(0 == strcmp(s1.machine_arr[1].ip ,s2.machine_arr[1].ip));
	assert(0 == strcmp(s1.machine_arr[2].ip ,s2.machine_arr[2].ip));
    printf("%s", s1.machine_arr[2].ip );
}

int main(){
	pong_test();
	return 0;
}

