/* To compile this file, first try:
gcc -O -fno-inline -I../include double_naive.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h> 
#include "clock.h"

#define N 64
#define B 128

double src64[N][N];
double  dst64[N][N];
double  src128[2 * N][2 * N];
double dst128[2 * N][2 * N];
double  src256[4 * N][4 * N];
double  dst256[4 * N][4 * N];
double  src512[8 * N][8 * N];
double dst512[8 * N][8 * N];
double src1024[16 * N][16 * N];
double  dst1024[16 * N][16 * N];
double  src2048[32 * N][32 * N];
double  dst2048[32 * N][32 * N];

int s = 0;

void naive_rotate(int n, double  src[n][n], double  dst[n][n]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dst[n-1-j][i] = src[i][j];

    //大域変数の値を変更する副作用
    s++;
    return;
}

double run(int n,double  src[n][n], double dst[n][n]){
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
    t = get_counter();

    return t;
}

int main(int argc, char *argv[]) {

    printf("%4s %4s%d\n", "N", "B=", B);
    printf("%4d %6.2f\n", N, run(N,src64,dst64)/(N*N));
    printf("%4d %6.2f\n", 2*N, run(2*N,src128,dst128)/(2*N*2*N));
    printf("%4d %6.2f\n", 4*N, run(4*N,src256,dst256)/(4*N*4*N));
    printf("%4d %6.2f\n", 8*N, run(8*N,src512,dst512)/(8*N*8*N));
    printf("%4d %6.2f\n", 16*N, run(16*N,src1024,dst1024)/(16*N*16*N));
    printf("%4d %6.2f\n", 32*N, run(32*N, src2048, dst2048)/(32*N*32*N));    

    return 0;
}
