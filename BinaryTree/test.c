#include "binaryTree.h"
#include "binaryTree.c"
status visit(treeNode *tn)
{
    if (tn == NULL)
        putchar('!');
    else
        printf("%u\n", tn->id);
    return OK;
}
int main(void)
{
    btree *T = NULL;
    char str[] = "[1111,2222,3333, , ,4444,5555, ,6666, , ,7777, , , ]";
    return 0;
}