#include "binaryTree.h"
#include "stack.h"
#include "queue.h"

#include <string.h>
#include <stdlib.h>

#define DATA_MAX_LENGTH 80
#define NODE_MAX_NUM 80

#define BEGINMARK '['
#define SPLITMARK ','
#define ENDMARK ']'
#define EMPTYMARK ' '

static char *CreateNode(treeNode **tn, char *def)
{
    //ending conditions
    if (def == NULL || *def == '\0')
        return NULL;
    if (*def == BEGINMARK)
        ++def;
    char *split = strchr(def, SPLITMARK);
    if (split == NULL)
        split = strchr(def, ENDMARK);
    if (*def == EMPTYMARK)
    {
        tn = NULL;
        def = split + 1;
    }
    else
    {
        char temp[DATA_MAX_LENGTH], *p = def, *q = temp;
        while (p != split)
            *(q++) = *(p++);
        *(q) = '\0'; //validation!!
        if (NULL == ((*tn) = (treeNode *)malloc(sizeof(treeNode))))
            exit(OVERFLOW);
        (*tn)->id = atoi(temp);
        (*tn)->lchild = (*tn)->rchild = NULL;
        (*tn)->data = 0;

        def = split + 1;
        def = CreateNode(&((*tn)->lchild), def);
        def = CreateNode(&((*tn)->rchild), def);
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
    }
    if ((*T)->root != NULL)
        ret = ClearBiTree(*T);
    if (*def == '[' && *(def + 1) == ']')
        return ret;
    //create binary tree recurssively
    if (ret == OK)
        CreateNode(&((*T)->root), def);
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
    if (*T == NULL)
        return INFEASIBLE;
    else
    {
        ClearBiTree(*T);
        free(*T);
        *T = NULL;
        return OK;
    }
}

status ClearBiTree(btree *T)
{
    if (T == NULL)
        return INFEASIBLE;
    FreeNode(T->root);
    T->root = NULL;
    return OK;
}

status BiTreeEmpty(btree *T)
{
    if (T == NULL)
        return ERROR;
    return (status)(T->root == NULL);
}

static uint32_t GetDepth(treeNode *root)
{
    if (root == NULL)
        return 0;
    uint32_t LD = GetDepth(root->lchild);
    uint32_t RD = GetDepth(root->rchild);
    return ((LD > RD) ? LD : RD) + 1;
}

uint32_t BiTreeDepth(btree *T)
{
    if (T == NULL)
        return ERROR;
    return GetDepth(T->root);
}

static treeNode *FindNode(treeNode *root, IDType e)
{
    if (root == NULL)
        return NULL;
    if (root->id == e)
        return root;
    treeNode *p = FindNode(root->lchild, e);
    if (p == NULL)
        p = FindNode(root->rchild, e);
    return p;
}

treeNode *LocateNode(btree *T, IDType e)
{
    if (T == NULL)
        return (void *)ERROR;
    return FindNode(T->root, e);
}

status Assign(btree *T, IDType e, elemType value)
{
    if (T == NULL)
        return ERROR;
    treeNode *tn = LocateNode(T, e);
    if (tn == NULL)
        return INFEASIBLE;
    tn->data = value;
    return OK;
}

static treeNode *GetParent(treeNode *parent, treeNode *current)
{
    if (parent == NULL)
        return NULL;
    if (parent->lchild == current)
        return parent;
    if (parent->rchild == current)
        return parent;
    treeNode *p = GetParent(parent->lchild, current);
    if (p == NULL)
        p = GetParent(parent->rchild, current);
    return p;
}

treeNode *GetSibling(btree *T, IDType e)
{
    if (T == NULL)
        return (void *)ERROR;
    treeNode *p = FindNode(T->root, e);
    if (p == NULL)
        return (void *)INFEASIBLE;
    treeNode *q = GetParent(T->root, p);
    if (q == NULL)
        return (void *)INFEASIBLE;
    if (q->lchild == p)
        return q->rchild;
    else
        return q->lchild;
}

status InsertNode(btree *T, IDType e, InsertMan LR, treeNode *c)
{
    if (T == NULL)
        return ERROR;
    treeNode *inp = LocateNode(T, e);
    if (inp == NULL)
        return INFEASIBLE;
    if (LR == L)
    {
        c->rchild = inp->lchild;
        inp->lchild = c;
    }
    else
    {
        c->rchild = inp->rchild;
        inp->rchild = c;
    }
    ++(T->cnt);
    return OK;
}

status DeleteNode(btree *T, IDType e)
{
    if (T == NULL)
        return ERROR;
    treeNode *del = FindNode(T->root, e), *loc;
    if (del == NULL)
        return INFEASIBLE;
    treeNode *p = GetParent(T->root, del);
    treeNode **m;
    if (p == NULL)
        m = &(T->root);
    else if (p->lchild == del)
        m = &(p->lchild);
    else
        m = &(p->rchild);

    if (del->lchild == NULL && del->rchild == NULL)
        *m = NULL;
    else if (del->lchild != NULL && del->rchild != NULL)
    {
        loc = del->lchild;
        while (loc->rchild)
            loc = loc->rchild;
        loc->rchild = del->rchild;
        *m = del->lchild;
    }
    else if (del->lchild != NULL)
        *m = del->lchild;
    else
        *m = del->rchild;
    free(del);
    --(T->cnt);
    return OK;
}

status PreOrderTraverse(btree *T, status (*visit)(treeNode *))
{
    if (T == NULL)
        return ERROR;
    if (BiTreeEmpty(T))
        return INFEASIBLE;
    status ret = OK;
    stack *tns;
    uint32_t cnt = 0;
    INIT_STACK(tns);
    treeNode *p = T->root;
    putchar('[');
    while (p != NULL || tns->top != 0)
    {
        while (p != NULL)
        {
            if (cnt == 0)
                cnt++;
            else
            {
                putchar(',');
                cnt++;
            }
            if ((ret = visit(p)) != OK)
            {
                DESTROY_STACK(tns);
                return ret;
            }
            PUSH(tns, p);
            p = p->lchild;
        }
        putchar(',');
        putchar(' ');
        if (tns->top != 0)
        {
            p = POP(tns);
            p = p->rchild;
        }
    }
    if (cnt != 0)
    {
        putchar(',');
        putchar(' ');
    }
    putchar(']');
    T->cnt = cnt;
    DESTROY_STACK(tns);
    return ret;
}

status InOrderTraverse(btree *T, status (*visit)(treeNode *))
{
    if (T == NULL)
        return ERROR;
    if (BiTreeEmpty(T))
        return INFEASIBLE;
    status ret = OK;
    uint32_t cnt = 0;
    stack *tns;
    INIT_STACK(tns);
    treeNode *p = T->root;
    putchar('[');
    while (p != NULL || tns->top != 0)
    {
        while (p != NULL)
        {
            PUSH(tns, p);
            p = p->lchild;
        }
        if (tns->top != 0)
        {
            p = POP(tns);
            if (cnt == 0)
                cnt++;
            else
            {
                putchar(',');
                cnt++;
            }
            if ((ret = visit(p)) != OK)
            {
                DESTROY_STACK(tns);
                return ret;
            }
            p = p->rchild;
        }
    }
    putchar(']');
    T->cnt = cnt;
    DESTROY_STACK(tns);
    return ret;
}

static status out = FALSE;
static status PostOrderTravel(treeNode *tn, status (*visit)(treeNode *))
{
    status ret = OK;
    if (tn == NULL)
        return ret;
    if ((ret = PostOrderTravel(tn->lchild, visit)) != OK)
        return ret;
    if ((ret = PostOrderTravel(tn->rchild, visit)) != OK)
        return ret;
    if (out)
        putchar(',');
    if ((ret = visit(tn)) != OK)
        return ret;
    else if (!out)
        out = TRUE;
    return ret;
}

status PostOrderTraverse(btree *T, status (*visit)(treeNode *))
{
    if (T == NULL)
        return ERROR;
    if (BiTreeEmpty(T))
        return INFEASIBLE;
    out = FALSE;
    putchar('[');
    status ret = PostOrderTravel(T->root, visit);
    putchar(']');
    return ret;
}

status LevelOrderTraverse(btree *T, status (*visit)(treeNode *))
{
    if (T == NULL)
        return ERROR;
    if (BiTreeEmpty(T))
        return INFEASIBLE;
    status ret = OK;
    uint32_t cnt = 0;
    queue *tnq;
    INIT_QUEUE(tnq);
    enqueue(tnq, T->root);
    putchar('[');
    while (tnq->size)
    {
        size_t cursz = tnq->size;
        for (size_t i = 0; i < cursz; ++i)
        {
            treeNode *curtn = dequeue(tnq);
            if (cnt == 0)
                cnt++;
            else
            {
                putchar(',');
                cnt++;
            }
            if ((ret = visit(curtn)) != OK)
            {
                DESTROY_QUEUE(tnq);
                return ret;
            }
            if (curtn->lchild != NULL)
                enqueue(tnq, curtn->lchild);
            if (curtn->rchild != NULL)
                enqueue(tnq, curtn->rchild);
        }
    }
    putchar(']');
    T->cnt = cnt;
    return ret;
}

status SaveInFile(btree *T, const char *fileName,
                  status (*visit)(treeNode *))
{
    if (T == NULL)
        return ERROR;
    FILE *fp = freopen(fileName, "w", stdout);
    if (fp == NULL)
        exit(1);
    status ret = PreOrderTraverse(T, visit);
    fclose(fp);
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

status ParseFromFile(btree **T, const char *fileName)
{
    status ret = OK;
    FILE *fp = freopen(fileName, "r", stdin);
    if (fp == NULL)
    {
        fprintf(stdout, "Cannot open the file or wrong address!\n");
        ret = INFEASIBLE;
    }
    else
    {
        char def[DATA_MAX_LENGTH * NODE_MAX_NUM];
        scanf("%[^\n]", def);
        getchar();
        printf("%s\n", def);
        if (T == NULL)
        {
            *T = (btree *)malloc(sizeof(btree));
            (*T)->root = NULL;
            (*T)->cnt = 0;
        }
        else if (BiTreeEmpty(*T))
            ClearBiTree(*T);
        ret = CreateBiTree(T, def);
        fclose(fp);
    }
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
