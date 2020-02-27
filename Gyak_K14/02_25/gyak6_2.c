#include <stdio.h>
#include <stdlib.h>

int elfer_e (int a, int b, int c)
{
	if (a < b)
	{
int csere = a;
	a = b;
	b = csere;
	}
		if (a < c)
		{
        int csere = a;
		a = b;
		b = csere;
		}

	if (b < c)
	{
	int csere = b;
	b = c;
	c = csere;
	}

	return a <= 55 && b <=40 && c <= 23; //Wizz Air FEDÉLZETI POGGYÁSZ követelmény
}


int main()
{

	int a,b,c;
	char koztes1,koztes2;
	printf("Kerem adja meg a borond 3 meret jellemzojet!\t\t");
	scanf("%d%c%d%c%d",&a,&koztes1,&b,&koztes2,&c);
        while (koztes1 != 'x' || koztes2 != 'x')
        {
        printf("Kerem hasznalja az x kisbetûs karakteret a parameterek megadasa kozott!\n");
        printf("Kerem adja meg a borond 3 meret jellemzojet!\t\t");
        scanf("%d%c%d%c%d",&a,&koztes1,&b,&koztes2,&c);
        }

	printf("Az On borondje megfelel-e a kovetelmenyeknek?\t\t%s\n",elfer_e(a,b,c)? "IGEN" :"NEM");
	//puts(elfer_e(a,b,c)? "IGEN" :"NEM");




	return 0;
}
