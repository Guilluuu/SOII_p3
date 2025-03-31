#ifndef BUFFER_H
#define BUFFER_H

#define BSIZE 64

typedef struct
{
    int   length;
    char  data[BSIZE];
    char *out_ptr;
    char *in_ptr;
} Buffer;

char buffer_add(char);

char buffer_pop();

char buffer_top();

void buffer_print();

int buffer_get_in_index();

int buffer_get_out_index();


#endif
