#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "imaker.h"

#define INF 100000

void graphInit(Graph *gph, int count) {
    gph->count = count;
    gph->siteinfo = (SiteData *)malloc(sizeof(SiteData) * count);
    gph->adj = (GraphNode *)malloc(sizeof(GraphNode) * count);
    for (int i = 0; i < count; i++)
        gph->adj[i].next = NULL;
}

int addDirectedEdge(Graph *gph, int src, int dst, int cost) {
    GraphNode *temp = (GraphNode *)malloc(sizeof(GraphNode));
    GraphNode *head = gph->adj[src].next;
    while (head) {
        if (head->dest == dst || head->vertex == dst)
            return 1;
        else 
            head = head->next;
    }
    temp->vertex = src;
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
    GraphNode *head;
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
        sites->siteinfo[i].tourTime = rand() % 3 + 4;
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
    GraphNode *head = gph->adj[index].next;
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
    GraphNode *head;

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

int compare(int a, int b, int isMinHeap) {
    if (isMinHeap)
    {
        if (a >= b) return 1;
        else return 0;
    }
    else 
    {
        if (a >= b) return 0;
        else return 1;
    }
}

void percolateDown(PqData *a, int position, int size, int isMinHeap) {
    int leftC = 2 * position + 1;
    int rightC = leftC + 1;

    int less = -1;
    PqData temp;

    if (leftC < size)
        less = leftC;
    
    if (rightC < size && compare(a[leftC].key, a[rightC].key, isMinHeap))
        less = rightC;
    
    if (less != -1 && compare(a[position].key, a[less].key, isMinHeap))
    {
        temp = a[position];
        a[position] = a[less];
        a[less] = temp;
        percolateDown(a, less, size, isMinHeap);
    }
}

void percolateUp(PqData *a, int position, int isMinHeap) {
    int parent = (position - 1) / 2;
    PqData temp;

    if (parent >= 0)
    {
        if (compare(a[parent].key, a[position].key, isMinHeap))
        {
            temp = a[position];
            a[position] = a[parent];
            a[parent] = temp;
            if (parent != 0)
                percolateUp(a, parent, isMinHeap);
        }
    }
}

void heapify(PqData *arr, int size, int isMinHeap) {
    for (int i = (size) / 2; i >= 0; i--)
        percolateDown(arr, i, size, isMinHeap);
}

void HeapInit(Heap *hp, int size, int isMinHeap) {
    hp->size = 0;
    hp->capacity = size;
    hp->isMinHeap = isMinHeap;
    hp->arr = (PqData *)malloc(sizeof(PqData) * (size + 1));
}

void HeapAdd(Heap *hp, int val, int index) {
    if (hp->size == hp->capacity)
        return;
    
    hp->size++;
    hp->arr[hp->size - 1].key = val;
    hp->arr[hp->size - 1].idx = index;
    percolateUp(hp->arr, hp->size - 1, hp->isMinHeap);
}

int HeapRemove(Heap *hp) {
    int val = hp->arr[0].idx;
    hp->arr[0] = hp->arr[hp->size - 1];
    hp->size--;
    percolateDown(hp->arr, 0, hp->size, hp->isMinHeap);

    return val;
}

void HeapUpdateKey(Heap *hp, int key, int index) {
    for (int i = 0; i < hp->size - 1; i++)
    {
        if (index == hp->arr[i].idx)
        {
            hp->arr[i].key = key;
            heapify(hp->arr, hp->size, hp->isMinHeap);
            break;
        }
    }
}

void printHeap(Heap *hp) {
    for (int i = 0; i < hp->size; i++)
    {
        printf("\n%d 번째 키 : %d, 인덱스 : %d\n", i, hp->arr[i].key, hp->arr[i].idx);
    }
}

int DijkstraReturnCost(Graph *gph, int source, int dst) {
    int count = gph->count;
    int val, destination, cost;
    int initSource = source;
    GraphNode *head;
    TableNode *table = (TableNode *)malloc(sizeof(TableNode) * count);
    for (int i = 0; i < count; i++)
    {
        table[i].cost = INF;
        table[i].visited = 0;
    }

    table[source].cost = 0;

    Heap PQ;
    HeapInit(&PQ, gph->count, 1);
    int i;
    for (i = 0; i < gph->count; i++)
        HeapAdd(&PQ, INF, i);

    HeapUpdateKey(&PQ, 0, source);

    while (PQ.size != 0) 
    {
        source = HeapRemove(&PQ);
        table[source].visited = 1;
        head = gph->adj[source].next;
        while(head)
        {
            destination = head->dest;
            cost = head->cost;
            int newcost = table[source].cost + cost;
            if (table[destination].visited == 0 && newcost < table[destination].cost)
            {
                table[destination].cost = newcost;
                table[destination].prev = source;
                HeapUpdateKey(&PQ, newcost, destination);
            }
            head = head->next;
        }
    }
    
    
    if (table[dst].cost == INF)
    {
        printf("Path to Site-%d cost : Unreachable\n", dst);
    }
    else 
    {
        printf("  출발지 : Site-%d\n", dst);
        int curr = dst;
        while (table[curr].prev != initSource)
        {
            printf("  경유지 : Site-%d\n", table[curr].prev);
            curr = table[curr].prev;
        }
        printf("  도착  : Site-%d\n\n", table[curr].prev);
        printf("  이동 거리 : 총 %d km\n", table[dst].cost * 8);
        printf("  이동 비용 : 총 %d 만 원 \n", table[dst].cost * 4);
        printf("  소요 시간 : 총 %d 시간\n\n", table[dst].cost);
    }
    return table[dst].cost * 4;
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

void RB_Print(RBt *T, RbtNode* root, Trunk* prev, int isLeft) {
    if (root == T->nil) 
        return;

    char* prev_str = "    ";
    Trunk* trunk = createTrunk(prev, prev_str);

    RB_Print(T, root->right, trunk, 1);

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
    
    printf(" User-%d\n", root->key);

    if (prev != NULL) 
        prev->str = prev_str;
    trunk->str = "   |";

    RB_Print(T, root->left, trunk, 0);
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
        hotel->key = (rand() % 10 + 1) * 3; //3만원 부터 30만원까지 5만원 단위의 price 랜덤 생성
        RB_INSERT(Hotels, hotel);
    }
    return Hotels;
}

Tourday *makeItinerary(Graph *sites, int *tourPeriod, int *destN, int *destlist) {
    srand(time(NULL));
    //destination 들을 셔플해준다.
    for (int i = 0; i < (*destN); i++)
    {
        int sh = rand() % (*destN);
        int temp = destlist[i];
        destlist[i] = destlist[sh];
        destlist[sh] = temp;
    }
    Tourday *days = (Tourday *)malloc(sizeof(Tourday) * (*destN));
    for (int i = 0; i < (*destN); i++)
    {
        int siteI = destlist[i];
        days[i].siteIndex = siteI;
        days[i].tourtime = sites->siteinfo[siteI].tourTime;
        days[i].hotelTonight = Search(sites->siteinfo[siteI].hotels, sites->siteinfo[siteI].hotelroot, rand() % 50);
        days[i].hotelCost = days[i].hotelTonight->key;
    }

    return days;
    
}