#ifndef GRAPH_H_
#define GRAPH_H_

#include "list.h"

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

typedef enum
{
    DG,
    UDG
} graphKind;

typedef struct AdjList AdjList;
struct AdjList
{
    size_t listLen;
    tab *adjlist;
};

typedef struct Graph
{
    size_t vertexNum, edgeNum;
    AdjList al;
    graphKind gk;
} graph;


#endif