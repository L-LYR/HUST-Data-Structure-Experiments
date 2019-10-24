#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqList.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__) //relative path
#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                              \
    do                                                                                                                \
    {                                                                                                                 \
        ++test_count;                                                                                                 \
        if (equality)                                                                                                 \
            ++test_pass;                                                                                              \
        else                                                                                                          \
        {                                                                                                             \
            fprintf(stderr, "%s:%d: expect: " format "actual: " format "\n", __FILENAME__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                             \
        }                                                                                                             \
    } while (0)
#define EXPECT_EQ(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
//for int
bool cmp(elemType a, elemType b)
{
    return a == b;
}

status visit(sqList *L, uint32_t i)
{
    fprintf(stdout, "%d", L->elem[i - 1]);
    return OK;
}
#define DEBUG
#ifndef DEBUG
#define SHOW_DETAIL(L)          \
    do                          \
    {                           \
        ListTraverse(L, visit); \
        putchar('\n');          \
        putchar('\n');          \
    } while (0)

#define SHOW_INFO(L)                       \
    do                                     \
    {                                      \
        fprintf(stdout, "L.length = %d\n"  \
                        "L.size = %d\n\n", \
                L->length, L->size);       \
    } while (0)
#else
#define SHOW_DETAIL(L)
#define SHOW_INFO(L)
#endif

void test(void)
{
    sqList *L;
    elemType e, pre_e, next_e;
    uint32_t len, i;

    //InitList() valid operation test
    EXPECT_EQ(OK, InitList(&L));
    SHOW_DETAIL(L);
    SHOW_INFO(L);
    //InitList() invalid operation test
    EXPECT_EQ(ERROR, InitList(NULL));

    //ListInsert() valid operation test
    EXPECT_EQ(OK, ListInsert(L, 1, 1));
    SHOW_DETAIL(L);
    SHOW_INFO(L);
    EXPECT_EQ(OK, ListInsert(L, 2, 3));
    SHOW_DETAIL(L);
    SHOW_INFO(L);
    EXPECT_EQ(OK, ListInsert(L, 2, 2));
    SHOW_DETAIL(L);
    SHOW_INFO(L);

    //ListLength() test
    len = ListLength(L);

    //ListEmpty() test
    EXPECT_EQ(FALSE, ListEmpty(L));

    //ListInsert() invalid operation test
    EXPECT_EQ(INFEASIBLE, ListInsert(L, len + 10, 0));
    EXPECT_EQ(INFEASIBLE, ListInsert(L, 0, 0));

    //ListDelete() invalid operation test
    EXPECT_EQ(INFEASIBLE, ListDelete(L, 0, &e));
    EXPECT_EQ(INFEASIBLE, ListDelete(L, len + 10, &e));

    //ListDelete() valid operation test
    EXPECT_EQ(OK, ListDelete(L, 2, &e));
    SHOW_DETAIL(L);
    SHOW_INFO(L);
    len = ListLength(L);
    EXPECT_EQ(OK, ListDelete(L, len, &e));
    SHOW_DETAIL(L);
    SHOW_INFO(L);
    EXPECT_EQ(OK, ListDelete(L, 1, &e));
    SHOW_DETAIL(L);
    SHOW_INFO(L);

    //ListEmpty() test
    EXPECT_EQ(TRUE, ListEmpty(L));

    //ListInsert() expansion test
    for (i = 120; i >= 1; --i)
    {
        EXPECT_EQ(OK, ListInsert(L, 1, i));
        if (i == 15 || i == 5 || i == 100)
        {
            SHOW_DETAIL(L);
            SHOW_INFO(L);
        }
    }
    len = ListLength(L);

    // GetElem() & LocateElem() & PriorElem() & NextElem() test
    EXPECT_EQ(INFEASIBLE, GetElem(L, len + 10, &e));
    EXPECT_EQ(INFEASIBLE, GetElem(L, 0, &e));

    EXPECT_EQ(OK, GetElem(L, len / 2, &e));
    EXPECT_EQ(OK, LocateElem(L, e, &i, cmp));
    EXPECT_EQ(OK, PriorElem(L, e, &pre_e, cmp));
    EXPECT_EQ(OK, NextElem(L, e, &next_e, cmp));
#ifndef DEBUG
    fprintf(stdout, "The element %d is %d\n", e, i);
    fprintf(stdout, "The %d-th element is %d\n", len / 2, e);
    fprintf(stdout, "The element %d's pre_e is %d, next_e is %d\n\n", e, pre_e, next_e);
#endif
    EXPECT_EQ(INFEASIBLE, LocateElem(L, 0, &i, cmp));

    EXPECT_EQ(OK, GetElem(L, 1, &e));
    EXPECT_EQ(INFEASIBLE, PriorElem(L, e, &pre_e, cmp));
    EXPECT_EQ(OK, GetElem(L, len, &e));
    EXPECT_EQ(INFEASIBLE, NextElem(L, e, &next_e, cmp));

    //ClearList() test
    EXPECT_EQ(OK, ClearList(L));
    SHOW_INFO(L);
    //DestroyList() test
    EXPECT_EQ(OK, DestroyList(&L));
    EXPECT_EQ(NULL, L);
}

int main(void)
{
    test();
    printf("test %d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}