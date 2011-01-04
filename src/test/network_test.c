#include<stdio.h>
#include "network.h"

int main(){
    printf("hello world\n");
    server_socket("localhost", "9991");

    client_socket("localhost", "9991");
    sleep(10);
}
