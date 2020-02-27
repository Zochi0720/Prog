#include <stdio.h>

int main() {

int n,m;
printf("Kerem adjon meg ket pozitiv egesz szamot szokozzel elvalasztva!\n");
scanf("%d %d",&n,&m);
while (n<=0 || m<=0 )
{
printf("A megadott szamok kozul valamely szam(ok) nem pozitiv szam(ok)!\nKerem adjon meg ket pozitiv egesz szamot szokozzel elvalasztva!\n");
scanf("%d %d",&n,&m);
}
int i = 1;
for (;i<=n;++i)
{
    if (i % m == 0)
    {
        printf("%d\n",i);
    }
}
    return 0;
			}
