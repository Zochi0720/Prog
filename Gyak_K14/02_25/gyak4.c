#include <stdio.h>
#include <stdlib.h>

// putchar() - Egy karakter kiírása
// puts   - Sztring kiírása + '\n'

int main()
{
int n;
printf("Adjon meg egy szamot!\n");
scanf("%d",&n);

for(int i=0;i<n;++i)
{
	for (int j=0;j<=i;++j)
	{
	putchar(j == 0 || i == j || i == n-1 ? '*':' ');       // Háromszög belsejének kihagyása

	}
	putchar('\n');


}





	return 0;
}
