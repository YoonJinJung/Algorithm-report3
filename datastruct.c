#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "imaker.h"

void graphInit(Graph *gph, int count) {
    gph->count = count;
    gph->siteinfo = (SiteData *)malloc(sizeof(SiteData) * count);
    gph->adj = (GraphEdge *)malloc(sizeof(GraphEdge) * count);
    for (int i = 0; i < count; i++)
        gph->adj[i].next = NULL;
}

int addDirectedEdge(Graph *gph, int src, int dst, int cost) {
    GraphEdge *temp = (GraphEdge *)malloc(sizeof(GraphEdge));
    /*if (!temp) {
        printf("Mem Err");
        return -1;
    }*/
    GraphEdge *head = gph->adj[src].next;
    while (head) {
        if (head->dest == dst)
            return 1;
        else 
            head = head->next;
    }
    temp->cost = cost;
    temp->dest = dst;
    temp->next = gph->adj[src].next;
    gph->adj[src].next = temp;
    return 0;
}

int addUndirectedEdge(Graph *gph, int src, int dst, int cost) {
    return addDirectedEdge(gph, src, dst, cost) || addDirectedEdge(gph, dst, src, cost);
}

void graphPrint(Graph *gph) {
    GraphEdge *head;
    if (!gph) {
        printf("Empty graph");
        return;
    }
    for (int i = 0; i < gph->count; i++)
    {
        head = gph->adj[i].next;
        printf(" 노드 [ %d ](Site-%d, Tourtime = %d):", i, i, gph->siteinfo[i].tourTime);
        while (head)
        {
            printf(" %d(%d) ", head->dest, head->cost);
            head = head->next;
        }
        printf("\n");
    }
}


void siteRandomInit(Graph *sites, int siteN, int transN, int hotelN) {
    graphInit(sites, siteN);
    srand(time(NULL));
    //site들의 index는 Graph의 노드 순서대로 대응된다.
    //site들에 hotel 을 연결
    for (int i = 0; i < siteN; i++)
    {
        sites->siteinfo[i].hotels = hotelInit(hotelN);
        sites->siteinfo[i].hotelroot = sites->siteinfo[i].hotels->root;
    }
    //site들의 tour time을 설정.
    for (int i = 0; i < siteN; i++)
    {
        sites->siteinfo[i].tourTime = rand() % 3 + 1;
    }
    //site들 사이를 edge 로 모두 이어질 때까지 이어준다.
    for (int i = 0; i < transN; i++)
    {
        int src = rand() % siteN;
        int dst = rand() % siteN;
        //cost정하기
        int cost = rand() % 5 + 1; //cost규칙 (이동수단의 비용, 거리, 이동시간)
        //이미 추가한 edge인지 확인
        int dupl = addUndirectedEdge(sites, src, dst, cost);
        if (dupl) 
        {
            //printf("%d Hit!!\n", dupl);
            i--;
        }
    }
}

void DFSrecur(Graph *gph, int index, int *visited) {
    int destination;
    visited[index] = 1;
    GraphEdge *head = gph->adj[index].next;
    while (head) 
    {
        destination = head->dest;
        if (visited[destination] == 0)
            DFSrecur(gph, destination, visited);
        head = head->next;
    }
}

int DFS(Graph *gph, int source, int target) {
    int count = gph->count;
    int *visited = (int *)calloc(count, sizeof(int));
    DFSrecur(gph, source, visited);
    return visited[target];
}

int isConected(Graph *gph) {
    int count = gph->count;
    int *visited = (int *)calloc(count, sizeof(int));
    GraphEdge *head;

    for (int i = 0; i < count; i++)
    {
        head = gph->adj[i].next;
        if (head)
        {
            DFSrecur(gph, i, visited);
            break;
        }
    }

    for (int j = 0; j < count; j++)
    {
        head = gph->adj[j].next;
        if (head)
        {
            if (visited[j] == 0 && head)
                return 0;
        }
    }

    return 1;
}

RBt *RBTInit(void) {
    RBt *t = (RBt *)malloc(sizeof(RBt));
    RbtNode *nil = (RbtNode *)malloc(sizeof(RbtNode));
    nil->color = 0;
    t->nil = nil;
    t->root = nil;

    return t;
}

void LeftRotate(RBt *T, RbtNode *x)
{
    RbtNode *y = x->right;
    x->right = y->left;
    if (y->left != T->nil) 
        y->left->p = x;
    y->p = x->p;

    if (x->p == T->nil) 
        T->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
    return;
}

void RightRotate(RBt *T, RbtNode *x)
{
    RbtNode *y = x->left;
    x->left = y->right;
    if (y->right != T->nil) 
        y->right->p = x;
    y->p = x->p;

    if (x->p == T->nil) 
        T->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->right = x;
    x->p = y;
    return;
}

void RB_INSERT(RBt *T, RbtNode *z)
{
    RbtNode *y = T->nil;
    RbtNode *x = T->root;
    while (x != T->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else 
            x = x->right;
    }
    z->p = y;
    if (y == T->nil) 
        T->root = z;
    else if (z->key < y->key) 
        y->left = z;
    else 
        y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->color = 1;
    RB_INSERT_FIXUP(T, z);
    return;
}

void RB_INSERT_FIXUP(RBt *T, RbtNode *z) {
    RbtNode *y;
    while (z->p->color == 1) {
        if (z->p == z->p->p->left) {
            y = z->p->p->right;
            if (y->color == 1) {
                z->p->color = 0;
                y->color = 0;
                z->p->p->color = 1;
                z = z->p->p;
            }
            else if (z == z->p->right) {
                z = z->p;
                LeftRotate(T, z);
            }
            else {
                z->p->color = 0;
                z->p->p->color = 1;
                RightRotate(T, z->p->p);
            }
        }
        else {
            y = z->p->p->left;
            if (y->color == 1) {
                z->p->color = 0;
                y->color = 0;
                z->p->p->color = 1;
                z = z->p->p;
            }
            else if (z == z->p->left) {
                z = z->p;
                RightRotate(T, z);
            }
            else {
                z->p->color = 0;
                z->p->p->color = 1;
                LeftRotate(T, z->p->p);
            }
        }
    }
    T->root->color = 0;
}

RbtNode *Successor(RBt *T, RbtNode *x)
{
    RbtNode *y = (RbtNode *)malloc(sizeof(RbtNode));
    if (x->right != T->nil)
        return Minimum(T, x->right);
    else {
        y = x->p;
        while (y != T->nil && x == y->right) {
            x = y;
            y = y->p;
        }
        return y;
    }
}

RbtNode *Minimum(RBt *T, RbtNode *x)
{
    while (x->left != T->nil) {
        x = x->left;
    }
    return x;
}

RbtNode *Search(RBt *T, RbtNode *x, int k)
{
    if (x != T->nil) {
        if (x->key == k) return x;
        else if (x->key > k) {
            if (x->left == T->nil) return x;
            else return Search(T, x->left, k);
        }
        else {
            if (x->right == T->nil) return Successor(T, x);
            else return Search(T, x->right, k);
        }
    }
    else {
        return T->nil;
    }
}

void RB_DELETE(RBt *T, RbtNode *z)
{
    RbtNode *x = (RbtNode *)malloc(sizeof(RbtNode));
    RbtNode *y = (RbtNode *)malloc(sizeof(RbtNode));
    if (z->left == T->nil || z->right == T->nil) 
        y = z;
    else  
        y = Successor(T, z);
    if (y->left != T->nil) 
        x = y->left;
    else 
        x = y->right;
    x->p = y->p;

    if (y->p == T->nil)
        T->root = x;
    else if (y == y->p->left)
        y->p->left = x;
    else 
        y->p->right = x;
    
    if (y != z)
        z->key = y->key;
        z->idx = y->idx;
    if (y->color == 0)
        RB_DELETE_FIXUP(T, x);
        
    return;
}

void RB_DELETE_FIXUP(RBt *T, RbtNode *x)
{
    while (x != T->root && x->color == 0) 
    {
        if (x == x->p->left)
        {
            RbtNode *w = (RbtNode *)malloc(sizeof(RbtNode));
            w = x->p->right;
            if (w->color == 1)
            {
                w->color = 0;
                x->p->color = 1;
                LeftRotate(T, x->p);
                w = x->p->right;
            }
            if (w->left->color == 0 && w->right->color == 0)
            {
                w->color = 1;
                x = x->p;
            }
            else 
            {
                if (w->right->color == 0)
                {
                    w->left->color = 0;
                    w->color = 1;
                    RightRotate(T,w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = 0;
                w->right->color = 0;
                LeftRotate(T, x->p);
                x = T->root;
            }
        }
        else
        {
            RbtNode *w = (RbtNode *)malloc(sizeof(RbtNode));
            w = x->p->left;
            if (w->color == 1)
            {
                w->color = 0;
                x->p->color = 1;
                RightRotate(T, x->p);
                w = x->p->left;
            }
            if (w->left->color == 0 && w->right->color == 0)
            {
                w->color = 1;
                x = x->p;
            }
            else 
            {
                if (w->left->color == 0)
                {
                    w->right->color = 0;
                    w->color = 1;
                    LeftRotate(T,w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = 0;
                w->left->color = 0;
                RightRotate(T, x->p);
                x = T->root;
            }
        }
    }
    x->color = 0;
    return;
}

Trunk* createTrunk(Trunk* prev, char* str) {
    Trunk* trunk = (Trunk*)malloc(sizeof(Trunk));
    trunk->prev = prev;
    trunk->str = (char*)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(trunk->str, str);
    return trunk;
}

void showTrunks(Trunk* p) {
    if (p == NULL) 
        return;
    showTrunks(p->prev);
    printf("%s", p->str);
}

void printTree(RBt *T, RbtNode* root, Trunk* prev, int isLeft) {
    if (root == T->nil) 
        return;

    char* prev_str = "    ";
    Trunk* trunk = createTrunk(prev, prev_str);

    printTree(T, root->right, trunk, 1);

    if (prev == NULL)
        trunk->str = "———";
    else if (isLeft) 
    {
        trunk->str = ".———";
        prev_str = "   |";
    }
    else 
    {
        trunk->str = "`———";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    
    printf(" H- %d(price:%d)\n", root->idx, root->key);

    if (prev != NULL) 
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(T, root->left, trunk, 0);
}

RBt *hotelInit(int hotelN) {
    //idx는 hotel number로 사용한다.
    //key를 가격으로 사용하고 가격순으로 RBtree를 사용해 정렬 후 사용한다.
    RBt *Hotels = RBTInit();
    int *hotelidx = (int *)malloc(sizeof(int) * hotelN);

    //RBtree에 random price로 호텔노드를 하나씩 넣어준다.
    for (int i = 0; i < hotelN; i++) 
    {
        RbtNode *hotel = (RbtNode *)malloc(sizeof(RbtNode));
        hotel->idx = i;
        hotel->key = (rand() % 10 + 1) * 5; //5만원 부터 50만원까지 5만원 단위의 price 랜덤 생성
        RB_INSERT(Hotels, hotel);
    }
    return Hotels;
}
