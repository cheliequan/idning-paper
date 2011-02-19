#include "datapack2.h"
#include "log.h"
#include <string.h>
#include <stdio.h>

// //commnet is free in this file 
// /* */ commment has special mean

/*message header for all messages!*/
typedef struct msg_header{
    uint32_t msglength;
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
}msg_header;

/*master<-client request*/
typedef struct mc_mkdir_request{
    uint32_t msglength;
    uint32_t msgid; //abc
    uint32_t version;
    uint32_t operation;

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
    uint32_t msglength;
    uint32_t msgid; // 
    uint32_t version;
    uint32_t operation;

    uint32_t result;
    uint32_t inode;

}mc_mkdir_response;


/*osd<-client request*/
typedef struct oc_request{
    uint32_t msglength;
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    /*--*/
    void (*pack) (struct oc_request * req, uint8_t * data, uint32_t len) ;
    void (*unpack)(struct oc_request * req, uint8_t * data, uint32_t len);
}oc_request;

/*osd<-client response*/
typedef struct oc_response{
    uint32_t msglength;
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
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
    uint32_t msglength;
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;

    uint32_t self_iplength; //self listen ip & port
    uint8_t* self_ip;
    uint32_t self_port;
}ping;

/*operation = MSG_PONG*/
typedef struct pong{
    uint32_t msglength;
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;

    uint32_t machine_arrlength;
    /*arr*/ machine* machine_arr;
}pong;

//void oc_request_pack(struct oc_request * req, uint8_t * data, uint32_t len);
//void oc_request_unpack(struct oc_request * req, uint8_t * data, uint32_t len);

//void mc_mkdir_request_pack(struct mc_mkdir_request * req, uint8_t * data, uint32_t len);
//void mc_mkdir_request_unpack(struct mc_mkdir_request * req, const uint8_t * data, uint32_t len);

/*below_is_auto_generated*/



