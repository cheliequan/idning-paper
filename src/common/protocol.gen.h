/*
 * Automatically generated from common/protocol.rpc
 */

#ifndef _COMMON_PROTOCOL_RPC_
#define _COMMON_PROTOCOL_RPC_

#include <event2/util.h> /* for ev_uint*_t */
#include <event2/rpc.h>

struct ping;
struct machine;
struct pong;
struct file_stat;
struct stat_request;
struct stat_response;

/* Tag definition for ping */
enum ping_ {
  PING_VERSION=1,
  PING_SELF_IP=2,
  PING_SELF_PORT=3,
  PING_MAX_TAGS
};

/* Structure declaration for ping */
struct ping_access_ {
  int (*version_assign)(struct ping *, const ev_uint32_t);
  int (*version_get)(struct ping *, ev_uint32_t *);
  int (*self_ip_assign)(struct ping *, const char *);
  int (*self_ip_get)(struct ping *, char * *);
  int (*self_port_assign)(struct ping *, const ev_uint32_t);
  int (*self_port_get)(struct ping *, ev_uint32_t *);
};

struct ping {
  struct ping_access_ *base;

  ev_uint32_t version;
  char *self_ip;
  ev_uint32_t self_port;

  ev_uint8_t version_set;
  ev_uint8_t self_ip_set;
  ev_uint8_t self_port_set;
};

struct ping *ping_new(void);
struct ping *ping_new_with_arg(void *);
void ping_free(struct ping *);
void ping_clear(struct ping *);
void ping_marshal(struct evbuffer *, const struct ping *);
int ping_unmarshal(struct ping *, struct evbuffer *);
int ping_complete(struct ping *);
void evtag_marshal_ping(struct evbuffer *, ev_uint32_t,
    const struct ping *);
int evtag_unmarshal_ping(struct evbuffer *, ev_uint32_t,
    struct ping *);
int ping_version_assign(struct ping *, const ev_uint32_t);
int ping_version_get(struct ping *, ev_uint32_t *);
int ping_self_ip_assign(struct ping *, const char *);
int ping_self_ip_get(struct ping *, char * *);
int ping_self_port_assign(struct ping *, const ev_uint32_t);
int ping_self_port_get(struct ping *, ev_uint32_t *);
/* --- ping done --- */

/* Tag definition for machine */
enum machine_ {
  MACHINE_UUID=1,
  MACHINE_IP=2,
  MACHINE_PORT=3,
  MACHINE_TYPE=4,
  MACHINE_MAX_TAGS
};

/* Structure declaration for machine */
struct machine_access_ {
  int (*uuid_assign)(struct machine *, const ev_uint32_t);
  int (*uuid_get)(struct machine *, ev_uint32_t *);
  int (*ip_assign)(struct machine *, const char *);
  int (*ip_get)(struct machine *, char * *);
  int (*port_assign)(struct machine *, const ev_uint32_t);
  int (*port_get)(struct machine *, ev_uint32_t *);
  int (*type_assign)(struct machine *, const ev_uint32_t);
  int (*type_get)(struct machine *, ev_uint32_t *);
};

struct machine {
  struct machine_access_ *base;

  ev_uint32_t uuid;
  char *ip;
  ev_uint32_t port;
  ev_uint32_t type;

  ev_uint8_t uuid_set;
  ev_uint8_t ip_set;
  ev_uint8_t port_set;
  ev_uint8_t type_set;
};

struct machine *machine_new(void);
struct machine *machine_new_with_arg(void *);
void machine_free(struct machine *);
void machine_clear(struct machine *);
void machine_marshal(struct evbuffer *, const struct machine *);
int machine_unmarshal(struct machine *, struct evbuffer *);
int machine_complete(struct machine *);
void evtag_marshal_machine(struct evbuffer *, ev_uint32_t,
    const struct machine *);
int evtag_unmarshal_machine(struct evbuffer *, ev_uint32_t,
    struct machine *);
int machine_uuid_assign(struct machine *, const ev_uint32_t);
int machine_uuid_get(struct machine *, ev_uint32_t *);
int machine_ip_assign(struct machine *, const char *);
int machine_ip_get(struct machine *, char * *);
int machine_port_assign(struct machine *, const ev_uint32_t);
int machine_port_get(struct machine *, ev_uint32_t *);
int machine_type_assign(struct machine *, const ev_uint32_t);
int machine_type_get(struct machine *, ev_uint32_t *);
/* --- machine done --- */

/* Tag definition for pong */
enum pong_ {
  PONG_VERSION=1,
  PONG_XX=2,
  PONG_MACHINES=3,
  PONG_MAX_TAGS
};

/* Structure declaration for pong */
struct pong_access_ {
  int (*version_assign)(struct pong *, const ev_uint32_t);
  int (*version_get)(struct pong *, ev_uint32_t *);
  int (*xx_assign)(struct pong *, const ev_uint32_t);
  int (*xx_get)(struct pong *, ev_uint32_t *);
  int (*machines_assign)(struct pong *, int, const struct machine*);
  int (*machines_get)(struct pong *, int, struct machine* *);
  struct machine*  (*machines_add)(struct pong *msg);
};

struct pong {
  struct pong_access_ *base;

  ev_uint32_t version;
  ev_uint32_t xx;
  struct machine* *machines;
  int machines_length;
  int machines_num_allocated;

  ev_uint8_t version_set;
  ev_uint8_t xx_set;
  ev_uint8_t machines_set;
};

struct pong *pong_new(void);
struct pong *pong_new_with_arg(void *);
void pong_free(struct pong *);
void pong_clear(struct pong *);
void pong_marshal(struct evbuffer *, const struct pong *);
int pong_unmarshal(struct pong *, struct evbuffer *);
int pong_complete(struct pong *);
void evtag_marshal_pong(struct evbuffer *, ev_uint32_t,
    const struct pong *);
int evtag_unmarshal_pong(struct evbuffer *, ev_uint32_t,
    struct pong *);
int pong_version_assign(struct pong *, const ev_uint32_t);
int pong_version_get(struct pong *, ev_uint32_t *);
int pong_xx_assign(struct pong *, const ev_uint32_t);
int pong_xx_get(struct pong *, ev_uint32_t *);
int pong_machines_assign(struct pong *, int, const struct machine*);
int pong_machines_get(struct pong *, int, struct machine* *);
struct machine*  pong_machines_add(struct pong *msg);
/* --- pong done --- */

/* Tag definition for file_stat */
enum file_stat_ {
  FILE_STAT_INODE=1,
  FILE_STAT_SIZE=2,
  FILE_STAT_MAX_TAGS
};

/* Structure declaration for file_stat */
struct file_stat_access_ {
  int (*inode_assign)(struct file_stat *, const ev_uint32_t);
  int (*inode_get)(struct file_stat *, ev_uint32_t *);
  int (*size_assign)(struct file_stat *, const ev_uint32_t);
  int (*size_get)(struct file_stat *, ev_uint32_t *);
};

struct file_stat {
  struct file_stat_access_ *base;

  ev_uint32_t inode;
  ev_uint32_t size;

  ev_uint8_t inode_set;
  ev_uint8_t size_set;
};

struct file_stat *file_stat_new(void);
struct file_stat *file_stat_new_with_arg(void *);
void file_stat_free(struct file_stat *);
void file_stat_clear(struct file_stat *);
void file_stat_marshal(struct evbuffer *, const struct file_stat *);
int file_stat_unmarshal(struct file_stat *, struct evbuffer *);
int file_stat_complete(struct file_stat *);
void evtag_marshal_file_stat(struct evbuffer *, ev_uint32_t,
    const struct file_stat *);
int evtag_unmarshal_file_stat(struct evbuffer *, ev_uint32_t,
    struct file_stat *);
int file_stat_inode_assign(struct file_stat *, const ev_uint32_t);
int file_stat_inode_get(struct file_stat *, ev_uint32_t *);
int file_stat_size_assign(struct file_stat *, const ev_uint32_t);
int file_stat_size_get(struct file_stat *, ev_uint32_t *);
/* --- file_stat done --- */

/* Tag definition for stat_request */
enum stat_request_ {
  STAT_REQUEST_INODE_ARR=1,
  STAT_REQUEST_MAX_TAGS
};

/* Structure declaration for stat_request */
struct stat_request_access_ {
  int (*inode_arr_assign)(struct stat_request *, int, const ev_uint32_t);
  int (*inode_arr_get)(struct stat_request *, int, ev_uint32_t *);
  ev_uint32_t * (*inode_arr_add)(struct stat_request *msg, const ev_uint32_t value);
};

struct stat_request {
  struct stat_request_access_ *base;

  ev_uint32_t *inode_arr;
  int inode_arr_length;
  int inode_arr_num_allocated;

  ev_uint8_t inode_arr_set;
};

struct stat_request *stat_request_new(void);
struct stat_request *stat_request_new_with_arg(void *);
void stat_request_free(struct stat_request *);
void stat_request_clear(struct stat_request *);
void stat_request_marshal(struct evbuffer *, const struct stat_request *);
int stat_request_unmarshal(struct stat_request *, struct evbuffer *);
int stat_request_complete(struct stat_request *);
void evtag_marshal_stat_request(struct evbuffer *, ev_uint32_t,
    const struct stat_request *);
int evtag_unmarshal_stat_request(struct evbuffer *, ev_uint32_t,
    struct stat_request *);
int stat_request_inode_arr_assign(struct stat_request *, int, const ev_uint32_t);
int stat_request_inode_arr_get(struct stat_request *, int, ev_uint32_t *);
ev_uint32_t * stat_request_inode_arr_add(struct stat_request *msg, const ev_uint32_t value);
/* --- stat_request done --- */

/* Tag definition for stat_response */
enum stat_response_ {
  STAT_RESPONSE_STAT_ARR=1,
  STAT_RESPONSE_MAX_TAGS
};

/* Structure declaration for stat_response */
struct stat_response_access_ {
  int (*stat_arr_assign)(struct stat_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct stat_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct stat_response *msg);
};

struct stat_response {
  struct stat_response_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct stat_response *stat_response_new(void);
struct stat_response *stat_response_new_with_arg(void *);
void stat_response_free(struct stat_response *);
void stat_response_clear(struct stat_response *);
void stat_response_marshal(struct evbuffer *, const struct stat_response *);
int stat_response_unmarshal(struct stat_response *, struct evbuffer *);
int stat_response_complete(struct stat_response *);
void evtag_marshal_stat_response(struct evbuffer *, ev_uint32_t,
    const struct stat_response *);
int evtag_unmarshal_stat_response(struct evbuffer *, ev_uint32_t,
    struct stat_response *);
int stat_response_stat_arr_assign(struct stat_response *, int, const struct file_stat*);
int stat_response_stat_arr_get(struct stat_response *, int, struct file_stat* *);
struct file_stat*  stat_response_stat_arr_add(struct stat_response *msg);
/* --- stat_response done --- */

#endif  /* _COMMON_PROTOCOL_RPC_ */
