/* To compile this file, first try:
   gcc -O -fno-inline -I../include char_rotate.c ../src/clock64.o
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 16
#define B 4

int s = 0;

char g[N][N];
char pre_g[N][N];

void naive_rotate(int n, char src[n][n], char dst[n][n]) {
    int i, j;
    char temp[n][n]; 
    
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            dst[n-1-j][i] = src[i][j];

    //大域変数の値を変更する副作用
    s++;
    return;
}

void printarray(int n, char a[n][n]) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%c ", a[i][j]);
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
            g[i][j] = rand() % 26 + 'A';
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
