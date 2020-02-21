#include <stdio.h>

int main()
{
double a,b;
a = -5.36;
b = -2.83;
printf("A ket szam osszege: %.2lf + %.2lf = %.2lf\n",a,b,a+b);
printf("A ket szam kulonbsege: %.2lf - %.2lf = %.2lf\n",a,b,a-b);
printf("A ket szam szorzata: %.2lf * %.2lf = %.4lf\n",a,b,a*b);
printf("A ket szam hanyadosa: %.2lf / %.2lf = %.4lf\n",a,b,a/b);
return 0;
}
