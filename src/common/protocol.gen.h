#include "datapack.h"
#include "log.h"
#include <string.h>
#include <stdio.h>


/*message header for all messages!*/
typedef struct msg_header{
    uint32_t msgid;
    uint32_t version;
    uint32_t operation;
    uint32_t msglength;
}msg_header;

/*master<-client request*/
typedef struct mc_mkdir_request{
    uint32_t msgid; /*abc*/
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
    uint32_t msgid; /*abc*/
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

//void oc_request_pack(struct oc_request * req, uint8_t * data, uint32_t len);
//void oc_request_unpack(struct oc_request * req, uint8_t * data, uint32_t len);

//void mc_mkdir_request_pack(struct mc_mkdir_request * req, uint8_t * data, uint32_t len);
//void mc_mkdir_request_unpack(struct mc_mkdir_request * req, const uint8_t * data, uint32_t len);


int msg_header_pack(struct msg_header * s, uint8_t * data, uint32_t len);
void msg_header_unpack(struct msg_header * s, const uint8_t * data, uint32_t len);
char* msg_header_tostring(struct msg_header * s);
msg_header * msg_header_new();

int mc_mkdir_request_pack(struct mc_mkdir_request * s, uint8_t * data, uint32_t len);
void mc_mkdir_request_unpack(struct mc_mkdir_request * s, const uint8_t * data, uint32_t len);
char* mc_mkdir_request_tostring(struct mc_mkdir_request * s);
mc_mkdir_request * mc_mkdir_request_new();

int mc_mkdir_response_pack(struct mc_mkdir_response * s, uint8_t * data, uint32_t len);
void mc_mkdir_response_unpack(struct mc_mkdir_response * s, const uint8_t * data, uint32_t len);
char* mc_mkdir_response_tostring(struct mc_mkdir_response * s);
mc_mkdir_response * mc_mkdir_response_new();

int oc_request_pack(struct oc_request * s, uint8_t * data, uint32_t len);
void oc_request_unpack(struct oc_request * s, const uint8_t * data, uint32_t len);
char* oc_request_tostring(struct oc_request * s);
oc_request * oc_request_new();

int oc_response_pack(struct oc_response * s, uint8_t * data, uint32_t len);
void oc_response_unpack(struct oc_response * s, const uint8_t * data, uint32_t len);
char* oc_response_tostring(struct oc_response * s);
oc_response * oc_response_new();


#define MSG_MSG_HEADER 100
#define MSG_MC_MKDIR_REQUEST 101
#define MSG_MC_MKDIR_RESPONSE 102
#define MSG_OC_REQUEST 103
#define MSG_OC_RESPONSE 104
