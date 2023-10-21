// Copyright 2022 <Tudor Ioana Octavia>
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* create_node(void *data, int data_size, Node* next)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = malloc(data_size);
    memcpy(node->data, data, data_size);
    node->next = next;
    return node;
}

void free_node(Node* node)
{
    free(node->data);
    free(node);
}

List * create_list(int (*cmp)(void*, int, void*, int))
{
    List* l = (List*)malloc(sizeof(List));
    l->head = NULL;
    l->size = 0;
    l->cmp = cmp;
    return l;
}

void push_front_list(List* l, void* data, int data_size)
{
    // imi adauga un nod nou ca fiind noul head al listei
    l->head = create_node(data, data_size, l->head);
    l->size++;
}

void pop_front_list(List* l)
{
    Node* removed =  l->head;
    l->head = l->head->next;
    l->size--;
    free_node(removed);
}

void* find_list(List* l, void* data, int data_size)
{
    for (Node* it = l->head; it != NULL; it = it->next) {
        if (l->cmp(data, data_size, it->data, it->data_size) == 0) {
            return it->data;
        }
    }
    return NULL;
}

void free_list(List* l)
{
    while (l->size > 0) {
        pop_front_list(l);
    }
    free(l);
}
