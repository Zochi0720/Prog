#include <stdio.h>

int main()
{
int szam;

while (1)
{

 scanf("%d",&szam);
 if (szam <= 0)
 {
    return 0; // break; 
 }
 printf("Pozitiv szamot adtunk meg!\n");
 // printf("%d\n",szam);

}
    return 0;
}
