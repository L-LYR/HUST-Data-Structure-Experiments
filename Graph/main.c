#include "graph.h"
#include "fcnmfy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

graph **g;
size_t size = 1, cur = 0;
char def[MAX_E_NUM][MAX_BUFFER_SIZE];
char filename[MAX_BUFFER_SIZE];
size_t cmd = 0, vexAddrBp = 0, vexAddrEp = 0;
weightType Weight;
status ret;
node *adjVex = NULL;
int flag;

#define Info "Graph %u"

void inputDef(char def[][MAX_BUFFER_SIZE]);
void display_adjlist(tab *t, size_t vn);
void display_nodelist(node *h);

void interact(void)
{
    g = (graph **)malloc(sizeof(graph *) * size);
    memset(g, 0, sizeof(graph *) * size);
    listFunc();
    while (scanf("%d", &cmd) == 1 && cmd != 0)
    {
        getchar();
        switch (cmd)
        {
        case 1:
            if (flag == 0)
            {
                fprintf(stdout, "\nFormat input is shown as below:\n"
                                "[number of vertex,number of edges]\n"
                                "[ids of vertex]\n"
                                "[weight of vertex]\n"
                                "...\n"
                                "[begin point of edge_i, end point of edge_i, weight of edge_i]\n"
                                "...\n\n"
                                "For example, "
                                "a graph which has five vertex and eight edges is defined as below:\n"
                                "[5,8]\n"
                                "[1,2,3,4,5]\n"
                                "[100,200,300,400,500]\n"
                                "[1,2,1]\n"
                                "[1,4,4]\n"
                                "[1,5,5]\n"
                                "[2,3,2]\n"
                                "[2,5,7]\n"
                                "[3,4,3]\n"
                                "[3,5,8]\n"
                                "[4,5,6]\n"
                                "\nEspecially for empty graph we have:\n"
                                "[0,0]\n"
                                "[]\n"
                                "[]\n\n");
                flag = 1;
            }
            fprintf(stdout, "Please enter the format definition of graph:\n");
            memset(def, 0, sizeof(def));
            inputDef(def);
            g[cur] = CreateGraph(def);
            ret = (status)g[cur];
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, "Invalid input!\n");
                break;
            case INFEASIBLE:
                fprintf(stdout, "Too many vertice!\n");
                break;
            default:
                fprintf(stdout, Info " is created successfully!\nIt's displayed in physical structure as below:\n\n", cur);
                display_adjlist(g[cur]->al, g[cur]->vn);
                break;
            }
            break;
        case 2:
            ret = DestroyGraph(g[cur]);
            free(g[cur]);
            g[cur] = NULL;
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, Info " doesn't exist!\n", cur);
                break;
            case OK:
                fprintf(stdout, Info " is destroyed successfully!\n", cur);
                break;
            }
            break;
        case 3:
            fprintf(stdout, "Enter the vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            vexAddrBp = LocateVex(g[cur], vexAddrBp);
            if (vexAddrBp == ERROR)
                fprintf(stdout, Info " doesn't exist!\n", cur);
            else if (vexAddrBp == MAX_V_NUM)
                fprintf(stdout, "Vertex doesn't exist!\n");
            else
            {
                fprintf(stdout, "%u(%d)->",
                        g[cur]->al[vexAddrBp].ti.bv, g[cur]->al[vexAddrBp].ti.w);
                display_nodelist(g[cur]->al[vexAddrBp].lh);
                fprintf(stdout, "\nThere %u vertice adjacent to vertex %u.\n",
                        g[cur]->al[vexAddrBp].ti.adjNum, g[cur]->al[vexAddrBp].ti.bv);
            }
            break;
        case 4:
            fprintf(stdout, "Enter the vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            fprintf(stdout, "Enter the weight:\n");
            while (scanf("%u", &Weight) != 1)
                getchar();
            ret = PutVex(g[cur], vexAddrBp, Weight);
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, Info " doesn't exist!\n", cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, "Vertex doesn't exist!\n");
                break;
            case OK:
                fprintf(stdout, "Assign successfully\nIt's displayed in physical structure as below:\n\n");
                display_adjlist(g[cur]->al, g[cur]->vn);
                break;
            }
            break;
        case 5:
            fprintf(stdout, "Enter the vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            adjVex = FirstAdjVex(g[cur], vexAddrBp);
            if (adjVex == (void *)ERROR)
                fprintf(stdout, Info " doesn't exist!\n", cur);
            else if (adjVex == (void *)INFEASIBLE)
                fprintf(stdout, "Vertex doesn't exist!\n");
            else if (adjVex != NULL)
                fprintf(stdout, "The first adjacent vertex is %u.\n", adjVex->ni.ev);
            else
                fprintf(stdout, "The first adjacent vertex doesn't exist.\n");
            break;
        case 6:
            fprintf(stdout, "Enter the vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            fprintf(stdout, "Enter the adjacent vertex ID:\n");
            while (scanf("%u", &vexAddrEp) != 1)
                getchar();
            adjVex = NextAdjVex(g[cur], vexAddrBp, vexAddrEp);
            if (adjVex == (void *)ERROR)
                fprintf(stdout, Info " doesn't exist!\n", cur);
            else if (adjVex == (void *)INFEASIBLE)
                fprintf(stdout, "Vertex doesn't exist!\n");
            else if (adjVex == NULL)
                fprintf(stdout, "The next adjacent vertex doesn't exist!\n");
            else
                fprintf(stdout, "The next adjacent vertex is %u.\n", adjVex->ni.ev);
            break;
        case 7:
            fprintf(stdout, "Enter the vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            ret = InsertVex(g[cur], vexAddrBp);
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, Info " doesn't exist or invalid ID number!\n", cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, "Vertex exists or beyond the limit vertice number!\n");
                break;
            case OK:
                fprintf(stdout, "Insert successfully\nIt's displayed in physical structure as below:\n\n");
                display_adjlist(g[cur]->al, g[cur]->vn);
                break;
            }
            break;
        case 8:
            fprintf(stdout, "Enter the vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            ret = DeleteVex(g[cur], vexAddrBp);
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, Info " doesn't exist!\n", cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, "Vertex doesn't exist!\n");
                break;
            case OK:
                fprintf(stdout, "Delete successfully\nIt's displayed in physical structure as below:\n\n");
                display_adjlist(g[cur]->al, g[cur]->vn);
                break;
            }
            break;
        case 9:
            fprintf(stdout, "Enter the begin-vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            fprintf(stdout, "Enter the end-vertex ID:\n");
            while (scanf("%u", &vexAddrEp) != 1)
                getchar();
            ret = InsertArc(g[cur], vexAddrBp, vexAddrEp);
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, Info " doesn't exist!\n", cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, "Vertice don't exist or edge already exists!\n");
                break;
            case OK:
                fprintf(stdout, "Insert successfully\nIt's displayed in physical structure as below:\n\n");
                display_adjlist(g[cur]->al, g[cur]->vn);
                break;
            }
            break;
        case 10:
            fprintf(stdout, "Enter the begin-vertex ID:\n");
            while (scanf("%u", &vexAddrBp) != 1)
                getchar();
            fprintf(stdout, "Enter the end-vertex ID:\n");
            while (scanf("%u", &vexAddrEp) != 1)
                getchar();
            ret = DeleteArc(g[cur], vexAddrBp, vexAddrEp);
            switch (ret)
            {
            case ERROR:
                fprintf(stdout, Info " doesn't exist!\n", cur);
                break;
            case INFEASIBLE:
                fprintf(stdout, "Vertice don't exist or edge doesn't exist!\n");
                break;
            case OK:
                fprintf(stdout, "Delete successfully\nIt's displayed in physical structure as below:\n\n");
                display_adjlist(g[cur]->al, g[cur]->vn);
                break;
            }
            break;
        case 11:
            if ((ret = BFSTraverse(g[cur])) == ERROR)
                fprintf(stdout, Info " doesn't exist!\n", cur);
            break;
        case 12:
            if ((ret = DFSTraverse(g[cur])) == ERROR)
                fprintf(stdout, Info " doesn't exist!\n", cur);
            break;
        case 14:
            fprintf(stdout, "Enter the Graph Number[0~%d]:", size - 1);
            while (scanf("%d", &cur) != 1)
                getchar();
            if (cur < 0 || cur >= size)
            {
                fprintf(stdout, "Out of range!\n");
                break;
            }
            fprintf(stdout, "Change to Graph %d successfully!\n", cur);
            break;
        case 13:
            ++size;
            g = (graph **)realloc(g, sizeof(graph *) * size);
            if (g == NULL)
            {
                fprintf(stdout, "AddGraph failed!\n");
                exit(OVERFLOW);
            }
            else
            {
                g[size - 1] = NULL;
                fprintf(stdout, "AddGraph succeed!\n");
            }
            break;
        case 15:
            fprintf(stdout, "Enter the valid file path:");
            scanf("%[^\n]", filename);
            getchar();
            if ((ret = SaveInFile(g[cur], filename)) == ERROR)
                fprintf(stdout, Info " doesn't exist!\n", cur);
            else if (ret == INFEASIBLE)
                fprintf(stdout, "No such file or wrong path!\n");
            else
            {
                fprintf(stdout, "Save in %s successfully!\n", filename);
                display_adjlist(g[cur]->al, g[cur]->vn);
            }
            break;
        case 16:
            fprintf(stdout, "Enter the valid file path:");
            scanf("%[^\n]", filename);
            getchar();
            if ((ret = ParseFromFile(&g[cur], filename)) == ERROR)
                fprintf(stdout, "No space to initialize!\n");
            else if (ret == INFEASIBLE)
                fprintf(stdout, "No such file or wrong path!\n");
            else
            {
                fprintf(stdout, "parse from %s successfully!\n", filename);
                display_adjlist(g[cur]->al, g[cur]->vn);
            }
            break;
        case 17:
            fprintf(stdout, "\nFormat input is shown as below:\n"
                            "[number of vertex,number of edges]\n"
                            "[ids of vertex]\n"
                            "[weight of vertex]\n"
                            "...\n"
                            "[begin point of edge_i, end point of edge_i, weight of edge_i]\n"
                            "...\n\n"
                            "For example, "
                            "a graph which has five vertex and eight edges is defined as below:\n"
                            "[5,8]\n"
                            "[1,2,3,4,5]\n"
                            "[100,200,300,400,500]\n"
                            "[1,2,1]\n"
                            "[1,4,4]\n"
                            "[1,5,5]\n"
                            "[2,3,2]\n"
                            "[2,5,7]\n"
                            "[3,4,3]\n"
                            "[3,5,8]\n"
                            "[4,5,6]\n"
                            "\nEspecially for empty graph we have:\n"
                            "[0,0]\n"
                            "[]\n"
                            "[]\n\n");
            break;
        }
        listFunc();
    }
}

void clearEnv(void)
{
    size_t i = 0;
    for (i = 0; i < size; ++i)
    {
        DestroyGraph(g[i]);
        free(g[i]);
    }
    free(g);
}

int main(void)
{
    interact();
    clearEnv();
    return 0;
}