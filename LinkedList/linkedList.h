#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//status code
typedef enum
{
    FALSE,
    TRUE,
    OVERFLOW = -3,
    INFEASIBLE,
    ERROR,
    OK = 1,
} status;

typedef int elemType;

typedef struct ListNode listNode;
struct ListNode
{
    elemType data;
    listNode *next;
};
typedef struct LinkedList
{
    listNode *head;
    uint32_t length;
} linkedList;

//initialize a linkedList
status InitList(linkedList **L);

//destroy the linkedList
status DestroyList(linkedList **L);

//clear the linkedList
status ClearList(linkedList *L);

//judge whether the linkedList is empty
status ListEmpty(linkedList *L);

//get the length of the linkedList
uint32_t ListLength(linkedList *L);

//get the element with index i in the linkedList
status GetElem(linkedList *L, uint32_t i, elemType *e);

//locate the element equal to e in the linkedList
status LocateElem(linkedList *L, elemType e, uint32_t *i,
                  bool (*cmp)(elemType, elemType));
//get the prior element of the current element
status PriorElem(linkedList *L, elemType cur_e, elemType *pre_e,
                 bool (*cmp)(elemType, elemType));

//get the next element of the current element
status NextElem(linkedList *L, elemType cur_e, elemType *next_e,
                bool (*cmp)(elemType, elemType));

//insert element e into the i-th position
status ListInsert(linkedList *L, uint32_t i, elemType e);

//delete the i-th element and return it
status ListDelete(linkedList *L, uint32_t i, elemType *e);

//traverse the whole linkedList
status ListTraverse(linkedList *L, status (*visit)(listNode *));

//save the linkedList in file
status SaveInFile(linkedList *L, const char *fileName,
                  status (*visit)(listNode *));

//parse the linkedList file
status ParseFromFile(linkedList **L, const char *fileName);

#endif