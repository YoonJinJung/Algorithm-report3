#include <stdio.h>
#include <stdlib.h>
#include "imaker.h"

int main() {
    Graph gph;
    graphInit(&gph, 100);
    do {
        siteRandomInit(&gph, 100, 300, 100);
    } while (!isConected(&gph));
    Trunk *prev = NULL;
    printTree(gph.siteinfo[10].hotels, gph.siteinfo[10].hotelroot, prev, 0);
    graphPrint(&gph);
    int *id = (int *)malloc(sizeof(int));
    int *tourPeriod = (int *)malloc(sizeof(int));
    int *budget = (int *)malloc(sizeof(int));
    int *destN = (int *)malloc(sizeof(int));
    inputUI_1(id, tourPeriod, budget, destN);
    int *destlist = (int *)malloc(sizeof(int) * (*destN));
    printf(" id : %d\n tourperiod : %d\n destN : %d\n budget : %d\n", *id, *tourPeriod, *destN, *budget);
    inputUI_2(destN, destlist);
    for (int i = 0; i < *destN; i++)
        printf("dest %d : %d\n", i, destlist[i]);
    return 1;
}