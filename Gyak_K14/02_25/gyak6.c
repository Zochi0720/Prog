#include <stdio.h>

int elfer_e(int a, int b, int c){
    if(a < b) {
        int csere = a;
        a = b;
        b = csere;
    }
    if(a < c) {
        int csere = a;
        a = c;
        c = csere;
    }
    if(b < c){
        int csere = b;
        b = c;
        c = csere;
    }

    return a <= 55 && b <= 40 && c <= 23;
}

int main(){
    int a, b ,c;

    scanf("%d %d %d", &a, &b, &c);
    puts(elfer_e(a, b, c) ? "YES" : "NO");

    return 0;
}
