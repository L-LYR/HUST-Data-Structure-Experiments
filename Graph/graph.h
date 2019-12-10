#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdint.h>
#include <stdlib.h>

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

// support for different kind of graph
// typedef enum GraphKind
// {
//     DG,
//     UDG
// } graphKind;

typedef int32_t weightType;
typedef uint32_t idType;

typedef struct NodeInfo nodeInfo;
struct NodeInfo
{
    idType ev;    //end vertex id
    weightType w; //weight of edge(bv, ev)
};

typedef struct Node node;
struct Node
{
    nodeInfo ni;
    node *next;
};

typedef struct TabInfo tabInfo;
struct TabInfo
{
    idType bv;     //begin vertex id
    size_t adjNum; //number of nodes in this list
    weightType w;  //weight of vertex(bv)
};

typedef struct Tab tab;
struct Tab
{
    tabInfo ti;
    node *lh;
};

// maxinum of vertex number
#define MAX_V_NUM 50
#define MAX_E_NUM (MAX_V_NUM * MAX_V_NUM)
#define INCEREMENT_SIZE 10
#define MAX_BUFFER_SIZE 256
typedef struct Graph
{
    size_t vn, en;
    tab *al;
    // graphKind gk;
} graph;
//Create a graph which is defined by a string-array "def"
graph *CreateGraph(char def[][MAX_BUFFER_SIZE]);
//Destroy the current graph
status DestroyGraph(graph *g);
//Locate the vertex v
size_t LocateVex(graph *g, idType v);
//Assign the vertex v by w
status PutVex(graph *g, idType v, weightType w);
//Find the first adjacent vertex of v
node *FirstAdjVex(graph *g, idType v);
//Find the next adjacent vertex of u in the node list of v
node *NextAdjVex(graph *g, idType v, idType u);

//Insert a vertex
status InsertVex(graph *g, idType v);
//Delete a vertex
status DeleteVex(graph *g, idType v);
//Insert an edge
status InsertArc(graph *g, idType v, idType u);
//Delete an edge
status DeleteArc(graph *g, idType v, idType u);

//BFS
status BFSTraverse(graph *g);
//DFS
status DFSTraverse(graph *g);

//save
status SaveInFile(graph *g, char *filename);
//parse
status ParseFromFile(graph **g, char *filename);
#endif