#ifndef FCNMFY_H_
#define FCNMFY_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
static const uint32_t maxWid = 60, gap = 3;
static const char *funcName[] = {
    "exit",
    "InitList",
    "DestroyList",
    "ClearList",
    "ListEmpty",
    "ListLength",
    "GetElem",
    "LocateElem",
    "PriorElem",
    "NextElem",
    "ListInsert",
    "ListDelete",
    "ListTraverse",
    "SaveInFile",
    "ParseFromFile",
    "ChangeList",
    "AddList",
};
static uint32_t maxLen = 0, N = sizeof(funcName) / sizeof(*funcName);

void listFunc(void)
{
    uint32_t i, j;

    for (i = 0; i < N; ++i)
        if (strlen(funcName[i]) > maxLen)
            maxLen = strlen(funcName[i]);

    uint32_t R, C, row_count;
    C = (maxWid + gap) / (maxLen + gap);
    row_count = R = (N + C - 1) / C;

    for (i = 0; i < maxWid; ++i)
        putchar('-');
    putchar('\n');
    for (i = 0; row_count-- > 0; ++i)
    {
        printf("%*d.", gap, i);
        printf("%-*s", maxLen, funcName[i]);
        for (j = 1; j < C; ++j)
        {
            if (j * R + i >= N)
                break;
            printf("%*d.", gap, i + j * R);
            printf("%-*s", maxLen, funcName[i + j * R]);
        }
        putchar('\n');
    }
    for (i = 0; i < maxWid; ++i)
        putchar('-');
    putchar('\n');
    printf("Enter [0~%d] to choose one of the operations listed above:", N - 1);
}
#endif