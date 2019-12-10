#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>

#define QueueElemType size_t
typedef struct QueueNode queueNode;
struct QueueNode
{
    QueueElemType data;
    queueNode *next;
};
typedef struct Queue
{
    queueNode *head, *tail;
    size_t size;
} queue;

void enqueue(queue *q, QueueElemType e)
{
    queueNode *newNode = (queueNode *)malloc(sizeof(queueNode));
    newNode->data = e;
    newNode->next = NULL;
    if (q->tail == NULL)
        q->head = newNode;
    else
        q->tail->next = newNode;
    q->tail = newNode;
    ++(q->size);
}

QueueElemType dequeue(queue *q)
{
    if (q->head == NULL)
        return -1;
    QueueElemType ret = q->head->data;
    if (q->tail == q->head)
        q->tail = NULL;
    queueNode *del = q->head;
    q->head = q->head->next;
    free(del);
    --(q->size);
    return ret;
}

void clear(queue *q)
{
    while (q->size)
        dequeue(q);
}

#define INIT_QUEUE(q)                       \
    do                                      \
    {                                       \
        q = (queue *)malloc(sizeof(queue)); \
        q->head = q->tail = NULL;           \
        q->size = 0;                        \
    } while (0)
#define DESTROY_QUEUE(q) \
    do                   \
    {                    \
        clear(q);        \
        free(q);         \
        q = NULL;        \
    } while (0)
#endif