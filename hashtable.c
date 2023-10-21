// Copyright 2022 <Tudor Ioana Octavia>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "utils.h"

Hashtable* create_hashtable(int size, int (*hash)(void*, int, int),
    int (*cmp)(void*, int, void*, int))
{   // creez hashtable generic
    Hashtable* h = (Hashtable*)malloc(sizeof(Hashtable));
    h->size = size;
    h->entries = 0;
    h->hash = hash;
    h->cmp = cmp;
    h->buckets = (List**)malloc(size * sizeof(List*));
    for (int i = 0; i < size; i++) {
        h->buckets[i] = create_list(cmp_entries);
    }
    return h;
}

Entry* create_entry(void* key, int key_len, void* val, int val_len)
{
    // creez un entry
    Entry* e = (Entry*)malloc(sizeof(Entry));
    e->key_len = key_len;
    e->val_len = val_len;
    e->key = malloc(key_len);
    memcpy(e->key, key, key_len);
    e->val = malloc(val_len);
    memcpy(e->val, val, val_len);
    return e;
}

int cmp_entries(void* a_p, int a_len, void* b_p, int b_len)
{
    // functie de comprare pentru perechi key-val
    Entry* a = (Entry*)a_p;
    Entry* b = (Entry*)b_p;
    // a_len = 0;
    // b_len = 0;
    if (a_len != a->key_len || b_len != b->key_len)
        return -1;
    return cmp(a->key, a->key_len, b->key, b->key_len);
}

void add_hashtable(Hashtable **h, void* key, int key_len,
    void* val, int val_len)
{
    int index = (*h)->hash(key, key_len, (*h)->size);
    // calculez indexul corespunzator cheii si ma duc in lista respectiva
    for (Node* it = (*h)->buckets[index]->head; it != NULL; it = it->next) {
        Entry* e = (Entry*)it->data;
        // daca gasesc deja cheia in lista doar dau free valorii
        // si inlocuiesc valoarea cu cea noua
        if ((*h)->cmp(e->key, e->key_len, key, key_len) == 0) {
            free(e->val);
            e->val = malloc(val_len);
            e->val_len = val_len;
            memcpy(e->val , val, val_len);
            return;
        }
    }
    // daca nu exista cheia deja in lista creez un Entry*e
    // si adaug pe e in lista
    Entry* e = create_entry(key, key_len, val, val_len);
    push_front_list((*h)->buckets[index], e, sizeof(Entry));
    free(e);
    // trebuie dat free pentru ca il mai aloc o data
    // in push_front_list
    (*h)->entries++;  // creste entries
    // verific daca am mai multe entries decat size
    // ca sa vad daca trebuie apelat resize
    if ((*h)->entries > (*h)->size) {
        resize_hashtable(h, (*h)->size * 2);
    }
}

void* find_hashtable(Hashtable *h, void* key, int key_len)
{
    // calculez indexul corespunzator cheii si ma duc in lista respectiva
    int index = h->hash(key, key_len, h->size);
    for (Node* it = h->buckets[index]->head; it != NULL; it = it->next) {
        Entry* e = (Entry*)it->data;
        if (h->cmp(e->key, e->key_len, key, key_len) == 0) {
            return e->val;  // imi returneaza valoarea cheii
        }
    }
    return NULL;
}


void delete_hashtable(Hashtable *h, void* key, int key_len)
{
     int index = h->hash(key, key_len, h->size);
     if (h->buckets[index]->size == 0) {
         return;
     }
     // caz cand key ul e in primul nod din lista
     Entry* e = (Entry*)h->buckets[index]->head->data;
     if (h->cmp(e->key, e->key_len, key, key_len) == 0) {
         free(e->key);
         free(e->val);
         pop_front_list(h->buckets[index]);
         h->entries--;
         return;
     }
     // caz cand nu e primu din lista nodul lui key
    for (Node* it = h->buckets[index]->head; it->next != NULL; it = it->next) {
           e = (Entry*)it->next->data;
        if (h->cmp(e->key, e->key_len, key, key_len) == 0) {
            free(e->key);
            free(e->val);
            // eliberez e->key si e->val
            Node* removed = it->next;
            it->next = it->next->next;
            h->buckets[index]->size--;
            free_node(removed);
            // scoot nodul din lista si decrementez h->entries
            h->entries--;
            return;
        }
    }
}


void free_hashtable(Hashtable* h)
{
    for (int i = 0; i < h->size; i++) {
        // parcurg toate listele prin toate nodurile
        for (Node* it = h->buckets[i]->head; it != NULL; it = it->next) {
            Entry* e = (Entry*)it->data;
            free(e->key);
            free(e->val);
            // eliberez mai intai e->key si e->val
        }
        free_list(h->buckets[i]);  // apoi lista
    }
    free(h->buckets);
    free(h);
}

void resize_hashtable(Hashtable** h, int new_size)
{
    // imi creez un nou hashtable cu noile dimensiuni
    Hashtable* new_h = create_hashtable(new_size, (*h)->hash, (*h)->cmp);
    for (int i = 0 ; i < (*h)->size; i++) {
        // parcurg hashtable ul vechi prin toate listele si fiecare nod
        // si adaug e->key si e->val la hashtable-ul nou
        for (Node* it = (*h)->buckets[i]->head; it != NULL; it = it->next) {
            Entry* e = (Entry*)it->data;
            add_hashtable(&new_h, e->key, e->key_len, e->val, e->val_len);
        }
    }
    free_hashtable(*h);  // ii eliberez pe hashtable-ul vechi
    *h = new_h;
}
