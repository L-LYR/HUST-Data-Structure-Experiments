#ifndef SQLIST_H_
#define SQLIST_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define SQLIST_INIT_SIZE 100
#define LISTINCREMENT 10

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

typedef struct sqlist
{
    elemType *elem;
    uint32_t length;
    uint32_t size;
} sqList;

//initialize a sqlist
status InitList(sqList **L);

//destroy the sqlist
status DestroyList(sqList **L);

//clear the sqlist
status ClearList(sqList *L);

//judge whether the sqlist is empty
status ListEmpty(sqList *L);

//get the length of the sqList
uint32_t ListLength(sqList *L);

//get the element with index i in the sqlist
status GetElem(sqList *L, uint32_t i, elemType *e);

//locate the element equal to e in the sqlist
status LocateElem(sqList *L, elemType e, uint32_t *i,
                  bool (*cmp)(elemType, elemType));
//get the prior element of the current element
status PriorElem(sqList *L, elemType cur_e, elemType *pre_e,
                 bool (*cmp)(elemType, elemType));

//get the next element of the current element
status NextElem(sqList *L, elemType cur_e, elemType *next_e,
                bool (*cmp)(elemType, elemType));

//insert element e into the i-th position
status ListInsert(sqList *L, uint32_t i, elemType e);

//delete the i-th element and return it
status ListDelete(sqList *L, uint32_t i, elemType *e);

//traverse the whole sqlist
status ListTraverse(sqList *L, status (*visit)(sqList *, uint32_t));

//save the sqlist in file
status SaveInFile(sqList *L, const char *fileName,
                  status (*visit)(sqList *, uint32_t));

//parse the sqlist file
status ParseFromFile(sqList **L, const char *fileName);
#endif