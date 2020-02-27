#include <stdio.h>

int main() 
{
int n,m;
printf("Kerek ket pozitiv egesz szamot szokozzel elvalasztva!\n");
scanf("%d %d",&n,&m);
while (n<=0 || m<=0)
{
    printf("A megadott szamok valamelyike nem pozitiv egesz szam!\nKerek ket pozitiv egesz szamot szokozzel elvalasztva!\n");
    scanf("%d %d",&n,&m);
}

if (n > m) printf(">\n");
else if (n == m) printf("=\n");
else printf("<\n");
    return 0;
}
