#include <stdio.h>

void bal(char c){
    putchar(c);
    puts("  ");
}

void jobb(char c){
    printf("  ");
    putchar(c);
    putchar('\n');
}

void bal_jobb(char c){
    printf("%c %c\n", c, c);
}

void teli(char c){
    printf("%c%c%c\n", c, c, c);
}

void szamjegy0(char c){
    teli(c);
    for(int i=0; i<3; i++){
        bal_jobb(c);
    }
    teli(c);
}

int main() {
    int x;
    char c;

    printf("kerem a szamjegyet!\n");
    scanf("%d", &x);
    printf("kerem a karaktert!\n");
    getchar(); // soremelés beolvasása
    c = getchar();

    if(x == 0){
        szamjegy0(c);
    }

    // TODO

    return 0;
}
