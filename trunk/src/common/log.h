#include<stdio.h>
#include<stdarg.h>

const int LOG_DEUBG = 1;
const int LOG_INFO = 2;
const int LOG_WARN = 3;
const int LOG_ERROR = 4;

int logging(int level, char * fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap); //注意这里要是vprintf
    va_end(ap);
}

