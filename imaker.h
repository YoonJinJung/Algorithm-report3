#ifndef GraphNode_H
#define GraphNode_H
#include <stdlib.h>

/*typedef struct pathdata {
    int src;
    int dst;
    int *transArr;
    int transtime;
    int transcost;
} PathData;*/

//RBtree 구조체 선언
typedef struct treeNode {
    int idx;
    int key;
    struct tourday *days;
    struct treeNode *p;
    struct treeNode *left;
    struct treeNode *right;
    unsigned color;
} RbtNode;

typedef struct Tree {
    struct treeNode *root;
    struct treeNode *nil;
} RBt;

typedef struct trunk {
    struct trunk* prev;
    char* str;
} Trunk;

//그래프 구조체 선언
typedef struct siteData {
    RBt *hotels;
    RbtNode *hotelroot;
    int tourTime;
} SiteData;

typedef struct graphNode {
    int vertex;
    int cost;
    int dest;
    struct graphNode *next;
} GraphNode;

typedef struct graph {
    int count;
    SiteData *siteinfo;
    GraphNode *adj;
} Graph;
//우선순위 큐 구조체
typedef struct queuedata {
    int key;
    int idx;
} PqData;

typedef struct heap {
    int capacity;
    int size;
    PqData *arr;
    int isMinHeap;
} Heap;

typedef struct tourday {
    int siteIndex;
    int tourtime;
    RbtNode *hotelTonight;
    int hotelCost;
} Tourday;

typedef struct table {
    int cost;
    int visited;
    int prev;
} TableNode;

//그래프 함수들
void graphInit(Graph *gph, int count);
int addDirectedEdge(Graph *gph, int src, int dst, int cost);
int addUndirectedEdge(Graph *gph, int src, int dst, int cost);
void graphPrint(Graph *gph);
void siteRandomInit(Graph *sites, int siteN, int transN, int hotelN);
int DFS(Graph *gph, int source, int target);
int isConected(Graph *gph);
//다익스트라 알고리즘 함수들
int compare(int a, int b, int isMinHeap);
void percolateDown(PqData *a, int position, int size, int isMinHeap);
void percolateUp(PqData *a, int position, int isMinHeap);
void heapify(PqData *arr, int size, int isMinHeap);
void HeapInit(Heap *hp, int size, int isMinHeap);
void HeapAdd(Heap *hp, int val, int index);
int HeapRemove(Heap *hp);
void HeapUpdateKey(Heap *hp, int key, int index);
void printHeap(Heap *hp);
int DijkstraReturnCost(Graph *gph, int source, int dst);

//RBtree 함수들
RBt *RBTInit(void);
void LeftRotate(RBt *T, RbtNode *x);
void RightRotate(RBt *T, RbtNode *x);
void RB_INSERT(RBt *T, RbtNode *z); 
void RB_INSERT_FIXUP(RBt *T, RbtNode *z);
RbtNode *Successor(RBt *T, RbtNode *x);
RbtNode *Minimum(RBt *T, RbtNode *x);
RbtNode *Search(RBt *T, RbtNode *x, int k);
void RB_DELETE(RBt *T, RbtNode *z);
void RB_DELETE_FIXUP(RBt *T, RbtNode *z);
Trunk* createTrunk(Trunk* prev, char* str);
void showTrunks(Trunk* p);
void RB_Print(RBt *T, RbtNode* root, Trunk* prev, int isLeft);

RBt *hotelInit(int hotelN);
Tourday *makeItinerary(Graph *sites, int *tourPeriod, int *destN, int *destlist);

int inputUI_0();
void inputUI_1(int *id, int *tourPeriod, int *budget, int *destN);
void inputUI_2(int *destN, int *destlist);
void outputUI_1(Graph *sites, Tourday *days, int destN, int tourPeriod, int budget);
void outputUI_2(RBt *Reservations);
#endif