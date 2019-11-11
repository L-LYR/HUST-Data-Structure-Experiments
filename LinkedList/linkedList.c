#include "linkedList.h"
#include <stdlib.h>
#include <string.h>

#define ListNotExistError(L) \
    do                       \
    {                        \
        if (L == NULL)       \
            return ERROR;    \
    } while (0)

//initialize a linkedList
status InitList(linkedList **L)
{
    if (L == NULL)
        return ERROR;
    (*L) = (linkedList *)malloc(sizeof(linkedList));
    if ((*L) == NULL)
        exit(OVERFLOW);
    (*L)->head = NULL;
    (*L)->length = 0;
    return OK;
}
//destroy the linkedList
status DestroyList(linkedList **L)
{
    status ret = ClearList(*L);
    if (ret == OK)
    {
        free(*L);
        *L = NULL;
    }
    return ret;
}
//clear the linkedList
status ClearList(linkedList *L)
{
    ListNotExistError(L);
    listNode *p = L->head, *del = NULL;
    while (p != NULL)
    {
        del = p;
        p = p->next;
        free(del);
    }
    L->head = NULL;
    L->length = 0;
    return OK;
}
//judge whether the linkedList is empty
status ListEmpty(linkedList *L)
{
    ListNotExistError(L);
    return (L->length == 0) ? TRUE : FALSE;
}

//get the length of the linkedList
uint32_t ListLength(linkedList *L)
{
    ListNotExistError(L);
    return L->length;
}

//get the element with index i in the linkedList
status GetElem(linkedList *L, uint32_t i, elemType *e)
{
    ListNotExistError(L);
    if (i < 1 || i > L->length)
        return INFEASIBLE;
    listNode *p = L->head;
    uint32_t k;
    for (k = 0; k < i - 1; ++k)
        p = p->next;
    *e = p->data;
    return OK;
}

//locate the element equal to e in the linkedList
status LocateElem(linkedList *L, elemType e, uint32_t *i,
                  bool (*cmp)(elemType, elemType))
{
    ListNotExistError(L);
    listNode *p = L->head;
    uint32_t cnt = 1;
    while (p != NULL)
    {
        if (cmp(e, p->data))
        {
            *i = cnt;
            return OK;
        }
        else
        {
            p = p->next;
            ++cnt;
        }
    }
    *i = 0;
    return INFEASIBLE;
}
//get the prior element of the current element
status PriorElem(linkedList *L, elemType cur_e, elemType *pre_e,
                 bool (*cmp)(elemType, elemType))
{
    ListNotExistError(L);
    listNode *p = L->head;
    if (p == NULL || cmp(p->data, cur_e))
        return INFEASIBLE;
    while (p->next != NULL)
    {
        if (cmp(p->next->data, cur_e))
        {
            *pre_e = p->data;
            return OK;
        }
        else
            p = p->next;
    }
    return INFEASIBLE;
}

//get the next element of the current element
status NextElem(linkedList *L, elemType cur_e, elemType *next_e,
                bool (*cmp)(elemType, elemType))
{
    ListNotExistError(L);
    listNode *p = L->head;
    if (p == NULL)
        return INFEASIBLE;
    while (p->next != NULL)
    {
        if (cmp(p->data, cur_e))
        {
            *next_e = p->next->data;
            return OK;
        }
        else
            p = p->next;
    }
    return INFEASIBLE;
}

//insert element e into the i-th position
status ListInsert(linkedList *L, uint32_t i, elemType e)
{
    ListNotExistError(L);
    listNode *p = L->head, *last = NULL;
    if (i < 1 || i > L->length + 1)
        return INFEASIBLE;
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    if (newNode == NULL)
        exit(OVERFLOW);
    newNode->data = e;
    newNode->next = NULL;

    uint32_t j;
    for (j = 0; j < i - 1; ++j)
    {
        last = p;
        p = p->next;
    }

    if (last == NULL)
        L->head = newNode;
    else
        last->next = newNode;
    newNode->next = p;
    L->length++;
    return OK;
}

//delete the i-th element and return it
status ListDelete(linkedList *L, uint32_t i, elemType *e)
{
    ListNotExistError(L);
    listNode *p = L->head, *last = NULL;
    if (i < 1 || i > L->length)
        return INFEASIBLE;
    uint32_t j;
    for (j = 0; j < i - 1; ++j)
    {
        last = p;
        p = p->next;
    }
    if (last == NULL)
        L->head = p->next;
    else
        last->next = p->next;
    *e = p->data;
    free(p);
    L->length--;
    return OK;
}

//traverse the whole linkedList
status ListTraverse(linkedList *L, status (*visit)(listNode *))
{
    ListNotExistError(L);
    status ret = OK;
    listNode *p = L->head;
    putchar('[');
    while (p != NULL)
    {
        if (p != L->head)
            putchar(',');
        if ((ret = visit(p)) != OK)
            break;
        p = p->next;
    }
    putchar(']');
    return ret;
}

//save the linkedList in file
status SaveInFile(linkedList *L, const char *fileName,
                  status (*visit)(listNode *))
{
    ListNotExistError(L);
    FILE *fp = freopen(fileName, "w", stdout);
    if (fp == NULL)
        exit(1);
    status ret = ListTraverse(L, visit);
    fclose(fp);
    //取消平台依赖性
    freopen(
#ifdef _WIN32
        "CON"
#elif __linux__
        "/dev/tty"
#endif
        ,
        "w", stdout);
    return ret;
}

//parse the linkedList file
status ParseFromFile(linkedList **L, const char *fileName)
{
    status ret;
    if (*L == NULL)
    {
        *L = (linkedList *)malloc(sizeof(linkedList));
        InitList(L);
    }
    else if ((*L)->length != 0)
        ClearList(*L);
    FILE *fp = freopen(fileName, "r", stdin);
    if (fp == NULL)
        exit(1);
    elemType e;
    getchar();
    while ((scanf("%d%*c", &e) == 1))
        if ((ret = ListInsert(*L, (*L)->length + 1, e)) != OK)
            break;
    getchar();
    fclose(fp);
    //取消平台依赖性
    freopen(
#ifdef _WIN32
        "CON"
#elif __linux__
        "/dev/tty"
#endif
        ,
        "r", stdin);

    return ret;
}
