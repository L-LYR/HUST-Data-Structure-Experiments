#include "binaryTree.h"
#include "fcnmfy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t N;

#define INFO "BinaryTree %d"
#define NOT_EXIST " doesn't exist!"
#define IS_EMPTY " is empty!"
#define TRAVERSAL(func, visit)                    \
    do                                            \
    {                                             \
        switch (ret = func(list[cur], visit))     \
        {                                         \
        case ERROR:                               \
            fprintf(stdout, INFO NOT_EXIST, cur); \
            break;                                \
        case INFEASIBLE:                          \
            fprintf(stdout, INFO IS_EMPTY, cur);  \
            break;                                \
        case OK:                                  \
            break;                                \
        }                                         \
        break;                                    \
    } while (0);

status visit(treeNode *tn)
{
    if (tn == NULL)
        return INFEASIBLE;
    else
        printf("%u", tn->id);
    return OK;
}

status visit_print(treeNode *tn)
{
    if (tn == NULL)
        return INFEASIBLE;
    else
        printf("%u(%u)", tn->id, tn->data);
    return OK;
}

status ret;
elemType value;
uint32_t size = 1, cur = 0, depth, searchID;
btree **list;
treeNode *p;
char lr;
InsertMan LR;
char fileName[80], def[80];

void switchCmd(void)
{
    int cmdID = 0;
    list = (btree **)malloc(sizeof(btree *) * size);
    list[cur] = malloc(sizeof(btree));
    list[cur]->root = NULL;
    list[cur]->cnt = 0;
    while ((scanf("%d", &cmdID) == 1) && cmdID != 0)
    {
        switch (cmdID)
        {
        case 1:
            fprintf(stdout, "Enter the formated defination of the " INFO ".\n", cur);
            getchar();
            scanf("%[^\n]%*c", def);
            switch (ret = CreateBiTree(&list[cur], def))
            {
            case ERROR:
                fprintf(stdout, "Valid Address! Cannot Initialize!");
                break;
            case OK:
                fprintf(stdout, INFO " Created Successfully!", cur);
                break;
            }
            break;
        case 2:
            switch (ret = DestroyBiTree(&list[cur]))
            {
            case ERROR:
                fprintf(stdout, "Valid Address! Cannot Destroy!");
                break;
            case INFEASIBLE:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case OK:
                fprintf(stdout, INFO " Deleted Successfully!", cur);
                break;
            }
            break;
        case 3:
            switch (ret = ClearBiTree(list[cur]))
            {
            case INFEASIBLE:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case OK:
                fprintf(stdout, INFO " Cleared Successfully!", cur);
                break;
            }
            break;
        case 4:
            switch (ret = BiTreeEmpty(list[cur]))
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case TRUE:
                fprintf(stdout, INFO IS_EMPTY, cur);
                break;
            case FALSE:
                fprintf(stdout, INFO " isn't empty!", cur);
                break;
            }
            break;
        case 5:
            depth = BiTreeDepth(list[cur]);
            switch (depth)
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            default:
                fprintf(stdout, INFO "'s depth is %u.", cur, depth);
                break;
            }
            break;
        case 6:
            fprintf(stdout, "Enter the node id: ");
            while (scanf("%u", &searchID) != 1)
                getchar();
            p = LocateNode(list[cur], searchID);
            if (p == (void *)ERROR)
            {
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            }
            else if (p == NULL)
                fprintf(stdout, "Cannot find %u in " INFO "!", searchID, cur);
            else
                fprintf(stdout, "Find %u in " INFO ". And its value is %u.", searchID, cur, p->data);
            break;
        case 7:
            fprintf(stdout, "Enter the node id: ");
            while (scanf("%u", &searchID) != 1)
                getchar();
            fprintf(stdout, "Enter the node value: ");
            while (scanf("%u", &value) != 1)
                getchar();
            switch ((ret = Assign(list[cur], searchID, value)))
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, INFO IS_EMPTY, cur);
                break;
            case OK:
                fprintf(stdout, "Assign value successfully!");
                break;
            }
            break;
        case 8:
            fprintf(stdout, "Enter the node id: ");
            while (scanf("%u", &searchID) != 1)
                getchar();
            p = GetSibling(list[cur], searchID);
            if (p == (void *)ERROR)
                fprintf(stdout, INFO NOT_EXIST, cur);
            else if (p == (void *)INFEASIBLE)
                fprintf(stdout, INFO " has no %u node or it is the root node!", cur, searchID);
            else if (p != NULL)
                fprintf(stdout, "%u node's sibling is %u node.", searchID, p->id);
            else
                fprintf(stdout, "%u node has no sibling.", searchID);
            break;
        case 9:
            fprintf(stdout, "Enter the ID of the new node:");
            while (scanf("%u", &searchID) != 1)
                getchar();
            if (LocateNode(list[cur], searchID) != NULL)
            {
                fprintf(stdout, INFO " already has node %u.", cur, searchID);
                break;
            }
            p = (treeNode *)malloc(sizeof(treeNode));
            if (p == NULL)
                exit(OVERFLOW);
            p->id = searchID;
            p->lchild = p->rchild = NULL;
            p->data = 0;
            fprintf(stdout, "Enter the ID of node which you want to insert: ");
            while (scanf("%u", &searchID) != 1)
                getchar();
            fprintf(stdout, "Left or Right? (Enter l(L) or r(R)): ");
            while (((lr = getchar()) == EOF) || ((lr != 'l') && (lr != 'r') && (lr != 'L') && (lr != 'R')))
                ;
            LR = (lr == 'L' || lr == 'l') ? L : R;
            switch (ret = InsertNode(list[cur], searchID, LR, p))
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, INFO " has no %u node.",
                        cur, searchID);
                break;
            case OK:
                fprintf(stdout, "Insert Successfully!");
                break;
            }
            break;
        case 10:
            fprintf(stdout, "Enter the ID of node which you want to delete: ");
            while (scanf("%u", &searchID) != 1)
                getchar();
            switch (ret = DeleteNode(list[cur], searchID))
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, INFO " has no %u node.",
                        cur, searchID);
                break;
            case OK:
                fprintf(stdout, "Delete Successfully!");
                break;
            }
            break;
        case 11:
            TRAVERSAL(PreOrderTraverse, visit);
            break;
        case 12:
            TRAVERSAL(InOrderTraverse, visit);
            break;
        case 13:
            TRAVERSAL(PostOrderTraverse, visit);
            break;
        case 14:
            TRAVERSAL(LevelOrderTraverse, visit_print);
            break;
        case 15:
            if ((ret = BiTreeEmpty(list[cur])) == ERROR)
            {
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            }
            else if (ret)
            {
                fprintf(stdout, INFO IS_EMPTY, cur);
                break;
            }
            fprintf(stdout, "Enter the file name (no more than 80 characters, no blanks) :");
            scanf("%s", fileName);
            switch (ret = SaveInFile(list[cur], fileName, visit))
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            default:
                fprintf(stdout, "Save in %s successfully!", fileName);
                break;
            }
            break;
        case 16:
            fprintf(stdout, "Enter the file name (no more than 80 characters, no blanks) :");
            getchar();
            scanf("%[^\n]", fileName);
            getchar();
            switch (ret = ParseFromFile(&list[cur], fileName))
            {
            case ERROR:
                fprintf(stdout, INFO NOT_EXIST, cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, "File Format Error!");
                break;
            case OK:
                fprintf(stdout, "Parse from file %s successfully!", fileName);
                break;
            }
            break;
        case 17:
            fprintf(stdout, "Enter the BinaryTree Number[0~%d]:", size - 1);
            while (scanf("%d", &cur) != 1)
                getchar();
            if (cur < 0 || cur >= size)
            {
                fprintf(stdout, "Out of range!");
                break;
            }
            fprintf(stdout, "Change to BinaryTree %d successfully!", cur);
            break;
        case 18:
            ++size;
            list = (btree **)realloc(list, sizeof(btree *) * size);
            if (list == NULL)
            {
                fprintf(stdout, "AddList failed!");
                exit(OVERFLOW);
            }
            else
            {
                list[size - 1] = (btree *)malloc(sizeof(btree));
                list[size - 1]->cnt = 0;
                list[size - 1]->root = NULL;
                fprintf(stdout, "AddList succeed!");
            }
            break;
        default:
            break;
        }
        printf("\nEnter [0~%d] to choose one of the operations listed above to continue:", N - 1);
    }
    for (int i = 0; i < size; ++i)
        DestroyBiTree(&list[i]);
    free(list);
}

int main(void)
{
    listFunc();
    switchCmd();
    return 0;
}