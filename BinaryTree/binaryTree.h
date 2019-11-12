#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include <stdint.h>
#include <stdio.h>
#include "stack.h"

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

typedef uint32_t IDType; //from 0 to UINT32_MAX to encode each node or tree sequentially

typedef struct TreeNode treeNode;
struct TreeNode
{
    elemType data;
    IDType id;
    treeNode *lchild, *rchild;
};
typedef struct BinaryTree
{
    treeNode *root;
    IDType cnt;
} btree;

//create a binary tree by definition
status CreateBiTree(btree **T, char *def);

//destroy the current binary tree
status DestroyBiTree(btree **T);

//clear up the current binary tree
status ClearBiTree(btree *T);

//judge whether the current binary tree is empty
status BiTreeEmpty(btree *T);

//get the depth of the binary tree
uint32_t BiTreeDepth(btree *T);

//locate a given element
treeNode *LocateNode(btree *T, IDType e);

//assign a value to a certain node
status Assign(btree *T, IDType e, elemType value);

//get the pointer of the given node's sibling
treeNode *GetSibling(btree *T, IDType e);

//insert a treenode
typedef enum
{
    L,
    R,
} InsertMan;
status InsertNode(btree *T, IDType e, InsertMan LR, treeNode *c);

//delete a certain treenode
status DeleteNode(btree *T, IDType e);

//four kinds of traverses
status PreOrderTraverse(btree *T, status (*visit)(treeNode *));
status INOrderTraverse(btree *T, status (*visit)(treeNode *));
status PostOrderTraverse(btree *T, status (*visit)(treeNode *));
status LevelOrderTraverse(btree *T, status (*visit)(treeNode *));

#endif