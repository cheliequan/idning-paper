#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>
#include "log.h"

#define  LOG_DEUBG 1
#define  LOG_INFO 2
#define  LOG_WARN 3
#define  LOG_ERROR 4

static int puttime(char ** buf){
    struct timeval tv;
    time_t curtime;

    gettimeofday(&tv, NULL); 
    curtime=tv.tv_sec;

    strftime(*buf,30,"%m-%d-%Y %T.", localtime(&curtime));
    sprintf(*buf+20, "%06ld ", tv.tv_usec);
    while(**buf)
        (*buf)++;
    return 0;
}

static int putlevel(char ** buf, int level){
    switch (level){
        case LOG_DEUBG:
            strcpy(*buf, "[DEBUG] ");
            *buf += strlen("[DEBUG] ");
            return 0;
        case LOG_INFO:
            strcpy(*buf, "[INFO] ");
            *buf += strlen("[INFO] ");
            return 0;
        case LOG_WARN:
            strcpy(*buf, "[WARN] ");
            *buf += strlen("[WARN] ");
            return 0;
        case LOG_ERROR:
            strcpy(*buf, "[ERROR] ");
            *buf += strlen("[ERROR] ");
            return 0;
        default:
            strcpy(*buf, "[UNKNOWN] ");
            *buf += strlen("[UNKNOWN] ");
            return 0;
    }
}

int logging(int level, char * fmt, ...){
    char stmp[10240];
    char *buf = stmp;
    puttime(&buf);
    putlevel(&buf, level);

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(stmp) -30 , fmt, ap);
    va_end(ap);
    printf("%s\n", stmp);
}

