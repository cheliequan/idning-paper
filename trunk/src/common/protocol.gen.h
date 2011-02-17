#include "datapack.h"
#include "log.h"
#include <string.h>
#include <stdio.h>

// //commnet is free in this file 
// /* */ commment has special mean

/*message header for all messages!*/
typedef struct msg_header{
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;
}msg_header;

/*master<-client request*/
typedef struct mc_mkdir_request{
    uint32_t msgid; //abc
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;

    uint32_t parent;
    uint32_t namelength;
    uint8_t* name;
    uint32_t mode;
    uint32_t uid;
    uint32_t gid;

    /*-- for protocol_gen*/
    void (*pack)  (struct mc_mkdir_request * req, uint8_t * data, uint32_t len) ;
    void (*unpack)(struct mc_mkdir_request * req, uint8_t * data, uint32_t len);
}mc_mkdir_request;

/*master<-client request*/
typedef struct mc_mkdir_response{
    uint32_t msgid; // 
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;

    uint32_t result;
    uint32_t inode;

}mc_mkdir_response;


/*osd<-client request*/
typedef struct oc_request{
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;
    /*--*/
    void (*pack) (struct oc_request * req, uint8_t * data, uint32_t len) ;
    void (*unpack)(struct oc_request * req, uint8_t * data, uint32_t len);
}oc_request;

/*osd<-client response*/
typedef struct oc_response{
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;
}oc_response;

typedef struct machine{
    uint32_t uuid;
    uint32_t iplength;
    uint8_t* ip;
    uint32_t port;
    uint32_t type;
}machine;

/*operation = MSG_PING*/
typedef struct ping{
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;
}ping;

/*operation = MSG_PONG*/
typedef struct pong{
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;

    uint32_t machine_arrlength;
    /*arr*/ machine* machine_arr;
}pong;

//void oc_request_pack(struct oc_request * req, uint8_t * data, uint32_t len);
//void oc_request_unpack(struct oc_request * req, uint8_t * data, uint32_t len);

//void mc_mkdir_request_pack(struct mc_mkdir_request * req, uint8_t * data, uint32_t len);
//void mc_mkdir_request_unpack(struct mc_mkdir_request * req, const uint8_t * data, uint32_t len);

/*below_is_auto_generated*/



int msg_header_pack(struct msg_header * s, uint8_t * data, uint32_t len);
int msg_header_unpack(struct msg_header * s, const uint8_t * data, uint32_t len);
char* msg_header_tostring(struct msg_header * s);
msg_header * msg_header_new();

int mc_mkdir_request_pack(struct mc_mkdir_request * s, uint8_t * data, uint32_t len);
int mc_mkdir_request_unpack(struct mc_mkdir_request * s, const uint8_t * data, uint32_t len);
char* mc_mkdir_request_tostring(struct mc_mkdir_request * s);
mc_mkdir_request * mc_mkdir_request_new();

int mc_mkdir_response_pack(struct mc_mkdir_response * s, uint8_t * data, uint32_t len);
int mc_mkdir_response_unpack(struct mc_mkdir_response * s, const uint8_t * data, uint32_t len);
char* mc_mkdir_response_tostring(struct mc_mkdir_response * s);
mc_mkdir_response * mc_mkdir_response_new();

int oc_request_pack(struct oc_request * s, uint8_t * data, uint32_t len);
int oc_request_unpack(struct oc_request * s, const uint8_t * data, uint32_t len);
char* oc_request_tostring(struct oc_request * s);
oc_request * oc_request_new();

int oc_response_pack(struct oc_response * s, uint8_t * data, uint32_t len);
int oc_response_unpack(struct oc_response * s, const uint8_t * data, uint32_t len);
char* oc_response_tostring(struct oc_response * s);
oc_response * oc_response_new();

int machine_pack(struct machine * s, uint8_t * data, uint32_t len);
int machine_unpack(struct machine * s, const uint8_t * data, uint32_t len);
char* machine_tostring(struct machine * s);
machine * machine_new();

int ping_pack(struct ping * s, uint8_t * data, uint32_t len);
int ping_unpack(struct ping * s, const uint8_t * data, uint32_t len);
char* ping_tostring(struct ping * s);
ping * ping_new();

int pong_pack(struct pong * s, uint8_t * data, uint32_t len);
int pong_unpack(struct pong * s, const uint8_t * data, uint32_t len);
char* pong_tostring(struct pong * s);
pong * pong_new();


#define MSG_MSG_HEADER 100
#define MSG_MC_MKDIR_REQUEST 101
#define MSG_MC_MKDIR_RESPONSE 102
#define MSG_OC_REQUEST 103
#define MSG_OC_RESPONSE 104
#define MSG_MACHINE 105
#define MSG_PING 106
#define MSG_PONG 107
