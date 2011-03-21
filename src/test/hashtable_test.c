#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

#include "network.h"
#include "hashtable.h"
#include "log.h"




int main(){
    Hashtable *ht = hashtable_new(hash_int_hash, hash_int_equal, 2);
    int arr[10];
    int k;
    char v[20];
    for (k=0; k< 10; k++){
        arr[k] = k;
        sprintf(v, "v%d", k);
        hashtable_insert(ht, arr+k, strdup(v)); //!!!!!!!!!!!!!!! I Should take care of Memory myself 
    }

    hashtable_foreach(ht, hash_int_str_foreach, NULL);

    for (k=0; k< 10; k++){
        char * p = hashtable_lookup(ht, &k);
        printf("%s\n", p);
        sprintf(v, "v%d", k);
        assert(0 == strcmp(p, v));
    }
    k = 3;
    hashtable_remove(ht, &k);
    char * p = hashtable_lookup(ht, &k);
    printf("%p\n", p);
    assert(NULL == p);

    return 0;
}

