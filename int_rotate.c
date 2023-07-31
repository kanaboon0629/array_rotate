/* To compile this file, first try:
   gcc -O -fno-inline -I../include int_rotate.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 16
#define B 8

int s = 0;

int g[N][N];
int pre_g[N][N];

void naive_rotate(int n, int src[n][n], int dst[n][n]) {
    int i, j;    
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dst[n-1-j][i] = src[i][j];

    s++;
    return;
}

//ブロック化とループのアンローリング(+=4)と手続き呼び出し
void rotate(int n, int src[n][n], int dst[n][n]) {
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

    //回転が出来ていることを確認する
    int j=rand() % N;
    int i=rand() % N;
    printf("\n%d\n", pre_g[N-1-j][i]);
    printf("%d\n", g[i][j]);
    
    return 0;
}