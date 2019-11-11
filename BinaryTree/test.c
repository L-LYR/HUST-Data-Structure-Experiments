#include "binaryTree.h"
#include "binaryTree.c"
int main(void)
{
    btree *T = NULL;
    char str[] = "[1,2,3, , ,4,5, ,6, , ,7, , , ]";
    CreateBiTree(&T, str);
    DestroyBiTree(&T);
    return 0;
}