#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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
typedef uint32_t IDType;

typedef struct TreeNode treeNode;
typedef treeNode *Root;
struct TreeNode
{
    elemType data;
    IDType id;
    treeNode *lchild, *rchild;
};

#endif