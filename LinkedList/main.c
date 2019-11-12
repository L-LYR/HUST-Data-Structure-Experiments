#include <stdio.h>
#include "fcnmfy.h"
#include "linkedList.h"
extern uint32_t N;
#define toStr(x) #x
#define feedbackUnit(func, str, arg)                              \
    do                                                            \
    {                                                             \
        status errorCode = func(arg);                             \
        switch (errorCode)                                        \
        {                                                         \
        case OK:                                                  \
            fprintf(stdout, "%s succeeded!\n", toStr(func));      \
            break;                                                \
        case ERROR:                                               \
            fprintf(stdout, "%s failed!\n", toStr(func));         \
            fprintf(stdout, "linkedList does not exist!\n");      \
            break;                                                \
        case INFEASIBLE:                                          \
            fprintf(stdout, "%s cannot be done!\n", toStr(func)); \
            if (str != NULL)                                      \
                fprintf(stdout, str);                             \
            break;                                                \
        }                                                         \
    } while (0)

#define ERROR_CASE                                       \
    case ERROR:                                          \
        fprintf(stdout, "linkedList does not exist!\n"); \
        break
#define INFEASIBLE_NOT_EXiST_CASE                           \
    case INFEASIBLE:                                        \
        fprintf(stdout, "Element %d does not exist!\n", e); \
        break
#define INFEASIBLE_OUT_OF_RANGE_CASE                     \
    case INFEASIBLE:                                     \
        fprintf(stdout, "Operation is out of range!\n"); \
        break

bool cmp(elemType a, elemType b)
{
    return a == b;
}

status visit(listNode *L)
{
    fprintf(stdout, "%d", L->data);
    return OK;
}

void interact(void)
{
    int choose, ret;
    uint32_t index, numL = 0, MAXN = 1;
    linkedList **L = (linkedList **)malloc(sizeof(linkedList *) * MAXN);
    memset(L, 0, sizeof(linkedList *) * MAXN);
    elemType e, pre_e, next_e;
    char fileName[80];
    while ((scanf("%d", &choose) == 1) && choose != 0)
    {
        switch (choose)
        {
        case 1:
            feedbackUnit(InitList, NULL, &L[numL]);
            break;
        case 2:
            feedbackUnit(DestroyList, NULL, &L[numL]);
            break;
        case 3:
            feedbackUnit(ClearList, NULL, L[numL]);
            break;
        case 4:
            switch (ListEmpty(L[numL]))
            {
                ERROR_CASE;
            case TRUE:
                fprintf(stdout, "linkedList is empty!\n");
                break;
            case FALSE:
                fprintf(stdout, "linkedList is not empty!\n");
                break;
            }
            break;
        case 5:
            ret = ListLength(L[numL]);
            if (ret == ERROR)
                fprintf(stdout, "linkedList does not exist!\n");
            else
                fprintf(stdout, "linkedList length is %d.\n", ret);
            break;
        case 6:
            fprintf(stdout, "Enter the index of the element you want to get:");
            while (scanf("%d", &index) != 1)
                getchar();
            switch (ret = GetElem(L[numL], index, &e))
            {
                ERROR_CASE;
                INFEASIBLE_OUT_OF_RANGE_CASE;
            case OK:
                fprintf(stdout, "The %d-th element is %d.\n", index, e);
                break;
            }
            break;
        case 7:
            fprintf(stdout, "Enter an element you want to locate:");
            while (scanf("%d", &e) != 1)
                getchar();
            switch (ret = LocateElem(L[numL], e, &index, cmp))
            {
                ERROR_CASE;
                INFEASIBLE_NOT_EXiST_CASE;
            case OK:
                fprintf(stdout, "The element %d is located at %d.\n", e, index);
                break;
            }
            break;
        case 8:
            fprintf(stdout, "Enter an element and locate its prior element:");
            while (scanf("%d", &e) != 1)
                getchar();
            switch (ret = PriorElem(L[numL], e, &pre_e, cmp))
            {
                ERROR_CASE;
            case INFEASIBLE:
                fprintf(stdout, "Element %d's prior element does not exist!\n", e);
                break;
            case OK:
                fprintf(stdout, "The element %d's prior element is %d.\n", e, pre_e);
                break;
            }
            break;
        case 9:
            fprintf(stdout, "Enter an element and locate its next element:");
            while (scanf("%d", &e) != 1)
                getchar();
            switch (ret = NextElem(L[numL], e, &next_e, cmp))
            {
                ERROR_CASE;
            case INFEASIBLE:
                fprintf(stdout, "Element %d's next element does not exist!\n", e);
                break;
            case OK:
                fprintf(stdout, "The element %d's next element is %d.\n", e, next_e);
                break;
            }
            break;
        case 10:
            fprintf(stdout, "Enter an interger:");
            while (scanf("%d", &e) != 1)
                getchar();
            fprintf(stdout, "Enter the location where it will be inserted  (between[1,%d]):", L[numL]->length + 1);
            while (scanf("%d", &index) != 1)
                getchar();
            switch (ret = ListInsert(L[numL], index, e))
            {
                ERROR_CASE;
                INFEASIBLE_OUT_OF_RANGE_CASE;
            case OK:
                fprintf(stdout, "Inserted successfully!\n");
                break;
            }
            break;
        case 11:
            fprintf(stdout, "Enter the location where you want to delete (between[1,%d]):", L[numL]->length);
            while (scanf("%d", &index) != 1)
                getchar();
            switch (ret = ListDelete(L[numL], index, &e))
            {
                ERROR_CASE;
                INFEASIBLE_OUT_OF_RANGE_CASE;
            case OK:
                fprintf(stdout, "Deleted successfully!\nThe deleted element is %d\n", e);
                break;
            }
            break;
        case 12:
            if ((ret = ListEmpty(L[numL])) == ERROR)
            {
                fprintf(stdout, "linkedList does not exist!\n");
                break;
            }
            else if (ret)
            {
                fprintf(stdout, "linkedList is empty!\n");
                break;
            }
            fprintf(stdout, "The whole list is shown below:\n");
            switch (ret = ListTraverse(L[numL], visit))
            {
                ERROR_CASE;
            default:
                putchar('\n');
                break;
            }
            break;
        case 13:
            if ((ret = ListEmpty(L[numL])) == ERROR)
            {
                fprintf(stdout, "sqList does not exist!\n");
                break;
            }
            else if (ret)
            {
                fprintf(stdout, "linkedList is empty!\n");
                break;
            }
            fprintf(stdout, "Enter the file name (no more than 80 characters, no blanks) :");
            scanf("%s", fileName);
            switch (ret = SaveInFile(L[numL], fileName, visit))
            {
            case ERROR:
                fprintf(stdout, "linkedList does not exist!\n");
                break;
            default:
                fprintf(stdout, "Save in %s successfully!\n", fileName);
                break;
            }
            break;
        case 14:
            fprintf(stdout, "Enter the file name (no more than 80 characters, no blanks) :");
            scanf("%s", fileName);
            switch (ret = ParseFromFile(&L[numL], fileName))
            {
            case ERROR:
                fprintf(stdout, "linkedList does not exist!\n");
                break;
            default:
                if (ListEmpty(L[numL]))
                    fprintf(stdout, "File Format Error!\n");
                else
                    fprintf(stdout, "Parse from file %s successfully!\n", fileName);
                break;
            }
            break;
        case 15:
            fprintf(stdout, "Enter the List Number[0~%d]:", MAXN - 1);
            while (scanf("%d", &numL) != 1)
                getchar();
            if (numL < 0 || numL >= MAXN)
                fprintf(stdout, "Error input!\n");
            fprintf(stdout, "Change to List %d successfully!\n", numL);
            break;
        case 16:
            ++MAXN;
            L = (linkedList **)realloc(L, sizeof(linkedList *) * MAXN);
            L[MAXN - 1] = (linkedList *)malloc(sizeof(linkedList));
            if (L == NULL)
            {
                fprintf(stdout, "AddList failed!\n");
                exit(OVERFLOW);
            }
            else
                fprintf(stdout, "AddList succeed!\n");
            feedbackUnit(InitList, NULL, &L[MAXN - 1]);
            break;
        default:
            break;
        }
        printf("\nEnter [1~%d] to choose one of the operations listed above to continue:", N - 1);
    }
    int i;
    for (i = 0; i < MAXN; ++i)
        if (L[i] != NULL)
        {
            free(L[i]);
            L[i] = NULL;
        }
}

int main(void)
{
    listFunc();
    interact();
    return 0;
}