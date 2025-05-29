#pragma once

typedef struct n {
    struct n* next;
    struct n* prev;

    void* data;
} list_node_t;

typedef struct {
    list_node_t* first;
    list_node_t* last;
} list_t;

void list_init(list_t* list);
void list_append(list_t* list, void* element);
void list_remove(list_t* list, void* element);

