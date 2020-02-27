#include <stdio.h>

int main(){

int n;
    printf("Kerek egy pozitiv egesz szamot!\n");
    scanf("%d",&n);

    while (n <= 0)
    {
        printf("A megadott szam nem pozitiv egesz szam!\nKerek egy pozitiv egesz szamot!\n");
        scanf("%d",&n);
    }
//int i;
    for(int i=1;i<=n;++i)
    {
        if (i % 4 == 0)
        {
            printf("%d\n",i);
        }

    }






    return 0;
}