/* To compile this file, first try:
gcc -O -fno-inline -I../include double.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h> 
#include "clock.h"

#define N 64
#define B 2048

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

//ブロック化とループのアンローリング(+=4)と手続き呼び出し
void rotate(int n, double src[n][n], double dst[n][n]) {
    int i, j, k, l;
    int remaining_rows, remaining_cols;

    //ブロックサイズが配列の大きさと同じ場合は1つのブロックとして処理
    //配列の大きさが4の倍数ならばアンローリング(+=4)
    if (n == B) {
        if(n%4==0){
            for (k = 0; k < B; k += 4) {
                int nk = n - 1 - k;
                int nk1 = nk - 1;
                int nk2 = nk - 2;
                int nk3 = nk - 3;
                
                for (l = 0; l < B; l += 4) {
                    int nl = n - 1 - l;
                    int nl1 = nl - 1;
                    int nl2 = nl - 2;
                    int nl3 = nl - 3;
                    
                    dst[nl][k] = src[k][l];
                    dst[nl][k + 1] = src[k + 1][l];
                    dst[nl][k + 2] = src[k + 2][l];
                    dst[nl][k + 3] = src[k + 3][l];

                    dst[nl1][k] = src[k][l + 1];
                    dst[nl1][k + 1] = src[k + 1][l + 1];
                    dst[nl1][k + 2] = src[k + 2][l + 1];
                    dst[nl1][k + 3] = src[k + 3][l + 1];

                    dst[nl2][k] = src[k][l + 2];
                    dst[nl2][k + 1] = src[k + 1][l + 2];
                    dst[nl2][k + 2] = src[k + 2][l + 2];
                    dst[nl2][k + 3] = src[k + 3][l + 2];

                    dst[nl3][k] = src[k][l + 3];
                    dst[nl3][k + 1] = src[k + 1][l + 3];
                    dst[nl3][k + 2] = src[k + 2][l + 3];
                    dst[nl3][k + 3] = src[k + 3][l + 3];
                }
            }
            return;

        }else{
            for (k = 0; k < B; k++) {
                for (l = 0; l < B; l++) {
                    dst[n - 1 - l][k] = src[k][l];
                }
            }
            return;
        }
    }

    //ブロックの大きさが配列の大きさでない場合はブロック化とアンローリング(+=4)
    for (i = 0; i < n -B+1; i += B) {
        for (j = 0; j < n-B+1; j += B) {
            for (k = i; k < i + B; k += 4) {
                int nk = n - 1 - k;
                int nk1 = nk - 1;
                int nk2 = nk - 2;
                int nk3 = nk - 3;
                
                for (l = j; l < j + B; l += 4) {
                    int nl = n - 1 - l;
                    int nl1 = nl - 1;
                    int nl2 = nl - 2;
                    int nl3 = nl - 3;
                    
                    dst[nl][k] = src[k][l];
                    dst[nl][k + 1] = src[k + 1][l];
                    dst[nl][k + 2] = src[k + 2][l];
                    dst[nl][k + 3] = src[k + 3][l];

                    dst[nl1][k] = src[k][l + 1];
                    dst[nl1][k + 1] = src[k + 1][l + 1];
                    dst[nl1][k + 2] = src[k + 2][l + 1];
                    dst[nl1][k + 3] = src[k + 3][l + 1];

                    dst[nl2][k] = src[k][l + 2];
                    dst[nl2][k + 1] = src[k + 1][l + 2];
                    dst[nl2][k + 2] = src[k + 2][l + 2];
                    dst[nl2][k + 3] = src[k + 3][l + 2];

                    dst[nl3][k] = src[k][l + 3];
                    dst[nl3][k + 1] = src[k + 1][l + 3];
                    dst[nl3][k + 2] = src[k + 2][l + 3];
                    dst[nl3][k + 3] = src[k + 3][l + 3];
                }
            }
        }
    }

    // 残りの行のブロック処理
    remaining_rows = n % B;
    for (i = n - remaining_rows; i < n; i++) {
        for (j = 0; j < n; j++) {
            dst[n - 1 - j][i] = src[i][j];
        }
    }

    // 残りの列のブロック処理
    remaining_cols = n % B;
    for (j = n - remaining_cols; j < n; j++) {
        for (i = 0; i < n; i++) {
            dst[n - 1 - j][i] = src[i][j];
        }
    }
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
    rotate(n, src, dst);
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
