// Copyright 2022 <Tudor Ioana Octavia>
#include "utils.h"

int cmp(void* a_p, int a_len, void* b_p, int b_len)
{
    if (a_len != b_len) {
        return a_len - b_len;
    }
    char* a = (char*)a_p;
    char* b = (char*)b_p;
    for (int i = 0; i < a_len; i++) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
    }
    return 0;
}

int hash(void* data_p, int data_len, int mod)
{
    char* data = (char*)data_p;

    unsigned int hash = 5381;
    int c;

    if (data_len < 0)
        return -1;

    while ((c = *data++) != 0) {
        hash = (((hash << 5) + hash) + c) % mod;
    }
    return hash;
}
