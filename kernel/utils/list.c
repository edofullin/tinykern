#include "list.h"
#include "riscv_const.h"
#include "types.h"


void list_init(list_t* list) {
    list->first = NULL;
    list->last = NULL;
}


void list_append(list_t* list, void* element) {
    list_node_t* n = list->last;

    list_node_t* new = NULL; //kalloc(sizeof(list_node_t));
    new->data = element;

    new->prev = list->last;
    new->next = NULL;

    list->last->next = new;
}

void list_remove(list_t* list, void* element) {
    list_node_t* n = list->first;

    if(n->data == element) {
        list->first = n->next;
        n->next->prev = NULL;
        // TODO kfree(n)
        return;
    }

    while(n) {
        if(n->data == element) {
            n->prev->next = n->next;
            n->next->prev = n->prev;
            // TODO kfree(n)
        }

        n = n->next;
    }
}