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
struct setattr_request;
struct setattr_response;
struct ls_request;
struct ls_response;
struct mknod_request;
struct mknod_response;
struct lookup_request;
struct lookup_response;
struct rm_request;
struct rm_response;
struct statfs_request;
struct statfs_response;

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
  FILE_STAT_INO=1,
  FILE_STAT_SIZE=2,
  FILE_STAT_TYPE=3,
  FILE_STAT_NAME=4,
  FILE_STAT_MODE=5,
  FILE_STAT_NLINK=6,
  FILE_STAT_ATIME=7,
  FILE_STAT_MTIME=8,
  FILE_STAT_CTIME=9,
  FILE_STAT_MAX_TAGS
};

/* Structure declaration for file_stat */
struct file_stat_access_ {
  int (*ino_assign)(struct file_stat *, const ev_uint32_t);
  int (*ino_get)(struct file_stat *, ev_uint32_t *);
  int (*size_assign)(struct file_stat *, const ev_uint32_t);
  int (*size_get)(struct file_stat *, ev_uint32_t *);
  int (*type_assign)(struct file_stat *, const ev_uint32_t);
  int (*type_get)(struct file_stat *, ev_uint32_t *);
  int (*name_assign)(struct file_stat *, const char *);
  int (*name_get)(struct file_stat *, char * *);
  int (*mode_assign)(struct file_stat *, const ev_uint32_t);
  int (*mode_get)(struct file_stat *, ev_uint32_t *);
  int (*nlink_assign)(struct file_stat *, const ev_uint32_t);
  int (*nlink_get)(struct file_stat *, ev_uint32_t *);
  int (*atime_assign)(struct file_stat *, const ev_uint32_t);
  int (*atime_get)(struct file_stat *, ev_uint32_t *);
  int (*mtime_assign)(struct file_stat *, const ev_uint32_t);
  int (*mtime_get)(struct file_stat *, ev_uint32_t *);
  int (*ctime_assign)(struct file_stat *, const ev_uint32_t);
  int (*ctime_get)(struct file_stat *, ev_uint32_t *);
};

struct file_stat {
  struct file_stat_access_ *base;

  ev_uint32_t ino;
  ev_uint32_t size;
  ev_uint32_t type;
  char *name;
  ev_uint32_t mode;
  ev_uint32_t nlink;
  ev_uint32_t atime;
  ev_uint32_t mtime;
  ev_uint32_t ctime;

  ev_uint8_t ino_set;
  ev_uint8_t size_set;
  ev_uint8_t type_set;
  ev_uint8_t name_set;
  ev_uint8_t mode_set;
  ev_uint8_t nlink_set;
  ev_uint8_t atime_set;
  ev_uint8_t mtime_set;
  ev_uint8_t ctime_set;
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
int file_stat_ino_assign(struct file_stat *, const ev_uint32_t);
int file_stat_ino_get(struct file_stat *, ev_uint32_t *);
int file_stat_size_assign(struct file_stat *, const ev_uint32_t);
int file_stat_size_get(struct file_stat *, ev_uint32_t *);
int file_stat_type_assign(struct file_stat *, const ev_uint32_t);
int file_stat_type_get(struct file_stat *, ev_uint32_t *);
int file_stat_name_assign(struct file_stat *, const char *);
int file_stat_name_get(struct file_stat *, char * *);
int file_stat_mode_assign(struct file_stat *, const ev_uint32_t);
int file_stat_mode_get(struct file_stat *, ev_uint32_t *);
int file_stat_nlink_assign(struct file_stat *, const ev_uint32_t);
int file_stat_nlink_get(struct file_stat *, ev_uint32_t *);
int file_stat_atime_assign(struct file_stat *, const ev_uint32_t);
int file_stat_atime_get(struct file_stat *, ev_uint32_t *);
int file_stat_mtime_assign(struct file_stat *, const ev_uint32_t);
int file_stat_mtime_get(struct file_stat *, ev_uint32_t *);
int file_stat_ctime_assign(struct file_stat *, const ev_uint32_t);
int file_stat_ctime_get(struct file_stat *, ev_uint32_t *);
/* --- file_stat done --- */

/* Tag definition for stat_request */
enum stat_request_ {
  STAT_REQUEST_INO_ARR=1,
  STAT_REQUEST_MAX_TAGS
};

/* Structure declaration for stat_request */
struct stat_request_access_ {
  int (*ino_arr_assign)(struct stat_request *, int, const ev_uint32_t);
  int (*ino_arr_get)(struct stat_request *, int, ev_uint32_t *);
  ev_uint32_t * (*ino_arr_add)(struct stat_request *msg, const ev_uint32_t value);
};

struct stat_request {
  struct stat_request_access_ *base;

  ev_uint32_t *ino_arr;
  int ino_arr_length;
  int ino_arr_num_allocated;

  ev_uint8_t ino_arr_set;
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
int stat_request_ino_arr_assign(struct stat_request *, int, const ev_uint32_t);
int stat_request_ino_arr_get(struct stat_request *, int, ev_uint32_t *);
ev_uint32_t * stat_request_ino_arr_add(struct stat_request *msg, const ev_uint32_t value);
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

/* Tag definition for setattr_request */
enum setattr_request_ {
  SETATTR_REQUEST_STAT_ARR=1,
  SETATTR_REQUEST_MAX_TAGS
};

/* Structure declaration for setattr_request */
struct setattr_request_access_ {
  int (*stat_arr_assign)(struct setattr_request *, int, const struct file_stat*);
  int (*stat_arr_get)(struct setattr_request *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct setattr_request *msg);
};

struct setattr_request {
  struct setattr_request_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct setattr_request *setattr_request_new(void);
struct setattr_request *setattr_request_new_with_arg(void *);
void setattr_request_free(struct setattr_request *);
void setattr_request_clear(struct setattr_request *);
void setattr_request_marshal(struct evbuffer *, const struct setattr_request *);
int setattr_request_unmarshal(struct setattr_request *, struct evbuffer *);
int setattr_request_complete(struct setattr_request *);
void evtag_marshal_setattr_request(struct evbuffer *, ev_uint32_t,
    const struct setattr_request *);
int evtag_unmarshal_setattr_request(struct evbuffer *, ev_uint32_t,
    struct setattr_request *);
int setattr_request_stat_arr_assign(struct setattr_request *, int, const struct file_stat*);
int setattr_request_stat_arr_get(struct setattr_request *, int, struct file_stat* *);
struct file_stat*  setattr_request_stat_arr_add(struct setattr_request *msg);
/* --- setattr_request done --- */

/* Tag definition for setattr_response */
enum setattr_response_ {
  SETATTR_RESPONSE_STAT_ARR=1,
  SETATTR_RESPONSE_MAX_TAGS
};

/* Structure declaration for setattr_response */
struct setattr_response_access_ {
  int (*stat_arr_assign)(struct setattr_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct setattr_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct setattr_response *msg);
};

struct setattr_response {
  struct setattr_response_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct setattr_response *setattr_response_new(void);
struct setattr_response *setattr_response_new_with_arg(void *);
void setattr_response_free(struct setattr_response *);
void setattr_response_clear(struct setattr_response *);
void setattr_response_marshal(struct evbuffer *, const struct setattr_response *);
int setattr_response_unmarshal(struct setattr_response *, struct evbuffer *);
int setattr_response_complete(struct setattr_response *);
void evtag_marshal_setattr_response(struct evbuffer *, ev_uint32_t,
    const struct setattr_response *);
int evtag_unmarshal_setattr_response(struct evbuffer *, ev_uint32_t,
    struct setattr_response *);
int setattr_response_stat_arr_assign(struct setattr_response *, int, const struct file_stat*);
int setattr_response_stat_arr_get(struct setattr_response *, int, struct file_stat* *);
struct file_stat*  setattr_response_stat_arr_add(struct setattr_response *msg);
/* --- setattr_response done --- */

/* Tag definition for ls_request */
enum ls_request_ {
  LS_REQUEST_INO_ARR=1,
  LS_REQUEST_MAX_TAGS
};

/* Structure declaration for ls_request */
struct ls_request_access_ {
  int (*ino_arr_assign)(struct ls_request *, int, const ev_uint32_t);
  int (*ino_arr_get)(struct ls_request *, int, ev_uint32_t *);
  ev_uint32_t * (*ino_arr_add)(struct ls_request *msg, const ev_uint32_t value);
};

struct ls_request {
  struct ls_request_access_ *base;

  ev_uint32_t *ino_arr;
  int ino_arr_length;
  int ino_arr_num_allocated;

  ev_uint8_t ino_arr_set;
};

struct ls_request *ls_request_new(void);
struct ls_request *ls_request_new_with_arg(void *);
void ls_request_free(struct ls_request *);
void ls_request_clear(struct ls_request *);
void ls_request_marshal(struct evbuffer *, const struct ls_request *);
int ls_request_unmarshal(struct ls_request *, struct evbuffer *);
int ls_request_complete(struct ls_request *);
void evtag_marshal_ls_request(struct evbuffer *, ev_uint32_t,
    const struct ls_request *);
int evtag_unmarshal_ls_request(struct evbuffer *, ev_uint32_t,
    struct ls_request *);
int ls_request_ino_arr_assign(struct ls_request *, int, const ev_uint32_t);
int ls_request_ino_arr_get(struct ls_request *, int, ev_uint32_t *);
ev_uint32_t * ls_request_ino_arr_add(struct ls_request *msg, const ev_uint32_t value);
/* --- ls_request done --- */

/* Tag definition for ls_response */
enum ls_response_ {
  LS_RESPONSE_STAT_ARR=1,
  LS_RESPONSE_MAX_TAGS
};

/* Structure declaration for ls_response */
struct ls_response_access_ {
  int (*stat_arr_assign)(struct ls_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct ls_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct ls_response *msg);
};

struct ls_response {
  struct ls_response_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct ls_response *ls_response_new(void);
struct ls_response *ls_response_new_with_arg(void *);
void ls_response_free(struct ls_response *);
void ls_response_clear(struct ls_response *);
void ls_response_marshal(struct evbuffer *, const struct ls_response *);
int ls_response_unmarshal(struct ls_response *, struct evbuffer *);
int ls_response_complete(struct ls_response *);
void evtag_marshal_ls_response(struct evbuffer *, ev_uint32_t,
    const struct ls_response *);
int evtag_unmarshal_ls_response(struct evbuffer *, ev_uint32_t,
    struct ls_response *);
int ls_response_stat_arr_assign(struct ls_response *, int, const struct file_stat*);
int ls_response_stat_arr_get(struct ls_response *, int, struct file_stat* *);
struct file_stat*  ls_response_stat_arr_add(struct ls_response *msg);
/* --- ls_response done --- */

/* Tag definition for mknod_request */
enum mknod_request_ {
  MKNOD_REQUEST_PARENT_INO=1,
  MKNOD_REQUEST_NAME=2,
  MKNOD_REQUEST_TYPE=3,
  MKNOD_REQUEST_MODE=4,
  MKNOD_REQUEST_MAX_TAGS
};

/* Structure declaration for mknod_request */
struct mknod_request_access_ {
  int (*parent_ino_assign)(struct mknod_request *, const ev_uint32_t);
  int (*parent_ino_get)(struct mknod_request *, ev_uint32_t *);
  int (*name_assign)(struct mknod_request *, const char *);
  int (*name_get)(struct mknod_request *, char * *);
  int (*type_assign)(struct mknod_request *, const ev_uint32_t);
  int (*type_get)(struct mknod_request *, ev_uint32_t *);
  int (*mode_assign)(struct mknod_request *, const ev_uint32_t);
  int (*mode_get)(struct mknod_request *, ev_uint32_t *);
};

struct mknod_request {
  struct mknod_request_access_ *base;

  ev_uint32_t parent_ino;
  char *name;
  ev_uint32_t type;
  ev_uint32_t mode;

  ev_uint8_t parent_ino_set;
  ev_uint8_t name_set;
  ev_uint8_t type_set;
  ev_uint8_t mode_set;
};

struct mknod_request *mknod_request_new(void);
struct mknod_request *mknod_request_new_with_arg(void *);
void mknod_request_free(struct mknod_request *);
void mknod_request_clear(struct mknod_request *);
void mknod_request_marshal(struct evbuffer *, const struct mknod_request *);
int mknod_request_unmarshal(struct mknod_request *, struct evbuffer *);
int mknod_request_complete(struct mknod_request *);
void evtag_marshal_mknod_request(struct evbuffer *, ev_uint32_t,
    const struct mknod_request *);
int evtag_unmarshal_mknod_request(struct evbuffer *, ev_uint32_t,
    struct mknod_request *);
int mknod_request_parent_ino_assign(struct mknod_request *, const ev_uint32_t);
int mknod_request_parent_ino_get(struct mknod_request *, ev_uint32_t *);
int mknod_request_name_assign(struct mknod_request *, const char *);
int mknod_request_name_get(struct mknod_request *, char * *);
int mknod_request_type_assign(struct mknod_request *, const ev_uint32_t);
int mknod_request_type_get(struct mknod_request *, ev_uint32_t *);
int mknod_request_mode_assign(struct mknod_request *, const ev_uint32_t);
int mknod_request_mode_get(struct mknod_request *, ev_uint32_t *);
/* --- mknod_request done --- */

/* Tag definition for mknod_response */
enum mknod_response_ {
  MKNOD_RESPONSE_STAT_ARR=1,
  MKNOD_RESPONSE_MAX_TAGS
};

/* Structure declaration for mknod_response */
struct mknod_response_access_ {
  int (*stat_arr_assign)(struct mknod_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct mknod_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct mknod_response *msg);
};

struct mknod_response {
  struct mknod_response_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct mknod_response *mknod_response_new(void);
struct mknod_response *mknod_response_new_with_arg(void *);
void mknod_response_free(struct mknod_response *);
void mknod_response_clear(struct mknod_response *);
void mknod_response_marshal(struct evbuffer *, const struct mknod_response *);
int mknod_response_unmarshal(struct mknod_response *, struct evbuffer *);
int mknod_response_complete(struct mknod_response *);
void evtag_marshal_mknod_response(struct evbuffer *, ev_uint32_t,
    const struct mknod_response *);
int evtag_unmarshal_mknod_response(struct evbuffer *, ev_uint32_t,
    struct mknod_response *);
int mknod_response_stat_arr_assign(struct mknod_response *, int, const struct file_stat*);
int mknod_response_stat_arr_get(struct mknod_response *, int, struct file_stat* *);
struct file_stat*  mknod_response_stat_arr_add(struct mknod_response *msg);
/* --- mknod_response done --- */

/* Tag definition for lookup_request */
enum lookup_request_ {
  LOOKUP_REQUEST_PARENT_INO=1,
  LOOKUP_REQUEST_NAME=2,
  LOOKUP_REQUEST_MAX_TAGS
};

/* Structure declaration for lookup_request */
struct lookup_request_access_ {
  int (*parent_ino_assign)(struct lookup_request *, const ev_uint32_t);
  int (*parent_ino_get)(struct lookup_request *, ev_uint32_t *);
  int (*name_assign)(struct lookup_request *, const char *);
  int (*name_get)(struct lookup_request *, char * *);
};

struct lookup_request {
  struct lookup_request_access_ *base;

  ev_uint32_t parent_ino;
  char *name;

  ev_uint8_t parent_ino_set;
  ev_uint8_t name_set;
};

struct lookup_request *lookup_request_new(void);
struct lookup_request *lookup_request_new_with_arg(void *);
void lookup_request_free(struct lookup_request *);
void lookup_request_clear(struct lookup_request *);
void lookup_request_marshal(struct evbuffer *, const struct lookup_request *);
int lookup_request_unmarshal(struct lookup_request *, struct evbuffer *);
int lookup_request_complete(struct lookup_request *);
void evtag_marshal_lookup_request(struct evbuffer *, ev_uint32_t,
    const struct lookup_request *);
int evtag_unmarshal_lookup_request(struct evbuffer *, ev_uint32_t,
    struct lookup_request *);
int lookup_request_parent_ino_assign(struct lookup_request *, const ev_uint32_t);
int lookup_request_parent_ino_get(struct lookup_request *, ev_uint32_t *);
int lookup_request_name_assign(struct lookup_request *, const char *);
int lookup_request_name_get(struct lookup_request *, char * *);
/* --- lookup_request done --- */

/* Tag definition for lookup_response */
enum lookup_response_ {
  LOOKUP_RESPONSE_STAT_ARR=1,
  LOOKUP_RESPONSE_MAX_TAGS
};

/* Structure declaration for lookup_response */
struct lookup_response_access_ {
  int (*stat_arr_assign)(struct lookup_response *, int, const struct file_stat*);
  int (*stat_arr_get)(struct lookup_response *, int, struct file_stat* *);
  struct file_stat*  (*stat_arr_add)(struct lookup_response *msg);
};

struct lookup_response {
  struct lookup_response_access_ *base;

  struct file_stat* *stat_arr;
  int stat_arr_length;
  int stat_arr_num_allocated;

  ev_uint8_t stat_arr_set;
};

struct lookup_response *lookup_response_new(void);
struct lookup_response *lookup_response_new_with_arg(void *);
void lookup_response_free(struct lookup_response *);
void lookup_response_clear(struct lookup_response *);
void lookup_response_marshal(struct evbuffer *, const struct lookup_response *);
int lookup_response_unmarshal(struct lookup_response *, struct evbuffer *);
int lookup_response_complete(struct lookup_response *);
void evtag_marshal_lookup_response(struct evbuffer *, ev_uint32_t,
    const struct lookup_response *);
int evtag_unmarshal_lookup_response(struct evbuffer *, ev_uint32_t,
    struct lookup_response *);
int lookup_response_stat_arr_assign(struct lookup_response *, int, const struct file_stat*);
int lookup_response_stat_arr_get(struct lookup_response *, int, struct file_stat* *);
struct file_stat*  lookup_response_stat_arr_add(struct lookup_response *msg);
/* --- lookup_response done --- */

/* Tag definition for rm_request */
enum rm_request_ {
  RM_REQUEST_INO=1,
  RM_REQUEST_TYPE=2,
  RM_REQUEST_MAX_TAGS
};

/* Structure declaration for rm_request */
struct rm_request_access_ {
  int (*ino_assign)(struct rm_request *, const ev_uint32_t);
  int (*ino_get)(struct rm_request *, ev_uint32_t *);
  int (*type_assign)(struct rm_request *, const ev_uint32_t);
  int (*type_get)(struct rm_request *, ev_uint32_t *);
};

struct rm_request {
  struct rm_request_access_ *base;

  ev_uint32_t ino;
  ev_uint32_t type;

  ev_uint8_t ino_set;
  ev_uint8_t type_set;
};

struct rm_request *rm_request_new(void);
struct rm_request *rm_request_new_with_arg(void *);
void rm_request_free(struct rm_request *);
void rm_request_clear(struct rm_request *);
void rm_request_marshal(struct evbuffer *, const struct rm_request *);
int rm_request_unmarshal(struct rm_request *, struct evbuffer *);
int rm_request_complete(struct rm_request *);
void evtag_marshal_rm_request(struct evbuffer *, ev_uint32_t,
    const struct rm_request *);
int evtag_unmarshal_rm_request(struct evbuffer *, ev_uint32_t,
    struct rm_request *);
int rm_request_ino_assign(struct rm_request *, const ev_uint32_t);
int rm_request_ino_get(struct rm_request *, ev_uint32_t *);
int rm_request_type_assign(struct rm_request *, const ev_uint32_t);
int rm_request_type_get(struct rm_request *, ev_uint32_t *);
/* --- rm_request done --- */

/* Tag definition for rm_response */
enum rm_response_ {
  RM_RESPONSE_INO=1,
  RM_RESPONSE_MAX_TAGS
};

/* Structure declaration for rm_response */
struct rm_response_access_ {
  int (*ino_assign)(struct rm_response *, const ev_uint32_t);
  int (*ino_get)(struct rm_response *, ev_uint32_t *);
};

struct rm_response {
  struct rm_response_access_ *base;

  ev_uint32_t ino;

  ev_uint8_t ino_set;
};

struct rm_response *rm_response_new(void);
struct rm_response *rm_response_new_with_arg(void *);
void rm_response_free(struct rm_response *);
void rm_response_clear(struct rm_response *);
void rm_response_marshal(struct evbuffer *, const struct rm_response *);
int rm_response_unmarshal(struct rm_response *, struct evbuffer *);
int rm_response_complete(struct rm_response *);
void evtag_marshal_rm_response(struct evbuffer *, ev_uint32_t,
    const struct rm_response *);
int evtag_unmarshal_rm_response(struct evbuffer *, ev_uint32_t,
    struct rm_response *);
int rm_response_ino_assign(struct rm_response *, const ev_uint32_t);
int rm_response_ino_get(struct rm_response *, ev_uint32_t *);
/* --- rm_response done --- */

/* Tag definition for statfs_request */
enum statfs_request_ {
  STATFS_REQUEST_X=1,
  STATFS_REQUEST_MAX_TAGS
};

/* Structure declaration for statfs_request */
struct statfs_request_access_ {
  int (*x_assign)(struct statfs_request *, const ev_uint32_t);
  int (*x_get)(struct statfs_request *, ev_uint32_t *);
};

struct statfs_request {
  struct statfs_request_access_ *base;

  ev_uint32_t x;

  ev_uint8_t x_set;
};

struct statfs_request *statfs_request_new(void);
struct statfs_request *statfs_request_new_with_arg(void *);
void statfs_request_free(struct statfs_request *);
void statfs_request_clear(struct statfs_request *);
void statfs_request_marshal(struct evbuffer *, const struct statfs_request *);
int statfs_request_unmarshal(struct statfs_request *, struct evbuffer *);
int statfs_request_complete(struct statfs_request *);
void evtag_marshal_statfs_request(struct evbuffer *, ev_uint32_t,
    const struct statfs_request *);
int evtag_unmarshal_statfs_request(struct evbuffer *, ev_uint32_t,
    struct statfs_request *);
int statfs_request_x_assign(struct statfs_request *, const ev_uint32_t);
int statfs_request_x_get(struct statfs_request *, ev_uint32_t *);
/* --- statfs_request done --- */

/* Tag definition for statfs_response */
enum statfs_response_ {
  STATFS_RESPONSE_X=1,
  STATFS_RESPONSE_MAX_TAGS
};

/* Structure declaration for statfs_response */
struct statfs_response_access_ {
  int (*x_assign)(struct statfs_response *, const ev_uint32_t);
  int (*x_get)(struct statfs_response *, ev_uint32_t *);
};

struct statfs_response {
  struct statfs_response_access_ *base;

  ev_uint32_t x;

  ev_uint8_t x_set;
};

struct statfs_response *statfs_response_new(void);
struct statfs_response *statfs_response_new_with_arg(void *);
void statfs_response_free(struct statfs_response *);
void statfs_response_clear(struct statfs_response *);
void statfs_response_marshal(struct evbuffer *, const struct statfs_response *);
int statfs_response_unmarshal(struct statfs_response *, struct evbuffer *);
int statfs_response_complete(struct statfs_response *);
void evtag_marshal_statfs_response(struct evbuffer *, ev_uint32_t,
    const struct statfs_response *);
int evtag_unmarshal_statfs_response(struct evbuffer *, ev_uint32_t,
    struct statfs_response *);
int statfs_response_x_assign(struct statfs_response *, const ev_uint32_t);
int statfs_response_x_get(struct statfs_response *, ev_uint32_t *);
/* --- statfs_response done --- */

#endif  /* _COMMON_PROTOCOL_RPC_ */
