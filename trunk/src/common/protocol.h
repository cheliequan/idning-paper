#include "datapack.h"

/*master<-client request*/
typedef struct mc_mkdir_request{
    int msgid;
    int version;
    int operation;
    int msglength;

    int parent;
    int namelenght;
    uint8_t * name;
    uint32_t mode;
    int uid;
    int gid;

    /*--*/
    void (*pack)  (struct mc_mkdir_request * req, uint8_t * data, int len) ;
    void (*unpack)(struct mc_mkdir_request * req, uint8_t * data, int len);
}mc_mkdir_request;


/*osd<-client request*/
typedef struct oc_request{
    int msgid;
    int version;
    int operation;
    int msglength;
    uint8_t * data;
    /*--*/
    void (*pack) (struct oc_request * req, uint8_t * data, int len) ;
    void (*unpack)(struct oc_request * req, uint8_t * data, int len);
}oc_request;

/*osd<-client response*/
typedef struct oc_response{
    int msgid;
    int version;
    int operation;
    int msglength;
}oc_response;





void oc_request_pack(struct oc_request * req, uint8_t * data, int len);
void oc_request_unpack(struct oc_request * req, uint8_t * data, int len);

void mc_mkdir_request_pack(struct mc_mkdir_request * req, uint8_t * data, int len);
void mc_mkdir_request_unpack(struct mc_mkdir_request * req, const uint8_t * data, int len);


