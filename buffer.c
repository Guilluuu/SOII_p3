#include "buffer.h"
#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Buffer char_buf = (Buffer){
    .out_ptr = char_buf.data,
    .in_ptr  = char_buf.data,
    .length  = BSIZE,
    .data    = { 0 },
};

static inline int __out_of_bounds(char *ptr)
{
    return ptr - char_buf.data >= char_buf.length && ptr >= char_buf.data;
}

int __valid_char(char item) { return item; }

inline int buffer_is_full() { return (__valid_char(*char_buf.in_ptr)); }

inline int buffer_is_empty() { return (!__valid_char(*char_buf.out_ptr)); }


static char *__move_forward(char **ptr)
{
    ++*ptr;

    if (__out_of_bounds(*ptr))
        *ptr = char_buf.data;

    if (*ptr == char_buf.in_ptr)
        return NULL;

    if (*ptr == char_buf.out_ptr)
        return NULL;

    return *ptr;
}

static char *__move_ptr(char **ptr, int n)
{
    while (n--)
    {
        if (!__move_forward(ptr))
            return NULL;
    }
    return *ptr;
}


char buffer_add(char kp)
{
    if (buffer_is_full())
    {
        return 0;
    }
    *char_buf.in_ptr = kp;
    __move_ptr(&char_buf.in_ptr, 1);

    return kp;
}

char buffer_pop()
{
    char kp;

    if (buffer_is_empty())
    {
        return 0;
    }


    kp                = *char_buf.out_ptr;
    *char_buf.out_ptr = 0;

    __move_ptr(&char_buf.out_ptr, 1);

    return kp;
}

void buffer_print()
{
    printf("Buff: ");
    for (int i = 0; i < BSIZE; i++)
    {
        if (__valid_char(char_buf.data[i]))
            printf("%c,", char_buf.data[i]);
    }
    printf("\n");
}

inline char buffer_top() { return *char_buf.out_ptr; }

int buffer_get_in_index()
{
    return (int) (char_buf.in_ptr - char_buf.data) - 1;
}

int buffer_get_out_index()
{
    return (int) (char_buf.out_ptr - char_buf.data) - 1;
}
