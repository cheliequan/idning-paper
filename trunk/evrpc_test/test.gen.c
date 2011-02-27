/*
 * Automatically generated from test.rpc
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


#include "test.gen.h"

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

