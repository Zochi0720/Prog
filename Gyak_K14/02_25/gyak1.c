#include <stdio.h>
#include <stdlib.h>

int main() {
	int x;
	srand(time(NULL)); // processzor idõ, inicializálás(srand) random számoknak!
	x=rand();

	printf("%d\n",(x%4)+1); // x%4 0-3-ig generál, hozzáadunk egyet akkor 1-4-ig.
	return 0;
}
