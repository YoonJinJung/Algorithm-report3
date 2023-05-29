#ifndef GraphNode_H
#define GraphNode_H
#include <stdlib.h>

//RBtree 구조체 선언
typedef struct treeNode {
    int idx;
    int key;
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

typedef struct graphEdge {
    int cost;
    int dest;
    struct graphEdge *next;
} GraphEdge;

typedef struct graph {
    int count;
    SiteData *siteinfo;
    GraphEdge *adj;
} Graph;

void graphInit(Graph *gph, int count);
int addDirectedEdge(Graph *gph, int src, int dst, int cost);
int addUndirectedEdge(Graph *gph, int src, int dst, int cost);
void graphPrint(Graph *gph);
void siteRandomInit(Graph *sites, int siteN, int transN, int hotelN);
int DFS(Graph *gph, int source, int target);
int isConected(Graph *gph);

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
void printTree(RBt *T, RbtNode* root, Trunk* prev, int isLeft);

RBt *hotelInit(int hotelN);

void makeIti();

void inputUI_1(int *id, int *tourPeriod, int *budget, int *destN);
void inputUI_2(int *destN, int *destlist);
#endif