#include <stdio.h>

int main(){
    int n;

    printf("kerem az n erteket!\n");
    scanf("%d", &n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j <= i; j++){
            putchar('*');
        }
        //putchar('\n');
        //printf("\n");
        puts("");
    }

     for(int i = 0; i < n; i++){
        for(int j = 0; j <= i; j++){
            putchar(j == 0 || i == j || i == n - 1 ? '*' : ' ');
        }
        //putchar('\n');
        //printf("\n");
        puts("");
    }

    return 0;
}
