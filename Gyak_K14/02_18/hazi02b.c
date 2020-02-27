#include <stdio.h>

int main()
{
int n;
printf("Kerem adjon meg egy pozitiv egesz szamot!\n");
scanf("%d",&n);

while (n <= 0)
{
printf("A megadott szam nem pozitiv egesz szam!\nKerem adjon meg egy pozitiv egesz szamot!\n");
scanf("%d",&n);
}
int i = n;
    for (;i>=1;--i)
    {   
       if (i % 2 == 1) 
       {
           printf("%d\n",i);
       }

    }
return 0;
}
