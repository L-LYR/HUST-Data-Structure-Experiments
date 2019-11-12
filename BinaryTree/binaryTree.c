#include "binaryTree.h"
#include <string.h>
#include <stdlib.h>

#define BEGINMARK '['
#define SPLITMARK ','
#define ENDMARK ']'
#define EMPTYMARK ' '

static char *CreateNode(treeNode **tn, char *def, IDType *id)
{
    //ending conditions
    if (def == NULL || *def == '\0')
        return NULL;

    if (*def == BEGINMARK)
        ++def;
    char *split = strchr(def, SPLITMARK);
    if (split == NULL)
        split = strchr(def, ENDMARK);

    char temp[80], *p = def, *q = temp;
    while (p != split)
        *(q++) = *(p++);

    if (*def == EMPTYMARK)
    {
        tn = NULL;
        def = split + 1;
    }
    else
    {
        if (NULL == ((*tn) = (treeNode *)malloc(sizeof(treeNode))))
            exit(OVERFLOW);
        elemType value = atoi(temp);
        (*tn)->id = *id;
        (*tn)->data = value;
        (*tn)->lchild = (*tn)->rchild = NULL;
        //update
        (*id)++;
        def = split + 1;

        def = CreateNode(&((*tn)->lchild), def, id);
        def = CreateNode(&((*tn)->rchild), def, id);
    }
    return def;
}
status CreateBiTree(btree **T, char *def)
{
    //valid address
    if (T == NULL)
        return ERROR;

    status ret = OK;
    //initialize tree
    if (*T == NULL)
    {
        (*T) = (btree *)malloc(sizeof(btree));
        if ((*T) == NULL)
            exit(OVERFLOW);
        (*T)->root = NULL;
        (*T)->cnt = 1;
    }
    if ((*T)->root != NULL)
        ret = DestroyBiTree(T);
    //create binary tree recurssively
    if (ret == OK)
    {
        CreateNode(&((*T)->root), def, &((*T)->cnt));
    }
    return ret;
}

static void FreeNode(treeNode *tn)
{
    if (tn == NULL)
        return;
    FreeNode(tn->lchild);
    FreeNode(tn->rchild);
    free(tn);
}

status DestroyBiTree(btree **T)
{
    if (T == NULL)
        return ERROR;
    status ret = OK;
    if (*T == NULL)
        ret = INFEASIBLE;
    else
    {
        ret = ClearBiTree(*T);
        free(*T);
        *T = NULL;
    }
    return ret;
}

status ClearBiTree(btree *T)
{
    if (T == NULL)
        return ERROR;
    T->cnt = 0;
    FreeNode(T->root);
    T->root = NULL;
    return OK;
}

status BiTreeEmpty(btree *T)
{
    return T->root == NULL;
}

status PreOrderTraverse(btree *T, status (*visit)(treeNode *))
{
    if (T == NULL)
        return ERROR;
    status ret = BiTreeEmpty(T);
    if (ret == FALSE)
    {
        //initlaize stack
        stack *ts = (stack *)malloc(sizeof(stack));
        ts->s = NULL;
        ts->size = ts->top = 0;
        *((elemType *)push(ts, sizeof(elemType))) = T->root->data;
        while (ts->top != 0)
        {
                }
    }
    return ret;
}