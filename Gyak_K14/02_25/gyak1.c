#include <stdio.h>
#include <stdlib.h>

int main() {
	int x;
	srand(time(NULL)); // processzor id�, inicializ�l�s(srand) random sz�moknak!
	x=rand();

	printf("%d\n",(x%4)+1); // x%4 0-3-ig gener�l, hozz�adunk egyet akkor 1-4-ig.
	return 0;
}
