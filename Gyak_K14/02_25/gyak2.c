#include <stdio.h>
#include <stdlib.h>
#include <math.h> // -lm kapcsol�

int teglalap_terulet(int a, int b) // form�lis param�terlista
{
return a*b;
}
int teglalap_kerulete(int a, int b)
{
return 2*(a+b);
}
double kor_terulet(int r)
{
return r*r*acos(r); // Visual Studio Code intk�nt kezeli!
}


int main()
{
// void: Elj�r�sok     nem void: F�ggv�nyek, valamilyen �rt�ket visszaad!	
int a,b;
printf("Kerek ket szamot!\n");
scanf("%d%d",&a,&b);
printf("A teglalap terulete:%d\n",teglalap_terulet(a,b)); // aktu�lis param�terlista
printf("A teglalap kerulete:%d\n",teglalap_kerulete(a,b));
	
	
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
