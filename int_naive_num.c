/* To compile this file, first try:
gcc -O -fno-inline -I../include int_naive_num.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h> 
#include "clock.h"

#define B 32

int src63[63][63];
int dst63[63][63];
int src127[127][127];
int dst127[127][127];
int src255[255][255];
int dst255[255][255];
int src511[511][511];
int dst511[511][511];
int src1021[1021][1021];
int dst1021[1021][1021];
int src2049[2049][2049];
int dst2049[2049][2049];

int s = 0;

void naive_rotate(int n, int src[n][n], int dst[n][n]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dst[n-1-j][i] = src[i][j];

    //大域変数の値を変更する副作用
    s++;
    return;
}

double run(int n, int src[n][n], int dst[n][n]){
    double t;

    // 乱数生成器の初期化
    srand(time(NULL));

    //0か1の配列
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            src[i][j] = rand();
        }
    }


    start_counter();
    naive_rotate(n, src, dst);
    //rotate(n, src, dst);
    t = get_counter();

    return t;
}

void printarray(int n, int a[n][n]) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    printf("%4s  %4s\n", "N", "naive");
    printf("%4d %6.2f\n", 63, run(63,src63,dst63)/(63*63));
    printf("%4d %6.2f\n", 127, run(127,src127,dst127)/(127*127));
    printf("%4d %6.2f\n", 255, run(255,src255,dst255)/(255*255));
    printf("%4d %6.2f\n", 511, run(511,src511,dst511)/(511*511));
    printf("%4d %6.2f\n", 1021, run(1021,src1021,dst1021)/(1021*1021));
    printf("%4d %6.2f\n", 2049, run(2049,src2049,dst2049)/(2049*2049));

    return 0;
}
