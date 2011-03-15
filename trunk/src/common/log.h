


#ifndef _LOG_H__
#define _LOG_H__

#define  LOG_DEUBG 1
#define  LOG_INFO 2
#define  LOG_WARN 3
#define  LOG_ERROR 4


int logging(int level, char * fmt, ...);

#define DBG() (fprintf(stderr, "%s:%s: called\n", __FILE__, __func__))



#endif
