#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "imaker.h"

#define INF 100000

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