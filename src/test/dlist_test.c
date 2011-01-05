#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "network.h"
#include "dlist.h"
#include "log.h"

dlist_t head_v;
dlist_t * head = & head_v;

typedef struct {
    char * str;
    dlist_t queue;
}dlist_entry;

int main(){
    int i;
    dlist_t * p;
    dlist_entry * e;
    dlist_entry entries [10];

    dlist_init(head);
    for (i=0; i<10; i++){
        entries[i].str = (char *)malloc(100);
        sprintf(entries[i].str, "entry %d", i);
        dlist_insert_after(head, & (entries[i].queue) );
    }

    for (p = head->next; p!=head; p=p->next){
        //offsetof(struct dlist_entry, queue);
        e = dlist_data(p, dlist_entry, queue);
        printf("print : %s\n", e->str);
    }
}

