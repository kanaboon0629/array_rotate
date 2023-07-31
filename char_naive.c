/* To compile this file, first try:
gcc -O -fno-inline -I../include char_naive.c ../src/clock64.o
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h> 
#include "clock.h"

#define N 64
#define B 64

char src64[N][N];
char dst64[N][N];
char src128[2 * N][2 * N];
char dst128[2 * N][2 * N];
char src256[4 * N][4 * N];
char dst256[4 * N][4 * N];
char src512[8 * N][8 * N];
char dst512[8 * N][8 * N];
char src1024[16 * N][16 * N];
char dst1024[16 * N][16 * N];
char src2048[32 * N][32 * N];
char dst2048[32 * N][32 * N];

int s = 0;

void naive_rotate(int n, char src[n][n], char dst[n][n]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dst[n-1-j][i] = src[i][j];

    //大域変数の値を変更する副作用
    s++;
    return;
}

double run(int n, char src[n][n], char dst[n][n]) {
    double t;
    int size = sizeof(char) * n * n;

    // 乱数生成器の初期化
    srand(time(NULL));

    // ランダムなchar型の文字を代入
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            src[i][j] = rand() % 256; // 0～255のランダムな整数を代入
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
