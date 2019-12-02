#include "graph.h"

static AdjList *InitAdjList(size_t vn, size_t en,
                            idType id[],
                            weightType vw[], weightType ew[],
                            idType bp[], idType ep[],
                            graphKind gk)
{
    AdjList *al = (AdjList *)malloc(sizeof(AdjList));
    al->listLen = vn;
    al->adjlist = NewList(T, id, ew, vn);
    tab *p = al->adjlist;
    for (size_t i = 0; i < vn; ++i)
    {
        p = NewList
    }
}