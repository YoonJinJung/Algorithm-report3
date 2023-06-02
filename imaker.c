#include <stdio.h>
#include <stdlib.h>
#include "imaker.h"

int main() {
    //site 랜덤으로 100곳, Transportation 랜덤 300개 생성
    Graph sites;
    graphInit(&sites, 100);
    do {
        siteRandomInit(&sites, 100, 300, 100);
    } while (!isConected(&sites));
    RBt *Reservations = RBTInit();
    while(1)
    {
        int N = inputUI_0();
        if (N == 0)
            return 1;
        else if (N == 1)
        {
            int *id = (int *)malloc(sizeof(int));
            int *tourPeriod = (int *)malloc(sizeof(int));
            int *budget = (int *)malloc(sizeof(int));
            int *destN = (int *)malloc(sizeof(int));
            //inputUI_1을 통해 아이디, 여행기간, 예산, 여행장소 개수를 받는다.
            inputUI_1(id, tourPeriod, destN, budget);

            int *destlist = (int *)malloc(sizeof(int) * (*destN));
            //printf(" id : %d\n tourperiod : %d\n destN : %d\n budget : %d\n", *id, *tourPeriod, *destN, *budget);
            //inputUI_2를 통해 여행 목적지를 입력받는다.
            inputUI_2(destN, destlist);

            Tourday *days = makeItinerary(&sites, tourPeriod, destN, destlist);
            RbtNode *reserve = (RbtNode *)malloc(sizeof(RbtNode));
            reserve->key = *id;
            reserve->days = days;
            RB_INSERT(Reservations, reserve); // 레드블랙트리에 예약정보 저장.
            printf("\n\nUser-%d 님의 일정입니다!\n\n", *id);
            outputUI_1(&sites, days, *destN, *tourPeriod, *budget);
        }

        else
        {
            outputUI_2(Reservations);
        }
    }
    
}