#include <stdio.h>
#include "imaker.h"

int main() {
    Graph gph;
    graphInit(&gph, 100);
    do {
        siteRandomInit(&gph, 100, 300);
    } while (!isConected(&gph));
    
    graphPrint(&gph);
    return 1;
}