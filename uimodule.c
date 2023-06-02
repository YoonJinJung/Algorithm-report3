#include "imaker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int inputUI_0() {
    int inputN;
    printf("\n<< 환 영 합 니 다 !! >>\n\n");
    printf("여행 계획은 Imaker 에게 맡기세요!\n");
    printf("알맞은 값을 입력해주세요!\n");
    printf("[ 0 ] -> 종료   [ 1 ] -> 계획 짜기   [ 2 ] -> 예약 취소\n>>> ");
    while (1)
    {
        if (scanf("%d", &inputN) != 1 || (inputN < 0 || inputN > 2)) 
        {
            printf("잘못된 입력입니다. 다시 입력하세요 : ");
            // 잘못된 입력이 발생하면 버퍼를 비운다.
            while (getchar() != '\n') 
                continue;
        }
        else
            break;
    }
    return inputN;
}

void inputUI_1(int *id, int *tourPeriod, int *destN, int *budget) {
    printf("\n 1. 사용자 id 를 입력하세요 : ");
    while (1) 
    {
        if (scanf("%d", id) != 1 || *id < 0) 
        {
            printf("잘못된 입력입니다. id를 다시 입력하세요 : ");
            // 잘못된 입력이 발생하면 버퍼를 비운다.
            while (getchar() != '\n') 
                continue;
        } 
        else 
            break;
    }

    printf("\n 2. 며칠 동안 떠나실 예정인가요? (일) : ");
    while (1) 
    {
        if (scanf("%d", tourPeriod) != 1 || *tourPeriod <= 0) 
        {
            printf("잘못된 입력입니다. 기간을 다시 입력하세요 (일): ");
            // 잘못된 입력이 발생하면 버퍼를 비운다.
            while (getchar() != '\n') 
                continue;
        } 
        else 
            break;
    }

    printf("\n 3. 예산은 어떻게 되세요? (만 원) : ");
    while (1) 
    {
        if (scanf("%d", budget) != 1 || (*budget) < 0) 
        {
            printf("잘못된 입력입니다. 몇 군데인지 다시 입력하세요 : ");
            // 잘못된 입력이 발생하면 버퍼를 비운다.
            while (getchar() != '\n') 
                continue;
        } 
        else 
            break;
    }

    printf("\n 4. 가시고 싶은 곳은 몇 군데인가요? : ");
    while (1) 
    {
        if (scanf("%d", destN) != 1 || *destN <= 0) 
        {
            printf("잘못된 입력입니다. 몇 군데인지 다시 입력하세요 : ");
            // 잘못된 입력이 발생하면 버퍼를 비운다.
            while (getchar() != '\n') 
                continue;
        } 
        else 
            break;
    }
}

void inputUI_2(int *destN, int *destlist) {
    printf("여행지는 총 %d 곳\n", *destN);
    for (int i = 0; i < *destN; i++)
    {
        printf("\n (%d 번째 도시)0 부터 99까지 가시고 싶은 도시를 골라주세요 : ", i + 1);
        while (1) 
        {
            if (scanf("%d", &destlist[i]) != 1 || (destlist[i] <= 0 || destlist[i] >= 100)) 
            {
                printf("잘못된 입력입니다. 여행지를 다시 입력하세요 : ");
                // 잘못된 입력이 발생하면 버퍼를 비운다.
                while (getchar() != '\n') 
                    continue;
            } 
            else 
                break;
        }
    }
}

void outputUI_1(Graph *sites, Tourday *days, int destN, int tourPeriod, int budget) {
    int sumBudget = 0;
    for (int i = 0; i < destN; i++)
    {
        printf("\n*** %d Day 의 일정 ***\n\n", i + 1);
        if (i != 0)
            sumBudget += DijkstraReturnCost(sites, days[i].siteIndex, days[i - 1].siteIndex);
        printf("여행장소 : Site-%d\n", days[i].siteIndex);
        printf("예상 소요 시간 : 약 %d 시간\n", days[i].tourtime);
        printf("숙박 장소 : Hotel-%d , 가격 : %d 만 원\n", days[i].hotelTonight->idx, days[i].hotelCost);
        sumBudget += days[i].hotelCost;
        printf("\n%d Day 일정 종료\n\n", i + 1);
    }
    printf("총 예상 비용은 %d 만 원 입니다.\n", sumBudget);
    if (budget < sumBudget) 
        printf("-> 여행 경비 %d 만 원이 부족합니다.\n", sumBudget - budget);
    else
        printf("-> 여행 경비 %d 만 원이 남습니다.\n", budget - sumBudget);
    
    if (tourPeriod < destN)
        printf("-> 여행 기간을 %d 일 늘리거나 장소를  %d 곳 줄여보세요!\n", destN - tourPeriod, destN - tourPeriod);
    else
        printf("-> 여행 기간이 %d 일 남습니다. 다른 장소도 여행해보세요!\n", tourPeriod - destN);

    printf("여행 끝!\n");
}

void outputUI_2(RBt *Reservations) {
    Trunk *prev = NULL;
    RB_Print(Reservations, Reservations->root, prev, 1);
    int id;
    printf("\n-> 취소하고 싶은 예약의 id 를 입력하세요\n(주의 : 정확하게 입력하지 않을 시 가까운 id가 삭제됩니다.) >>> ");
    while (1) 
    {
        if (scanf("%d", &id) != 1 || id < 0) 
        {
            printf("잘못된 입력입니다. 여행지를 다시 입력하세요 : ");
            // 잘못된 입력이 발생하면 버퍼를 비운다.
            while (getchar() != '\n') 
                continue;
        } 
        else 
            break;
    }
    RbtNode *forDelete = Search(Reservations, Reservations->root, id);
    RB_DELETE(Reservations, forDelete);
    prev = NULL;
    printf("\n");
    RB_Print(Reservations, Reservations->root, prev, 1);
}