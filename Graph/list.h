#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef int32_t weightType;
typedef uint32_t idType;

typedef struct NodeInfo nodeInfo;
struct NodeInfo
{
    idType ev;    //end vertex id
    weightType w; //weight of edge(bv, ev)
};

typedef struct TabInfo tabInfo;
struct TabInfo
{
    idType bv;      //begin vertex id
    size_t nodeNum; //number of nodes in this list
    weightType w;   //weight of vertex(bv)
    node *el;       //edge list;
};

typedef union Info info;
union Info {
    nodeInfo *ni;
    tabInfo *ti;
};

typedef enum
{
    T,
    N
} nodeType;

typedef struct ListNode listnode;
typedef listnode node;
typedef listnode tab;

struct ListNode
{
    nodeType t; //info-pointer switcher
    info i;     //information of the node
    listnode *next;
};

listnode *NewNode(nodeType t)
{
    listnode *tmp = (listnode *)malloc(sizeof(listnode));
    if (tmp == NULL)
        exit(1);
    memset(tmp, 0, sizeof(tmp));
    tmp->t = t;
    return tmp;
}

listnode *NewList(nodeType t, idType id[], weightType w[], size_t l)
{
    listnode *h = NULL, *tmp = NULL, *last = NULL;

    for (size_t i = 0; i < l; ++i)
    {
        tmp = NewNode(t);

        if (t == T)
        {
            tmp->i.ti->bv = id[i];
            tmp->i.ti->nodeNum = 0;
            tmp->i.ti->w = w[i];
            tmp->i.ti->el = NULL;
        }
        else if (t == N)
        {
            tmp->i.ni->ev = id[i];
            tmp->i.ni->w = w[i];
        }

        if (h == NULL)
            h->next = tmp;
        else
            last->next = tmp;
        last = tmp;
    }
    return h;
}

void DeleteNode(listnode *n, nodeType t)
{
    if (t == N)
    {
        free(n->i.ni);
        free(n);
    }
    else if (t == T)
    {
        DeleteList(n->i.ti->el, N);
        free(n->i.ti);
        free(n);
    }
}

void DeleteList(listnode *l, nodeType t)
{
    listnode *p = l, *last = NULL;
    while (p != NULL)
    {
        last = p;
        p = p->next;
        DeleteNode(last, t);
    }
}

#endif