#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>

#define INIT_SIZE 256

typedef int elemType;

typedef struct
{
    elemType *s;
    size_t size, top;
} stack;

void *push(stack *c, size_t size)
{
    void *ret;
    if (c->top + size >= c->size)
    {
        if (c->size == 0)
            c->size = INIT_SIZE;
        while (c->top + size >= c->size)
            c->size += c->size >> 1;
        c->s = (elemType *)realloc(c->s, c->size);
    }
    ret = c->s + c->top;
    c->top += size;
    return ret;
}

void *pop(stack *c, size_t size) { return c->s + (c->top -= size); }

#endif