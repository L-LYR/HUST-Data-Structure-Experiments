#include "sqList.h"
#include <stdlib.h>
#include <string.h>

#define ListNotExistError(L) \
    do                       \
    {                        \
        if (L == NULL)       \
            return ERROR;    \
    } while (0)

status InitList(sqList **L)
{
    if (L == NULL)
        return ERROR;
    *L = (sqList *)malloc(sizeof(sqList));
    if ((*L) == NULL)
        exit(OVERFLOW);
    (*L)->elem = (elemType *)malloc(sizeof(elemType) * SQLIST_INIT_SIZE);
    if ((*L)->elem == NULL)
        exit(OVERFLOW);
    (*L)->length = 0;
    (*L)->size = SQLIST_INIT_SIZE;
    return OK;
}

status DestroyList(sqList **L)
{
    ListNotExistError(*L);
    free((*L)->elem);
    free(*L);
    (*L) = NULL;
    return OK;
}

status ClearList(sqList *L)
{
    ListNotExistError(L);
    L->length = 0;
    return OK;
}

status ListEmpty(sqList *L)
{
    ListNotExistError(L);
    return (L->length == 0) ? TRUE : FALSE;
}

uint32_t ListLength(sqList *L)
{
    ListNotExistError(L);
    return L->length;
}

status GetElem(sqList *L, uint32_t i, elemType *e)
{
    ListNotExistError(L);
    if (i < 1 || i > L->length)
        return INFEASIBLE;
    *e = L->elem[i - 1];
    return OK;
}

status LocateElem(sqList *L, elemType e, uint32_t *i,
                  bool (*cmp)(elemType, elemType))
{
    ListNotExistError(L);
    int j;
    for (j = 0; j < L->length; ++j)
    {
        if (cmp(e, L->elem[j]))
        {
            *i = j + 1;
            return OK;
        }
    }
    *i = 0;
    return INFEASIBLE;
}

status PriorElem(sqList *L, elemType cur_e, elemType *pre_e,
                 bool (*cmp)(elemType, elemType))
{
    ListNotExistError(L);
    uint32_t i;
    status ret = LocateElem(L, cur_e, &i, cmp);
    if (ret == OK)
    {
        if (i == 1)
            ret = INFEASIBLE;
        else
            *pre_e = L->elem[i - 2];
    }
    return ret;
}

status NextElem(sqList *L, elemType cur_e, elemType *next_e,
                bool (*cmp)(elemType, elemType))
{
    ListNotExistError(L);
    uint32_t i;
    status ret = LocateElem(L, cur_e, &i, cmp);
    if (ret == OK)
    {
        if (i == L->length)
            ret = INFEASIBLE;
        else
            *next_e = L->elem[i];
    }
    return ret;
}

#define ListExpandDefault(L) ListExpand(L, LISTINCREMENT)
static status ListExpand(sqList *L, uint32_t exSize)
{
    ListNotExistError(L);
    L->elem = (elemType *)realloc(L->elem,
                                  sizeof(elemType) * (L->size + exSize));
    if (L->elem == NULL)
        exit(OVERFLOW);
    L->size += exSize;
    return OK;
}

status ListInsert(sqList *L, uint32_t i, elemType e)
{
    ListNotExistError(L);
    if (i < 1 || i > L->length + 1)
        return INFEASIBLE;
    if (L->length == L->size)
        ListExpandDefault(L);
    uint32_t j;
    for (j = L->length; j > i - 1; --j)
        L->elem[j] = L->elem[j - 1];
    L->elem[i - 1] = e;
    L->length++;
    return OK;
}

status ListDelete(sqList *L, uint32_t i, elemType *e)
{
    ListNotExistError(L);
    if (i < 1 || i > L->length)
        return INFEASIBLE;
    uint32_t j = i - 1;
    *e = L->elem[j];
    while (j < L->length - 1)
    {
        L->elem[j] = L->elem[j + 1];
        ++j;
    }
    L->length--;
    return OK;
}

status ListTraverse(sqList *L, status (*visit)(sqList *, uint32_t))
{
    ListNotExistError(L);
    status ret = OK;
    putchar('[');
    for (uint32_t i = 1; i <= L->length; ++i)
    {
        if (i > 1)
            putchar(',');
        if ((ret = visit(L, i)) != OK)
            break;
    }
    putchar(']');
    return ret;
}

status SaveInFile(sqList *L, const char *fileName,
                  status (*visit)(sqList *, uint32_t))
{
    ListNotExistError(L);
    FILE *fp = freopen(fileName, "w", stdout);
    if (fp == NULL)
        exit(1);
    status ret = ListTraverse(L, visit);
    fclose(fp);
    freopen("CON", "w", stdout);
    return ret;
}

status ParseFromFile(sqList **L, const char *fileName)
{
    status ret;
    if (*L == NULL)
    {
        *L = (sqList *)malloc(sizeof(sqList));
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
    freopen("CON", "r", stdin);
    return ret;
}