#include <stdio.h>
#include <stdlib.h>
#include <math.h> // -lm kapcsoló

int teglalap_terulet(int a, int b) // formális paraméterlista
{
return a*b;
}
int teglalap_kerulete(int a, int b)
{
return 2*(a+b);
}
double kor_terulet(int r)
{
return r*r*acos(r); // Visual Studio Code intként kezeli!
}


int main()
{
// void: Eljárások     nem void: Függvények, valamilyen értéket visszaad!	
int a,b;
printf("Kerek ket szamot!\n");
scanf("%d%d",&a,&b);
printf("A teglalap terulete:%d\n",teglalap_terulet(a,b)); // aktuális paraméterlista
printf("A teglalap kerulete:%d\n",teglalap_kerulete(a,b));
	
	
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
