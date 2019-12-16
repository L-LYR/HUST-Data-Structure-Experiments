#include "graph.h"
#include "stack.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
/*
format input as below:
[number of vertex,number of edges]
[ids of vertex]
[weight of vertex]
...
[begin point of edge_i, end point of edge_i, weight of edge_i]
...
such as:
[5,8]
[1,2,3,4,5]
[100,200,300,400,500]
[1,2,1]
[1,4,4]
[1,5,5]
[2,3,2]
[2,5,7]
[3,4,3]
[3,5,8]
[4,5,6]

especially for empty graph we have:
[0,0]
[]
[]
*/
static status parse_from_string(char def[][MAX_BUFFER_SIZE], size_t *vn, size_t *en,
                                idType *id, weightType *vw,
                                idType *bp, idType *ep, weightType *ew)
{
    if (def == NULL)
        return ERROR;
    if (2 != sscanf(def[0], "[%u,%u]", vn, en))
        return ERROR;
    else if (*vn > MAX_V_NUM)
        return INFEASIBLE;
    size_t i;
    char *p;

    p = def[1];
    ++p;
    for (i = 0; i < *vn; ++i)
    {
        if (1 != sscanf(p, "%u", &id[i]) || id[i] == 0)
            return ERROR;
        p = strchr(p, ',');
        ++p;
    }

    p = def[2];
    ++p;
    for (i = 0; i < *vn; ++i)
    {
        if (1 != sscanf(p, "%u", &vw[i]))
            return ERROR;
        p = strchr(p, ',');
        ++p;
    }

    for (i = 0; i < *en; ++i)
        if (3 != sscanf(def[3 + i], "[%u,%u,%u]", &bp[i], &ep[i], &ew[i]))
            return ERROR;

    return OK;
}

static status init_tablist(tab **t)
{
    if (t == NULL)
        return ERROR;
    *t = (tab *)malloc(sizeof(tab) * MAX_V_NUM);
    if (*t == NULL)
    {
        fprintf(stdout, "OverFlow!\n");
        exit(OVERFLOW);
    }
    memset(*t, 0, sizeof(tab) * MAX_V_NUM);
    return OK;
}

static size_t find_vex(tab *h, idType v)
{
    size_t i;
    for (i = 0; i < MAX_V_NUM; ++i)
        if (h[i].ti.bv == v)
            return i;
    return MAX_V_NUM;
}

static node *find_arc(tab *h, idType bp, idType ep)
{
    size_t i;
    for (i = 0; i < MAX_V_NUM; ++i)
        if (h[i].ti.bv == bp)
        {
            node *p = h[i].lh;
            while (p != NULL)
            {
                if (p->ni.ev == ep)
                    return p;
                else
                    p = p->next;
            }
            return NULL;
        }
    return NULL;
}

static status insert_arc(tab *t, idType bp, idType ep, weightType ew)
{
    size_t b = find_vex(t, bp);
    if (b == MAX_V_NUM)
        return ERROR;
    node *newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL)
    {
        fprintf(stdout, "OverFlow!\n");
        exit(OVERFLOW);
    }

    newNode->ni.ev = ep;
    newNode->ni.w = ew;

    if (t[b].lh != NULL)
        newNode->next = t[b].lh;
    else
        newNode->next = NULL;
    t[b].lh = newNode;
    t[b].ti.adjNum++;
    return OK;
}

graph *CreateGraph(char def[][MAX_BUFFER_SIZE])
{
    graph *g = (graph *)malloc(sizeof(graph));
    memset(g, 0, sizeof(g));
    if (g == NULL)
    {
        fprintf(stdout, "OverFlow!\n");
        exit(OVERFLOW);
    }

    status ret = OK;
    idType id[MAX_V_NUM];
    weightType vw[MAX_V_NUM], ew[MAX_E_NUM];
    idType bp[MAX_E_NUM], ep[MAX_E_NUM];

    if ((ret = parse_from_string(def, &(g->vn), &(g->en), id, vw, bp, ep, ew)) != OK)
        return (void *)ret;
    if ((ret = init_tablist(&(g->al))) != OK)
        return (void *)ret;

    size_t i, b, e;
    for (i = 0; i < g->vn; ++i)
    {
        g->al[i].ti.bv = id[i];
        g->al[i].ti.w = vw[i];
    }

    for (i = 0; i < g->en; ++i)
    {
        if ((ret = insert_arc(g->al, bp[i], ep[i], ew[i])) != OK)
            return (void *)ret;
        if ((ret = insert_arc(g->al, ep[i], bp[i], ew[i])) != OK)
            return (void *)ret;
    }

    return g;
}

static node *delete_node(node *h, idType t)
{
    node *p = h, *last = NULL;
    while (p != NULL)
    {
        if (p->ni.ev == t)
        {
            if (last != NULL)
                last->next = p->next;
            else
                h = p->next;
            free(p);
            return h;
        }
        else
        {
            last = p;
            p = p->next;
        }
    }
    return h;
}

static void delete_list(node *h)
{
    node *p = h, *last = NULL;
    while (p != NULL)
    {
        last = p;
        p = p->next;
        free(last);
    }
}

static void clear_adjlist(tab *t, size_t vn)
{
    size_t i;
    for (i = 0; i < vn; ++i)
        delete_list(t[i].lh);
}

status DestroyGraph(graph *g)
{
    if (g == NULL)
        return ERROR;
    free(g->al);
    clear_adjlist(g->al, g->vn);
    g->en = g->vn = 0;
    g->al = NULL;
    return OK;
}

size_t LocateVex(graph *g, idType v)
{
    if (g == NULL)
        return ERROR;
    return find_vex(g->al, v);
}

status PutVex(graph *g, idType v, weightType w)
{
    if (g == NULL)
        return ERROR;
    size_t i = find_vex(g->al, v);
    if (i == MAX_V_NUM)
        return INFEASIBLE;
    g->al[i].ti.w = w;
    return OK;
}

node *FirstAdjVex(graph *g, idType v)
{
    if (g == NULL)
        return (void *)ERROR;
    size_t i = find_vex(g->al, v);
    if (i == MAX_V_NUM)
        return (void *)INFEASIBLE;
    return g->al[i].lh;
}

node *NextAdjVex(graph *g, idType v, idType u)
{
    if (g == NULL)
        return (void *)ERROR;
    size_t i = find_vex(g->al, v);
    if (i == MAX_V_NUM)
        return (void *)INFEASIBLE;
    node *p = g->al[i].lh;
    while (p != NULL)
    {
        if (p->ni.ev == u)
            return p->next;
        p = p->next;
    }
    return NULL;
}

status InsertVex(graph *g, idType v)
{
    if (g == NULL || v == 0)
        return ERROR;
    if (find_vex(g->al, v) != MAX_V_NUM)
        return INFEASIBLE;
    else if (g->vn + 1 >= MAX_V_NUM)
        return INFEASIBLE;
    size_t i = find_vex(g->al, 0);
    g->al[i].ti.bv = v;
    g->al[i].lh = NULL;
    g->al[i].ti.w = 0;
    g->al[i].ti.adjNum = 0;
    (g->vn)++;
    return OK;
}

status DeleteVex(graph *g, idType v)
{
    if (g == NULL)
        return ERROR;
    size_t i = 0, j = 0;
    if ((i = find_vex(g->al, v)) == MAX_V_NUM)
        return INFEASIBLE;
    node *p = g->al[i].lh, *last = NULL;
    while (p != NULL)
    {
        last = p;
        p = p->next;
        j = find_vex(g->al, last->ni.ev);
        --(g->al[j].ti.adjNum);
        free(last);
    }
    g->al[i].ti.bv = 0;
    for (int i = 0; i < MAX_V_NUM; ++i)
        if (g->al[i].ti.bv != 0)
            g->al[i].lh = delete_node(g->al[i].lh, v);
    (g->vn)--;                   //update the number of vertice
    g->en -= g->al[i].ti.adjNum; //update the number of edges
    return OK;
}

void display_nodelist(node *h)
{
    node *p = h;
    while (p != NULL)
    {
        fprintf(stdout, "%u(%d)->", p->ni.ev, p->ni.w);
        p = p->next;
    }
}

status InsertArc(graph *g, idType v, idType u)
{
    if (g == NULL)
        return ERROR;
    size_t i, j;
    if ((i = find_vex(g->al, v)) == MAX_V_NUM || (j = find_vex(g->al, u)) == MAX_V_NUM)
        return INFEASIBLE;
    else if (find_arc(g->al, v, u) != NULL)
        return INFEASIBLE;
    status ret = OK;
    if ((ret = insert_arc(g->al, v, u, 0)) != OK)
        return ret;
    if ((ret = insert_arc(g->al, u, v, 0)) != OK)
        return ret;
    ++(g->al[i].ti.adjNum);
    ++(g->al[j].ti.adjNum);
    ++(g->en);
    return ret; //update
}

status DeleteArc(graph *g, idType v, idType u)
{
    if (g == NULL)
        return ERROR;
    size_t i, j;
    if ((i = find_vex(g->al, v)) == MAX_V_NUM || (j = find_vex(g->al, u)) == MAX_V_NUM)
        return INFEASIBLE;
    else if (find_arc(g->al, v, u) == NULL)
        return INFEASIBLE;
    g->al[i].lh = delete_node(g->al[i].lh, u);
    g->al[j].lh = delete_node(g->al[j].lh, v);
    --(g->al[i].ti.adjNum);
    --(g->al[j].ti.adjNum);
    --(g->en);
    return OK;
}

static void BFS(tab *t, size_t b, int vis[])
{
    queue *q;
    node *p;
    size_t cur, next, cnt = 0;
    putchar('[');
    INIT_QUEUE(q);
    enqueue(q, b);
    vis[b] = 1;
    while (q->size != 0)
    {
        cur = dequeue(q);
        if (cnt++ != 0)
            putchar(',');
        // fprintf(stdout, "%u(%d)", t[cur].ti.bv, t[cur].ti.w);
        fprintf(stdout, "%u", t[cur].ti.bv);
        p = t[cur].lh;
        while (p != NULL)
        {
            next = find_vex(t, p->ni.ev);
            if (!vis[next])
            {
                enqueue(q, next);
                vis[next] = 1;
            }
            p = p->next;
        }
    }
    DESTROY_QUEUE(q);
    putchar(']');
    putchar('\n');
    return;
}

static void DFS(tab *t, size_t b, int vis[])
{
    stack *s;
    node *p;
    size_t cur, next, cnt = 0;
    putchar('[');
    INIT_STACK(s);
    PUSH(s, b);
    vis[b] = 1;
    while (s->top != 0)
    {
        cur = POP(s);
        if (cnt++ != 0)
            putchar(',');
        // fprintf(stdout, "%u(%d)", t[cur].ti.bv, t[cur].ti.w);
        fprintf(stdout, "%u", t[cur].ti.bv);
        p = t[cur].lh;
        while (p != NULL)
        {
            next = find_vex(t, p->ni.ev);
            if (!vis[next])
            {
                PUSH(s, next);
                vis[next] = 1;
            }
            p = p->next;
        }
    }
    DESTROY_STACK(s);
    putchar(']');
    putchar('\n');
    return;
}

status BFSTraverse(graph *g)
{
    if (g == NULL)
        return ERROR;
    int vis[MAX_V_NUM];
    memset(vis, 0, sizeof(vis));
    size_t i;
    for (i = 0; i < MAX_V_NUM; ++i)
    {
        if (!vis[i] && g->al[i].ti.bv != 0)
            BFS(g->al, i, vis);
    }
    return OK;
}

status DFSTraverse(graph *g)
{
    if (g == NULL)
        return ERROR;
    int vis[MAX_V_NUM];
    memset(vis, 0, sizeof(vis));
    size_t i;
    for (i = 0; i < MAX_V_NUM; ++i)
    {
        if (!vis[i] && g->al[i].ti.bv != 0)
            DFS(g->al, i, vis);
    }
    return OK;
}

void display_adjlist(tab *t, size_t vn)
{
    if (vn == 0)
    {
        fprintf(stdout, "<EMPTY>\n\n");
        return;
    }
    size_t i, j;
    putchar('\n');
    for (j = i = 0; j < vn; ++i)
    {
        if (t[i].ti.bv != 0)
        {
            fprintf(stdout, "%u(%d)->", t[i].ti.bv, t[i].ti.w);
            display_nodelist(t[i].lh);
            fprintf(stdout, "\n");
            ++j;
        }
    }
    putchar('\n');
    putchar('\n');
}

status SaveInFile(graph *g, char *filename)
{
    if (g == NULL)
        return ERROR;
    FILE *fp = freopen(filename, "w", stdout);
    if (fp == NULL)
    {
        fprintf(stdout, "Cannot open the file or wrong address!\n");
        return INFEASIBLE;
    }
    fprintf(stdout, "[%u,%u]\n", g->vn, g->en);
    putchar('[');
    size_t i, cnt = 0;
    tab *t = g->al;
    for (i = 0; i < MAX_V_NUM; ++i)
    {
        if (t[i].ti.bv != 0)
        {
            if (cnt++ != 0)
                putchar(',');
            fprintf(stdout, "%u", t[i].ti.bv);
        }
    }
    putchar(']');
    putchar('\n');

    putchar('[');
    cnt = 0;
    for (i = 0; i < MAX_V_NUM; ++i)
    {
        if (t[i].ti.bv != 0)
        {
            if (cnt++ != 0)
                putchar(',');
            fprintf(stdout, "%d", t[i].ti.w);
        }
    }
    putchar(']');
    putchar('\n');

    size_t j = 0;
    idType bp[MAX_E_NUM], ep[MAX_E_NUM];
    memset(bp, 0, sizeof(bp));
    memset(ep, 0, sizeof(ep));
    int flag;
    node *p;

    cnt = 0;
    for (i = 0; i < MAX_V_NUM; ++i)
    {
        if (t[i].ti.bv != 0)
        {
            p = t[i].lh;
            while (p != NULL)
            {
                flag = 0; //judge whether it is the reverse edge
                for (j = 0; j < cnt; ++j)
                    if (ep[j] == t[i].ti.bv && bp[j] == p->ni.ev)
                    {
                        flag = 1;
                        break;
                    }

                if (flag == 0)
                {
                    fprintf(stdout, "[%u,%u,%d]\n", t[i].ti.bv, p->ni.ev, p->ni.w);
                    bp[cnt] = t[i].ti.bv;
                    ep[cnt] = p->ni.ev;
                    ++cnt;
                }
                p = p->next;
            }
        }
    }
    fclose(fp);
    freopen(
#ifdef _WIN32
        "CON"
#elif __linux__
        "/dev/tty"
#endif
        ,
        "w", stdout);
    return OK;
}

void inputDef(char def[][MAX_BUFFER_SIZE])
{
    size_t vn = 0, en = 0;
    fscanf(stdin, "%[^\n]", def[0]);
    getchar();
    sscanf(def[0], "[%u,%u]", &vn, &en);

    size_t i;
    for (i = 1; i <= en + 2; ++i)
    {
        fscanf(stdin, "%[^\n]", def[i]);
        getchar();
    }
}

status ParseFromFile(graph **g, char *filename)
{
    if (g == NULL)
        return ERROR;
    char def[MAX_E_NUM][MAX_BUFFER_SIZE];
    memset(def, 0, sizeof(def));
    FILE *fp = freopen(filename, "r", stdin);
    if (fp == NULL)
    {
        fclose(fp);
        fprintf(stdout, "Cannot open the file or wrong address!\n");
        return INFEASIBLE;
    }

    inputDef(def);
    *g = CreateGraph(def);

    fclose(fp);
    freopen(
#ifdef _WIN32
        "CON"
#elif __linux__
        "/dev/tty"
#endif
        ,
        "r", stdin);
    return OK;
}