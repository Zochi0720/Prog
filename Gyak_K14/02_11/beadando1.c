#include <stdio.h>

int main()
{
int n;
printf("Adjon meg egy pozitiv egesz szamot, majd usse le az ENTER billentyut!\n");
scanf("%d",&n);
if (n <= 0)
    {

do
{
    printf("A megadott szam nem pozitiv!\nAdjon meg egy pozitiv egesz szamot, majd usse le az ENTER billentyut!\n");
   scanf("%d",&n);
} while (n<=0);
    }
for (int i=1;i<=n;++i)
{

if (i % 2 == 0)
	printf("%d\n",i);

}

	return 0;
}
