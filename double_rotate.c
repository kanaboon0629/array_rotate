/* To compile this file, first try:
   gcc -O -fno-inline -I../include double_rotate.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 16
#define B 8

int s = 0;

double g[N][N];
double pre_g[N][N];

void naive_rotate(int n, double src[n][n], double dst[n][n]) {
    int i, j;    
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dst[n-1-j][i] = src[i][j];

    s++;
    return;
}

void printarray(int n, double a[n][n]) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%1.1f ", a[i][j]);
        }
        printf("\n");
    }
}



int main(int argc, char *argv[]) {

    // 乱数生成器の初期化
    srand(time(NULL));
    //ランダムな配列の初期化
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            g[i][j] = rand() % 10;
        }
    }

    naive_rotate(N, g, pre_g);
    //rotate(N, g, pre_g);

    //配列の表示
    printarray(N, g);
    printf("\n↓\n\n");
    printarray(N, pre_g);
    
  
    return 0;
}