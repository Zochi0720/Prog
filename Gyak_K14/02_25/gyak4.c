#include <stdio.h>
#include <stdlib.h>

// putchar() - Egy karakter ki�r�sa
// puts   - Sztring ki�r�sa + '\n'

int main()
{
int n;
printf("Adjon meg egy szamot!\n");
scanf("%d",&n);

for(int i=0;i<n;++i)
{
	for (int j=0;j<=i;++j)
	{
	putchar(j == 0 || i == j || i == n-1 ? '*':' ');       // H�romsz�g belsej�nek kihagy�sa

	}
	putchar('\n');


}





	return 0;
}
