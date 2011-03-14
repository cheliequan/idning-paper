
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <sys/time.h>
#include <sys/resource.h>

#include "log.h"
#include "cfg.h"


#define MAX_FILES 10000
#define APP_VERSION "0.0.1"

int init_app(int argc,char **argv, char * appname) ;
