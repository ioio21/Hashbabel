// Copyright 2022 <Tudor Ioana Octavia>
#ifndef LIST_H_
#define LIST_H_


typedef struct Node {
    void* data;
    struct Node* next;
    int data_size;
} Node;

typedef struct List {
    Node* head;
    int size;
    int (*cmp)(void*, int, void*, int);
} List;

Node* create_node(void *data, int data_size, Node* next);
void free_node(Node* node);


List * create_list(int (*cmp)(void*, int, void*, int));

void push_front_list(List* l, void* data, int data_size);
void pop_front_list(List* l);
void* find_list(List* l, void* data, int data_size);
void free_list(List* l);




#endif  // LIST_H_
