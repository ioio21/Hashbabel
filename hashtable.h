// Copyright 2022 <Tudor Ioana Octavia>
#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include "list.h"
typedef struct Entry {
    void* key;
    int key_len;
    void* val;
    int val_len;
} Entry;

typedef struct Hashtable {
    int size;
    int entries;
    List** buckets;
    int (*hash)(void*, int, int);
    int (*cmp)(void*, int, void*, int);
} Hashtable;
Hashtable* create_hashtable(int size, int (*hash)(void*, int, int),
                                    int (*cmp)(void*, int, void*, int));
int cmp_entries(void* a_p, int a_len, void* b_p, int b_len);
Entry* create_entry(void* key, int key_len, void* val, int val_len);
void add_hashtable(Hashtable **h, void* key, int key_len, void* val,
                                                            int val_len);
void* find_hashtable(Hashtable *h, void* key, int key_len);
void delete_hashtable(Hashtable *h, void* key, int key_len);
void resize_hashtable(Hashtable** h, int new_size);
void free_hashtable(Hashtable* h);


#endif  // HASHTABLE_H_
