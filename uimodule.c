#include "imaker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputUI_1(int *id, int *tourPeriod, int *destN, int *budget) {
    printf("\n<< 환 영 합 니 다 !! >>\n\n");
    printf("여행 계획은 Imaker 에게 맡기세요!\n");
    printf("절차에 따라 알맞은 값을 입력해주세요!\n");

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