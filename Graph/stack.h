#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>

#define INIT_SIZE 256

#define StackElemType size_t

typedef struct
{
    StackElemType *s;
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
        c->s = (StackElemType *)realloc(c->s, c->size);
    }
    ret = c->s + c->top;
    c->top += size;
    return ret;
}

void *pop(stack *c, size_t size) { return c->s + (c->top -= size); }

#define INIT_STACK(c)                       \
    do                                      \
    {                                       \
        c = (stack *)malloc(sizeof(stack)); \
        c->s = NULL;                        \
        c->size = c->top = 0;               \
    } while (0)

#define DESTROY_STACK(c) \
    do                   \
    {                    \
        free(c->s);      \
        free(c);         \
        c = NULL;        \
    } while (0);

#define DEFAULT_SIZE sizeof(StackElemType)
#define PUSH(c, e) CAST_PUSH(c, DEFAULT_SIZE) = e
#define CAST_PUSH(c, size) *(StackElemType *)push(c, size)
#define POP(c) CAST_POP(c, DEFAULT_SIZE)
#define CAST_POP(c, size) *(StackElemType *)pop(c, size)
#endif