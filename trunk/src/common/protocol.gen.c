/*
 * Automatically generated from common/protocol.rpc
 * by event_rpcgen.py/0.1.  DO NOT EDIT THIS FILE.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <event2/event-config.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/tag.h>

#ifdef _EVENT___func__
#define __func__ _EVENT___func__
#endif


#include "protocol.gen.h"

void event_warn(const char *fmt, ...);
void event_warnx(const char *fmt, ...);


/*
 * Implementation of ping
 */

static struct ping_access_ __ping_base = {
  ping_version_assign,
  ping_version_get,
  ping_self_ip_assign,
  ping_self_ip_get,
  ping_self_port_assign,
  ping_self_port_get,
};

struct ping *
ping_new(void)
{
  return ping_new_with_arg(NULL);
}

struct ping *
ping_new_with_arg(void *unused)
{
  struct ping *tmp;
  if ((tmp = malloc(sizeof(struct ping))) == NULL) {
    event_warn("%s: malloc", __func__);
    return (NULL);
  }
  tmp->base = &__ping_base;

  tmp->version = 0;
  tmp->version_set = 0;

  tmp->self_ip = NULL;
  tmp->self_ip_set = 0;

  tmp->self_port = 0;
  tmp->self_port_set = 0;

  return (tmp);
}




int
ping_version_assign(struct ping *msg, const ev_uint32_t value)
{
  msg->version_set = 1;
  msg->version = value;
  return (0);
}

int
ping_self_ip_assign(struct ping *msg,
    const char * value)
{
  if (msg->self_ip != NULL)
    free(msg->self_ip);
  if ((msg->self_ip = strdup(value)) == NULL)
    return (-1);
  msg->self_ip_set = 1;
  return (0);
}

int
ping_self_port_assign(struct ping *msg, const ev_uint32_t value)
{
  msg->self_port_set = 1;
  msg->self_port = value;
  return (0);
}

int
ping_version_get(struct ping *msg, ev_uint32_t *value)
{
  if (msg->version_set != 1)
    return (-1);
  *value = msg->version;
  return (0);
}

int
ping_self_ip_get(struct ping *msg, char * *value)
{
  if (msg->self_ip_set != 1)
    return (-1);
  *value = msg->self_ip;
  return (0);
}

int
ping_self_port_get(struct ping *msg, ev_uint32_t *value)
{
  if (msg->self_port_set != 1)
    return (-1);
  *value = msg->self_port;
  return (0);
}

void
ping_clear(struct ping *tmp)
{
  tmp->version_set = 0;
  if (tmp->self_ip_set == 1) {
    free(tmp->self_ip);
    tmp->self_ip = NULL;
    tmp->self_ip_set = 0;
  }
  tmp->self_port_set = 0;
}

void
ping_free(struct ping *tmp)
{
  if (tmp->self_ip != NULL)
      free (tmp->self_ip);
  free(tmp);
}

void
ping_marshal(struct evbuffer *evbuf, const struct ping *tmp){
  evtag_marshal_int(evbuf, PING_VERSION, tmp->version);
  evtag_marshal_string(evbuf, PING_SELF_IP, tmp->self_ip);
  evtag_marshal_int(evbuf, PING_SELF_PORT, tmp->self_port);
}

int
ping_unmarshal(struct ping *tmp,  struct evbuffer *evbuf)
{
  ev_uint32_t tag;
  while (evbuffer_get_length(evbuf) > 0) {
    if (evtag_peek(evbuf, &tag) == -1)
      return (-1);
    switch (tag) {

      case PING_VERSION:

        if (tmp->version_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, PING_VERSION, &tmp->version) == -1) {
          event_warnx("%s: failed to unmarshal version", __func__);
          return (-1);
        }
        tmp->version_set = 1;
        break;

      case PING_SELF_IP:

        if (tmp->self_ip_set)
          return (-1);
        if (evtag_unmarshal_string(evbuf, PING_SELF_IP, &tmp->self_ip) == -1) {
          event_warnx("%s: failed to unmarshal self_ip", __func__);
          return (-1);
        }
        tmp->self_ip_set = 1;
        break;

      case PING_SELF_PORT:

        if (tmp->self_port_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, PING_SELF_PORT, &tmp->self_port) == -1) {
          event_warnx("%s: failed to unmarshal self_port", __func__);
          return (-1);
        }
        tmp->self_port_set = 1;
        break;

      default:
        return -1;
    }
  }

  if (ping_complete(tmp) == -1)
    return (-1);
  return (0);
}

int
ping_complete(struct ping *msg)
{
  if (!msg->version_set)
    return (-1);
  if (!msg->self_ip_set)
    return (-1);
  if (!msg->self_port_set)
    return (-1);
  return (0);
}

int
evtag_unmarshal_ping(struct evbuffer *evbuf, ev_uint32_t need_tag, struct ping *msg)
{
  ev_uint32_t tag;
  int res = -1;

  struct evbuffer *tmp = evbuffer_new();

  if (evtag_unmarshal(evbuf, &tag, tmp) == -1 || tag != need_tag)
    goto error;

  if (ping_unmarshal(msg, tmp) == -1)
    goto error;

  res = 0;

 error:
  evbuffer_free(tmp);
  return (res);
}

void
evtag_marshal_ping(struct evbuffer *evbuf, ev_uint32_t tag, const struct ping *msg)
{
  struct evbuffer *_buf = evbuffer_new();
  assert(_buf != NULL);
  ping_marshal(_buf, msg);
  evtag_marshal_buffer(evbuf, tag, _buf);
   evbuffer_free(_buf);
}

/*
 * Implementation of machine
 */

static struct machine_access_ __machine_base = {
  machine_uuid_assign,
  machine_uuid_get,
  machine_ip_assign,
  machine_ip_get,
  machine_port_assign,
  machine_port_get,
  machine_type_assign,
  machine_type_get,
};

struct machine *
machine_new(void)
{
  return machine_new_with_arg(NULL);
}

struct machine *
machine_new_with_arg(void *unused)
{
  struct machine *tmp;
  if ((tmp = malloc(sizeof(struct machine))) == NULL) {
    event_warn("%s: malloc", __func__);
    return (NULL);
  }
  tmp->base = &__machine_base;

  tmp->uuid = 0;
  tmp->uuid_set = 0;

  tmp->ip = NULL;
  tmp->ip_set = 0;

  tmp->port = 0;
  tmp->port_set = 0;

  tmp->type = 0;
  tmp->type_set = 0;

  return (tmp);
}





int
machine_uuid_assign(struct machine *msg, const ev_uint32_t value)
{
  msg->uuid_set = 1;
  msg->uuid = value;
  return (0);
}

int
machine_ip_assign(struct machine *msg,
    const char * value)
{
  if (msg->ip != NULL)
    free(msg->ip);
  if ((msg->ip = strdup(value)) == NULL)
    return (-1);
  msg->ip_set = 1;
  return (0);
}

int
machine_port_assign(struct machine *msg, const ev_uint32_t value)
{
  msg->port_set = 1;
  msg->port = value;
  return (0);
}

int
machine_type_assign(struct machine *msg, const ev_uint32_t value)
{
  msg->type_set = 1;
  msg->type = value;
  return (0);
}

int
machine_uuid_get(struct machine *msg, ev_uint32_t *value)
{
  if (msg->uuid_set != 1)
    return (-1);
  *value = msg->uuid;
  return (0);
}

int
machine_ip_get(struct machine *msg, char * *value)
{
  if (msg->ip_set != 1)
    return (-1);
  *value = msg->ip;
  return (0);
}

int
machine_port_get(struct machine *msg, ev_uint32_t *value)
{
  if (msg->port_set != 1)
    return (-1);
  *value = msg->port;
  return (0);
}

int
machine_type_get(struct machine *msg, ev_uint32_t *value)
{
  if (msg->type_set != 1)
    return (-1);
  *value = msg->type;
  return (0);
}

void
machine_clear(struct machine *tmp)
{
  tmp->uuid_set = 0;
  if (tmp->ip_set == 1) {
    free(tmp->ip);
    tmp->ip = NULL;
    tmp->ip_set = 0;
  }
  tmp->port_set = 0;
  tmp->type_set = 0;
}

void
machine_free(struct machine *tmp)
{
  if (tmp->ip != NULL)
      free (tmp->ip);
  free(tmp);
}

void
machine_marshal(struct evbuffer *evbuf, const struct machine *tmp){
  evtag_marshal_int(evbuf, MACHINE_UUID, tmp->uuid);
  evtag_marshal_string(evbuf, MACHINE_IP, tmp->ip);
  evtag_marshal_int(evbuf, MACHINE_PORT, tmp->port);
  evtag_marshal_int(evbuf, MACHINE_TYPE, tmp->type);
}

int
machine_unmarshal(struct machine *tmp,  struct evbuffer *evbuf)
{
  ev_uint32_t tag;
  while (evbuffer_get_length(evbuf) > 0) {
    if (evtag_peek(evbuf, &tag) == -1)
      return (-1);
    switch (tag) {

      case MACHINE_UUID:

        if (tmp->uuid_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, MACHINE_UUID, &tmp->uuid) == -1) {
          event_warnx("%s: failed to unmarshal uuid", __func__);
          return (-1);
        }
        tmp->uuid_set = 1;
        break;

      case MACHINE_IP:

        if (tmp->ip_set)
          return (-1);
        if (evtag_unmarshal_string(evbuf, MACHINE_IP, &tmp->ip) == -1) {
          event_warnx("%s: failed to unmarshal ip", __func__);
          return (-1);
        }
        tmp->ip_set = 1;
        break;

      case MACHINE_PORT:

        if (tmp->port_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, MACHINE_PORT, &tmp->port) == -1) {
          event_warnx("%s: failed to unmarshal port", __func__);
          return (-1);
        }
        tmp->port_set = 1;
        break;

      case MACHINE_TYPE:

        if (tmp->type_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, MACHINE_TYPE, &tmp->type) == -1) {
          event_warnx("%s: failed to unmarshal type", __func__);
          return (-1);
        }
        tmp->type_set = 1;
        break;

      default:
        return -1;
    }
  }

  if (machine_complete(tmp) == -1)
    return (-1);
  return (0);
}

int
machine_complete(struct machine *msg)
{
  if (!msg->uuid_set)
    return (-1);
  if (!msg->ip_set)
    return (-1);
  if (!msg->port_set)
    return (-1);
  if (!msg->type_set)
    return (-1);
  return (0);
}

int
evtag_unmarshal_machine(struct evbuffer *evbuf, ev_uint32_t need_tag, struct machine *msg)
{
  ev_uint32_t tag;
  int res = -1;

  struct evbuffer *tmp = evbuffer_new();

  if (evtag_unmarshal(evbuf, &tag, tmp) == -1 || tag != need_tag)
    goto error;

  if (machine_unmarshal(msg, tmp) == -1)
    goto error;

  res = 0;

 error:
  evbuffer_free(tmp);
  return (res);
}

void
evtag_marshal_machine(struct evbuffer *evbuf, ev_uint32_t tag, const struct machine *msg)
{
  struct evbuffer *_buf = evbuffer_new();
  assert(_buf != NULL);
  machine_marshal(_buf, msg);
  evtag_marshal_buffer(evbuf, tag, _buf);
   evbuffer_free(_buf);
}

/*
 * Implementation of pong
 */

static struct pong_access_ __pong_base = {
  pong_version_assign,
  pong_version_get,
  pong_xx_assign,
  pong_xx_get,
  pong_machines_assign,
  pong_machines_get,
  pong_machines_add,
};

struct pong *
pong_new(void)
{
  return pong_new_with_arg(NULL);
}

struct pong *
pong_new_with_arg(void *unused)
{
  struct pong *tmp;
  if ((tmp = malloc(sizeof(struct pong))) == NULL) {
    event_warn("%s: malloc", __func__);
    return (NULL);
  }
  tmp->base = &__pong_base;

  tmp->version = 0;
  tmp->version_set = 0;

  tmp->xx = 0;
  tmp->xx_set = 0;

  tmp->machines = NULL;
  tmp->machines_length = 0;
  tmp->machines_num_allocated = 0;
  tmp->machines_set = 0;

  return (tmp);
}



static int
pong_machines_expand_to_hold_more(struct pong *msg)
{
  int tobe_allocated = msg->machines_num_allocated;
  struct machine** new_d_ata = NULL;
  tobe_allocated = !tobe_allocated ? 1 : tobe_allocated << 1;
  new_d_ata = (struct machine**) realloc(msg->machines,
      tobe_allocated * sizeof(struct machine*));
  if (new_d_ata == NULL)
    return -1;
  msg->machines = new_d_ata;
  msg->machines_num_allocated = tobe_allocated;
  return 0;}

struct machine* 
pong_machines_add(struct pong *msg)
{
  if (++msg->machines_length >= msg->machines_num_allocated) {
    if (pong_machines_expand_to_hold_more(msg)<0)
      goto error;
  }
  msg->machines[msg->machines_length - 1] = machine_new();
  if (msg->machines[msg->machines_length - 1] == NULL)
    goto error;
  msg->machines_set = 1;
  return (msg->machines[msg->machines_length - 1]);
error:
  --msg->machines_length;
  return (NULL);
}

int
pong_version_assign(struct pong *msg, const ev_uint32_t value)
{
  msg->version_set = 1;
  msg->version = value;
  return (0);
}

int
pong_xx_assign(struct pong *msg, const ev_uint32_t value)
{
  msg->xx_set = 1;
  msg->xx = value;
  return (0);
}

int
pong_machines_assign(struct pong *msg, int off,
    const struct machine* value)
{
  if (!msg->machines_set || off < 0 || off >= msg->machines_length)
    return (-1);

  {
    int had_error = 0;
    struct evbuffer *tmp = NULL;
    machine_clear(msg->machines[off]);
    if ((tmp = evbuffer_new()) == NULL) {
      event_warn("%s: evbuffer_new()", __func__);
      had_error = 1;
      goto done;
    }
    machine_marshal(tmp, value);
    if (machine_unmarshal(msg->machines[off], tmp) == -1) {
      event_warnx("%s: machine_unmarshal", __func__);
      had_error = 1;
      goto done;
    }
    done:if (tmp != NULL)
      evbuffer_free(tmp);
    if (had_error) {
      machine_clear(msg->machines[off]);
      return (-1);
    }
  }
  return (0);
}

int
pong_version_get(struct pong *msg, ev_uint32_t *value)
{
  if (msg->version_set != 1)
    return (-1);
  *value = msg->version;
  return (0);
}

int
pong_xx_get(struct pong *msg, ev_uint32_t *value)
{
  if (msg->xx_set != 1)
    return (-1);
  *value = msg->xx;
  return (0);
}

int
pong_machines_get(struct pong *msg, int offset,
    struct machine* *value)
{
  if (!msg->machines_set || offset < 0 || offset >= msg->machines_length)
    return (-1);
  *value = msg->machines[offset];
  return (0);
}

void
pong_clear(struct pong *tmp)
{
  tmp->version_set = 0;
  tmp->xx_set = 0;
  if (tmp->machines_set == 1) {
    int i;
    for (i = 0; i < tmp->machines_length; ++i) {
      machine_free(tmp->machines[i]);
    }
    free(tmp->machines);
    tmp->machines = NULL;
    tmp->machines_set = 0;
    tmp->machines_length = 0;
    tmp->machines_num_allocated = 0;
  }
}

void
pong_free(struct pong *tmp)
{
  if (tmp->machines_set == 1) {
    int i;
    for (i = 0; i < tmp->machines_length; ++i) {
      machine_free(tmp->machines[i]);
    }
    free(tmp->machines);
    tmp->machines = NULL;
    tmp->machines_set = 0;
    tmp->machines_length = 0;
    tmp->machines_num_allocated = 0;
  }
  free(tmp->machines);
  free(tmp);
}

void
pong_marshal(struct evbuffer *evbuf, const struct pong *tmp){
  evtag_marshal_int(evbuf, PONG_VERSION, tmp->version);
  evtag_marshal_int(evbuf, PONG_XX, tmp->xx);
  if (tmp->machines_set) {
    {
      int i;
      for (i = 0; i < tmp->machines_length; ++i) {
    evtag_marshal_machine(evbuf, PONG_MACHINES, tmp->machines[i]);
      }
    }
  }
}

int
pong_unmarshal(struct pong *tmp,  struct evbuffer *evbuf)
{
  ev_uint32_t tag;
  while (evbuffer_get_length(evbuf) > 0) {
    if (evtag_peek(evbuf, &tag) == -1)
      return (-1);
    switch (tag) {

      case PONG_VERSION:

        if (tmp->version_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, PONG_VERSION, &tmp->version) == -1) {
          event_warnx("%s: failed to unmarshal version", __func__);
          return (-1);
        }
        tmp->version_set = 1;
        break;

      case PONG_XX:

        if (tmp->xx_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, PONG_XX, &tmp->xx) == -1) {
          event_warnx("%s: failed to unmarshal xx", __func__);
          return (-1);
        }
        tmp->xx_set = 1;
        break;

      case PONG_MACHINES:

        if (tmp->machines_length >= tmp->machines_num_allocated &&
            pong_machines_expand_to_hold_more(tmp) < 0) {
          puts("HEY NOW");
          return (-1);
        }
        tmp->machines[tmp->machines_length] = machine_new();
        if (tmp->machines[tmp->machines_length] == NULL)
          return (-1);
        if (evtag_unmarshal_machine(evbuf, PONG_MACHINES, tmp->machines[tmp->machines_length]) == -1) {
          event_warnx("%s: failed to unmarshal machines", __func__);
          return (-1);
        }
        ++tmp->machines_length;
        tmp->machines_set = 1;
        break;

      default:
        return -1;
    }
  }

  if (pong_complete(tmp) == -1)
    return (-1);
  return (0);
}

int
pong_complete(struct pong *msg)
{
  if (!msg->version_set)
    return (-1);
  if (!msg->xx_set)
    return (-1);
  {
    int i;
    for (i = 0; i < msg->machines_length; ++i) {
      if (msg->machines_set && machine_complete(msg->machines[i]) == -1)
        return (-1);
    }
  }
  return (0);
}

int
evtag_unmarshal_pong(struct evbuffer *evbuf, ev_uint32_t need_tag, struct pong *msg)
{
  ev_uint32_t tag;
  int res = -1;

  struct evbuffer *tmp = evbuffer_new();

  if (evtag_unmarshal(evbuf, &tag, tmp) == -1 || tag != need_tag)
    goto error;

  if (pong_unmarshal(msg, tmp) == -1)
    goto error;

  res = 0;

 error:
  evbuffer_free(tmp);
  return (res);
}

void
evtag_marshal_pong(struct evbuffer *evbuf, ev_uint32_t tag, const struct pong *msg)
{
  struct evbuffer *_buf = evbuffer_new();
  assert(_buf != NULL);
  pong_marshal(_buf, msg);
  evtag_marshal_buffer(evbuf, tag, _buf);
   evbuffer_free(_buf);
}

/*
 * Implementation of file_stat
 */

static struct file_stat_access_ __file_stat_base = {
  file_stat_inode_assign,
  file_stat_inode_get,
  file_stat_size_assign,
  file_stat_size_get,
};

struct file_stat *
file_stat_new(void)
{
  return file_stat_new_with_arg(NULL);
}

struct file_stat *
file_stat_new_with_arg(void *unused)
{
  struct file_stat *tmp;
  if ((tmp = malloc(sizeof(struct file_stat))) == NULL) {
    event_warn("%s: malloc", __func__);
    return (NULL);
  }
  tmp->base = &__file_stat_base;

  tmp->inode = 0;
  tmp->inode_set = 0;

  tmp->size = 0;
  tmp->size_set = 0;

  return (tmp);
}



int
file_stat_inode_assign(struct file_stat *msg, const ev_uint32_t value)
{
  msg->inode_set = 1;
  msg->inode = value;
  return (0);
}

int
file_stat_size_assign(struct file_stat *msg, const ev_uint32_t value)
{
  msg->size_set = 1;
  msg->size = value;
  return (0);
}

int
file_stat_inode_get(struct file_stat *msg, ev_uint32_t *value)
{
  if (msg->inode_set != 1)
    return (-1);
  *value = msg->inode;
  return (0);
}

int
file_stat_size_get(struct file_stat *msg, ev_uint32_t *value)
{
  if (msg->size_set != 1)
    return (-1);
  *value = msg->size;
  return (0);
}

void
file_stat_clear(struct file_stat *tmp)
{
  tmp->inode_set = 0;
  tmp->size_set = 0;
}

void
file_stat_free(struct file_stat *tmp)
{
  free(tmp);
}

void
file_stat_marshal(struct evbuffer *evbuf, const struct file_stat *tmp){
  evtag_marshal_int(evbuf, FILE_STAT_INODE, tmp->inode);
  evtag_marshal_int(evbuf, FILE_STAT_SIZE, tmp->size);
}

int
file_stat_unmarshal(struct file_stat *tmp,  struct evbuffer *evbuf)
{
  ev_uint32_t tag;
  while (evbuffer_get_length(evbuf) > 0) {
    if (evtag_peek(evbuf, &tag) == -1)
      return (-1);
    switch (tag) {

      case FILE_STAT_INODE:

        if (tmp->inode_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, FILE_STAT_INODE, &tmp->inode) == -1) {
          event_warnx("%s: failed to unmarshal inode", __func__);
          return (-1);
        }
        tmp->inode_set = 1;
        break;

      case FILE_STAT_SIZE:

        if (tmp->size_set)
          return (-1);
        if (evtag_unmarshal_int(evbuf, FILE_STAT_SIZE, &tmp->size) == -1) {
          event_warnx("%s: failed to unmarshal size", __func__);
          return (-1);
        }
        tmp->size_set = 1;
        break;

      default:
        return -1;
    }
  }

  if (file_stat_complete(tmp) == -1)
    return (-1);
  return (0);
}

int
file_stat_complete(struct file_stat *msg)
{
  if (!msg->inode_set)
    return (-1);
  if (!msg->size_set)
    return (-1);
  return (0);
}

int
evtag_unmarshal_file_stat(struct evbuffer *evbuf, ev_uint32_t need_tag, struct file_stat *msg)
{
  ev_uint32_t tag;
  int res = -1;

  struct evbuffer *tmp = evbuffer_new();

  if (evtag_unmarshal(evbuf, &tag, tmp) == -1 || tag != need_tag)
    goto error;

  if (file_stat_unmarshal(msg, tmp) == -1)
    goto error;

  res = 0;

 error:
  evbuffer_free(tmp);
  return (res);
}

void
evtag_marshal_file_stat(struct evbuffer *evbuf, ev_uint32_t tag, const struct file_stat *msg)
{
  struct evbuffer *_buf = evbuffer_new();
  assert(_buf != NULL);
  file_stat_marshal(_buf, msg);
  evtag_marshal_buffer(evbuf, tag, _buf);
   evbuffer_free(_buf);
}

/*
 * Implementation of stat_request
 */

static struct stat_request_access_ __stat_request_base = {
  stat_request_inode_arr_assign,
  stat_request_inode_arr_get,
  stat_request_inode_arr_add,
};

struct stat_request *
stat_request_new(void)
{
  return stat_request_new_with_arg(NULL);
}

struct stat_request *
stat_request_new_with_arg(void *unused)
{
  struct stat_request *tmp;
  if ((tmp = malloc(sizeof(struct stat_request))) == NULL) {
    event_warn("%s: malloc", __func__);
    return (NULL);
  }
  tmp->base = &__stat_request_base;

  tmp->inode_arr = NULL;
  tmp->inode_arr_length = 0;
  tmp->inode_arr_num_allocated = 0;
  tmp->inode_arr_set = 0;

  return (tmp);
}

static int
stat_request_inode_arr_expand_to_hold_more(struct stat_request *msg)
{
  int tobe_allocated = msg->inode_arr_num_allocated;
  ev_uint32_t* new_d_ata = NULL;
  tobe_allocated = !tobe_allocated ? 1 : tobe_allocated << 1;
  new_d_ata = (ev_uint32_t*) realloc(msg->inode_arr,
      tobe_allocated * sizeof(ev_uint32_t));
  if (new_d_ata == NULL)
    return -1;
  msg->inode_arr = new_d_ata;
  msg->inode_arr_num_allocated = tobe_allocated;
  return 0;}

ev_uint32_t *
stat_request_inode_arr_add(struct stat_request *msg, const ev_uint32_t value)
{
  if (++msg->inode_arr_length >= msg->inode_arr_num_allocated) {
    if (stat_request_inode_arr_expand_to_hold_more(msg)<0)
      goto error;
  }
  msg->inode_arr[msg->inode_arr_length - 1] = value;
  msg->inode_arr_set = 1;
  return &(msg->inode_arr[msg->inode_arr_length - 1]);
error:
  --msg->inode_arr_length;
  return (NULL);
}

int
stat_request_inode_arr_assign(struct stat_request *msg, int off,
    const ev_uint32_t value)
{
  if (!msg->inode_arr_set || off < 0 || off >= msg->inode_arr_length)
    return (-1);

  {
    msg->inode_arr[off] = value;
  }
  return (0);
}

int
stat_request_inode_arr_get(struct stat_request *msg, int offset,
    ev_uint32_t *value)
{
  if (!msg->inode_arr_set || offset < 0 || offset >= msg->inode_arr_length)
    return (-1);
  *value = msg->inode_arr[offset];
  return (0);
}

void
stat_request_clear(struct stat_request *tmp)
{
  if (tmp->inode_arr_set == 1) {
    free(tmp->inode_arr);
    tmp->inode_arr = NULL;
    tmp->inode_arr_set = 0;
    tmp->inode_arr_length = 0;
    tmp->inode_arr_num_allocated = 0;
  }
}

void
stat_request_free(struct stat_request *tmp)
{
  if (tmp->inode_arr_set == 1) {
    free(tmp->inode_arr);
    tmp->inode_arr = NULL;
    tmp->inode_arr_set = 0;
    tmp->inode_arr_length = 0;
    tmp->inode_arr_num_allocated = 0;
  }
  free(tmp->inode_arr);
  free(tmp);
}

void
stat_request_marshal(struct evbuffer *evbuf, const struct stat_request *tmp){
  if (tmp->inode_arr_set) {
    {
      int i;
      for (i = 0; i < tmp->inode_arr_length; ++i) {
    evtag_marshal_int(evbuf, STAT_REQUEST_INODE_ARR, tmp->inode_arr[i]);
      }
    }
  }
}

int
stat_request_unmarshal(struct stat_request *tmp,  struct evbuffer *evbuf)
{
  ev_uint32_t tag;
  while (evbuffer_get_length(evbuf) > 0) {
    if (evtag_peek(evbuf, &tag) == -1)
      return (-1);
    switch (tag) {

      case STAT_REQUEST_INODE_ARR:

        if (tmp->inode_arr_length >= tmp->inode_arr_num_allocated &&
            stat_request_inode_arr_expand_to_hold_more(tmp) < 0) {
          puts("HEY NOW");
          return (-1);
        }
        if (evtag_unmarshal_int(evbuf, STAT_REQUEST_INODE_ARR, &tmp->inode_arr[tmp->inode_arr_length]) == -1) {
          event_warnx("%s: failed to unmarshal inode_arr", __func__);
          return (-1);
        }
        ++tmp->inode_arr_length;
        tmp->inode_arr_set = 1;
        break;

      default:
        return -1;
    }
  }

  if (stat_request_complete(tmp) == -1)
    return (-1);
  return (0);
}

int
stat_request_complete(struct stat_request *msg)
{
  return (0);
}

int
evtag_unmarshal_stat_request(struct evbuffer *evbuf, ev_uint32_t need_tag, struct stat_request *msg)
{
  ev_uint32_t tag;
  int res = -1;

  struct evbuffer *tmp = evbuffer_new();

  if (evtag_unmarshal(evbuf, &tag, tmp) == -1 || tag != need_tag)
    goto error;

  if (stat_request_unmarshal(msg, tmp) == -1)
    goto error;

  res = 0;

 error:
  evbuffer_free(tmp);
  return (res);
}

void
evtag_marshal_stat_request(struct evbuffer *evbuf, ev_uint32_t tag, const struct stat_request *msg)
{
  struct evbuffer *_buf = evbuffer_new();
  assert(_buf != NULL);
  stat_request_marshal(_buf, msg);
  evtag_marshal_buffer(evbuf, tag, _buf);
   evbuffer_free(_buf);
}

/*
 * Implementation of stat_response
 */

static struct stat_response_access_ __stat_response_base = {
  stat_response_stat_arr_assign,
  stat_response_stat_arr_get,
  stat_response_stat_arr_add,
};

struct stat_response *
stat_response_new(void)
{
  return stat_response_new_with_arg(NULL);
}

struct stat_response *
stat_response_new_with_arg(void *unused)
{
  struct stat_response *tmp;
  if ((tmp = malloc(sizeof(struct stat_response))) == NULL) {
    event_warn("%s: malloc", __func__);
    return (NULL);
  }
  tmp->base = &__stat_response_base;

  tmp->stat_arr = NULL;
  tmp->stat_arr_length = 0;
  tmp->stat_arr_num_allocated = 0;
  tmp->stat_arr_set = 0;

  return (tmp);
}

static int
stat_response_stat_arr_expand_to_hold_more(struct stat_response *msg)
{
  int tobe_allocated = msg->stat_arr_num_allocated;
  struct file_stat** new_d_ata = NULL;
  tobe_allocated = !tobe_allocated ? 1 : tobe_allocated << 1;
  new_d_ata = (struct file_stat**) realloc(msg->stat_arr,
      tobe_allocated * sizeof(struct file_stat*));
  if (new_d_ata == NULL)
    return -1;
  msg->stat_arr = new_d_ata;
  msg->stat_arr_num_allocated = tobe_allocated;
  return 0;}

struct file_stat* 
stat_response_stat_arr_add(struct stat_response *msg)
{
  if (++msg->stat_arr_length >= msg->stat_arr_num_allocated) {
    if (stat_response_stat_arr_expand_to_hold_more(msg)<0)
      goto error;
  }
  msg->stat_arr[msg->stat_arr_length - 1] = file_stat_new();
  if (msg->stat_arr[msg->stat_arr_length - 1] == NULL)
    goto error;
  msg->stat_arr_set = 1;
  return (msg->stat_arr[msg->stat_arr_length - 1]);
error:
  --msg->stat_arr_length;
  return (NULL);
}

int
stat_response_stat_arr_assign(struct stat_response *msg, int off,
    const struct file_stat* value)
{
  if (!msg->stat_arr_set || off < 0 || off >= msg->stat_arr_length)
    return (-1);

  {
    int had_error = 0;
    struct evbuffer *tmp = NULL;
    file_stat_clear(msg->stat_arr[off]);
    if ((tmp = evbuffer_new()) == NULL) {
      event_warn("%s: evbuffer_new()", __func__);
      had_error = 1;
      goto done;
    }
    file_stat_marshal(tmp, value);
    if (file_stat_unmarshal(msg->stat_arr[off], tmp) == -1) {
      event_warnx("%s: file_stat_unmarshal", __func__);
      had_error = 1;
      goto done;
    }
    done:if (tmp != NULL)
      evbuffer_free(tmp);
    if (had_error) {
      file_stat_clear(msg->stat_arr[off]);
      return (-1);
    }
  }
  return (0);
}

int
stat_response_stat_arr_get(struct stat_response *msg, int offset,
    struct file_stat* *value)
{
  if (!msg->stat_arr_set || offset < 0 || offset >= msg->stat_arr_length)
    return (-1);
  *value = msg->stat_arr[offset];
  return (0);
}

void
stat_response_clear(struct stat_response *tmp)
{
  if (tmp->stat_arr_set == 1) {
    int i;
    for (i = 0; i < tmp->stat_arr_length; ++i) {
      file_stat_free(tmp->stat_arr[i]);
    }
    free(tmp->stat_arr);
    tmp->stat_arr = NULL;
    tmp->stat_arr_set = 0;
    tmp->stat_arr_length = 0;
    tmp->stat_arr_num_allocated = 0;
  }
}

void
stat_response_free(struct stat_response *tmp)
{
  if (tmp->stat_arr_set == 1) {
    int i;
    for (i = 0; i < tmp->stat_arr_length; ++i) {
      file_stat_free(tmp->stat_arr[i]);
    }
    free(tmp->stat_arr);
    tmp->stat_arr = NULL;
    tmp->stat_arr_set = 0;
    tmp->stat_arr_length = 0;
    tmp->stat_arr_num_allocated = 0;
  }
  free(tmp->stat_arr);
  free(tmp);
}

void
stat_response_marshal(struct evbuffer *evbuf, const struct stat_response *tmp){
  if (tmp->stat_arr_set) {
    {
      int i;
      for (i = 0; i < tmp->stat_arr_length; ++i) {
    evtag_marshal_file_stat(evbuf, STAT_RESPONSE_STAT_ARR, tmp->stat_arr[i]);
      }
    }
  }
}

int
stat_response_unmarshal(struct stat_response *tmp,  struct evbuffer *evbuf)
{
  ev_uint32_t tag;
  while (evbuffer_get_length(evbuf) > 0) {
    if (evtag_peek(evbuf, &tag) == -1)
      return (-1);
    switch (tag) {

      case STAT_RESPONSE_STAT_ARR:

        if (tmp->stat_arr_length >= tmp->stat_arr_num_allocated &&
            stat_response_stat_arr_expand_to_hold_more(tmp) < 0) {
          puts("HEY NOW");
          return (-1);
        }
        tmp->stat_arr[tmp->stat_arr_length] = file_stat_new();
        if (tmp->stat_arr[tmp->stat_arr_length] == NULL)
          return (-1);
        if (evtag_unmarshal_file_stat(evbuf, STAT_RESPONSE_STAT_ARR, tmp->stat_arr[tmp->stat_arr_length]) == -1) {
          event_warnx("%s: failed to unmarshal stat_arr", __func__);
          return (-1);
        }
        ++tmp->stat_arr_length;
        tmp->stat_arr_set = 1;
        break;

      default:
        return -1;
    }
  }

  if (stat_response_complete(tmp) == -1)
    return (-1);
  return (0);
}

int
stat_response_complete(struct stat_response *msg)
{
  {
    int i;
    for (i = 0; i < msg->stat_arr_length; ++i) {
      if (msg->stat_arr_set && file_stat_complete(msg->stat_arr[i]) == -1)
        return (-1);
    }
  }
  return (0);
}

int
evtag_unmarshal_stat_response(struct evbuffer *evbuf, ev_uint32_t need_tag, struct stat_response *msg)
{
  ev_uint32_t tag;
  int res = -1;

  struct evbuffer *tmp = evbuffer_new();

  if (evtag_unmarshal(evbuf, &tag, tmp) == -1 || tag != need_tag)
    goto error;

  if (stat_response_unmarshal(msg, tmp) == -1)
    goto error;

  res = 0;

 error:
  evbuffer_free(tmp);
  return (res);
}

void
evtag_marshal_stat_response(struct evbuffer *evbuf, ev_uint32_t tag, const struct stat_response *msg)
{
  struct evbuffer *_buf = evbuffer_new();
  assert(_buf != NULL);
  stat_response_marshal(_buf, msg);
  evtag_marshal_buffer(evbuf, tag, _buf);
   evbuffer_free(_buf);
}

