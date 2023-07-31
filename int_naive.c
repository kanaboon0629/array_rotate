/* To compile this file, first try:
gcc -O -fno-inline -I../include int_naive.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h> 
#include "clock.h"

#define N 64

int src64[N][N];
int dst64[N][N];
int src128[2 * N][2 * N];
int dst128[2 * N][2 * N];
int src256[4 * N][4 * N];
int dst256[4 * N][4 * N];
int src512[8 * N][8 * N];
int dst512[8 * N][8 * N];
int src1024[16 * N][16 * N];
int dst1024[16 * N][16 * N];
int src2048[32 * N][32 * N];
int dst2048[32 * N][32 * N];

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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            src[i][j] = rand();
        }
    }

    start_counter();
    naive_rotate(n, src, dst);
    t = get_counter();

    //大域変数の値を変更する副作用
    s++;
    return t;
}

int main(int argc, char *argv[]) {

    printf("%4s  %4s\n", "N", "naive");
    printf("%4d %6.2f\n", N, run(N,src64,dst64)/(N*N));
    printf("%4d %6.2f\n", 2*N, run(2*N,src128,dst128)/(2*N*2*N));
    printf("%4d %6.2f\n", 4*N, run(4*N,src256,dst256)/(4*N*4*N));
    printf("%4d %6.2f\n", 8*N, run(8*N,src512,dst512)/(8*N*8*N));
    printf("%4d %6.2f\n", 16*N, run(16*N,src1024,dst1024)/(16*N*16*N));
    printf("%4d %6.2f\n", 32*N, run(32*N, src2048, dst2048)/(32*N*32*N));    

    return 0;
}
