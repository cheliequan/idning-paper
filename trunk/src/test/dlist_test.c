#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include "network.h"
#include "dlist.h"
#include "log.h"


typedef struct dlist_entry{
    char * str;
    struct dlist_entry * next;
    struct dlist_entry * prev;
}dlist_entry;

dlist_entry head_v;
dlist_entry * head = & head_v;

int main(){
    int i;
    dlist_entry * p;
    dlist_entry * e;
    dlist_entry entries [10];

    dlist_init(head);
    for (i=0; i<10; i++){
        entries[i].str = (char *)malloc(100);
        sprintf(entries[i].str, "entry %d", i);
        dlist_insert_after(head, entries+i );
    }

    for (p = head->next; p!=head; p=p->next){
        //offsetof(struct dlist_entry, queue);
        /*e = dlist_data(p, dlist_entry, queue);*/
        e = p;
        printf("print : %s\n", e->str);
    }
}

