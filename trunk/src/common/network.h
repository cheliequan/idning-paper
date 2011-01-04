#include <stdint.h>

int server_socket(char * host, char * port);
int client_socket(char * host, char * port);


int tcpnonblock(int sock) ;
int tcpreuseaddr(int sock) ;
int tcpnodelay(int sock) ;

int tcpaccept(int sock);
int tcpclose(int sock);

int32_t tcpread(int sock,void *buff,uint32_t leng, uint32_t msecto);
int32_t tcpwrite(int sock,const void *buff,uint32_t leng, uint32_t msecto);




