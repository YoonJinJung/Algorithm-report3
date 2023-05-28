#include <stdio.h>
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
    return 1;
}