#include <stdio.h>
#include <stdlib.h>

// putchar() - Egy karakter kiírása
// puts   - Sztring kiírása + '\n'
// getchar(karakter beolvasása) ASCII

void ures (char c, int hanyszor)
{
    for (int i=1;i<=hanyszor;i++) printf("   \n");
}

void bal(char c, int hanyszor)
{
	putchar(c);
	//return; Az eljárás véget ér.
	puts("    ");
}
void jobb (char c, int hanyszor)
{
    for(int i=1;i<=hanyszor;i++)
    {
    printf("    ");
	printf("%c\n",c);
    }
}
void bal_jobb (char c, int hanyszor)
{
    for (int i=1;i<=hanyszor;i++)
    {
    putchar(c);
	printf("   ");
	putchar(c);
	printf("\n");
    }
}
void jobbnem(char c, int hanyszor)
{
    for(int i=1;i<=hanyszor;i++)
        printf("%c %c  \n",c,c);
}

void teli (char c, int hanyszor)
{
    for (int i=1;i<=hanyszor;i++)
        printf("%c %c %c\n",c,c,c);

}

void kozep (char c, int hanyszor)
{
    for (int i=1;i<=hanyszor;i++)
    {
    printf("  ");
    putchar(c);
    puts("  ");
    }
}
void szamjegy0(char c)
{
	teli(c,1);
	bal_jobb(c,3);
	teli(c,1);
/*for(int i=0;i<3;i++)
	{
		bal_jobb(c);
	}
*/
}

void szamjegy1(char c)
{
	jobb(c,5);
}
void szamjegy2(char c)
{
    teli(c,1); //
    jobb(c,1);
    teli(c,1);
    bal(c,1);
    teli(c,1);
}
void szamjegy3(char c)
    {
      teli(c,1);//teli(c,1);
      jobb(c,1);
      teli(c,1);
      jobb(c,1);
      teli(c,1);//teli(c,1);//
    }
void szamjegy4 (char c)
{
    bal_jobb(c,2);
    teli(c,1);
    jobb(c,2);


}
void szamjegy5 (char c)
{
    teli(c,1);
    bal(c,1);
    teli(c,1);
    jobb(c,1);
    teli(c,1);
}
void szamjegy6 (char c)
{
    teli(c,1);
    bal(c,1);
    teli(c,1);
    bal_jobb(c,1);
    teli(c,1);
}
void szamjegy7 (char c)
{
    teli(c,1);
    jobb(c,4);
}
void szamjegy8 (char c)
{
    teli(c,1);
    bal_jobb(c,1);
    teli(c,1);
    bal_jobb(c,1);
    teli(c,1);
}
void szamjegy9 (char c)
    {
    teli(c,1);
    bal_jobb(c,1);
    teli(c,1);
    jobb(c,1);
    teli(c,1);
    }

int main()
{
int szamjegy;
char c;
printf("Adjon meg egy szamot!\n");
scanf("%d",&szamjegy);
printf("Kerem a karaktert!\n");

getchar(); // soremelés karakter a scanf beolvasás után!
c = getchar();
puts("-----------------");
	if (szamjegy == 0) szamjegy0(c);
	if (szamjegy == 1) szamjegy1(c);
	if (szamjegy == 2) szamjegy2(c);
	if (szamjegy == 3) szamjegy3(c);
	if (szamjegy == 4) szamjegy4(c);
	if (szamjegy == 5) szamjegy5(c);
	if (szamjegy == 6) szamjegy6(c);
	if (szamjegy == 7) szamjegy7(c);
	if (szamjegy == 8) szamjegy8(c);
    if (szamjegy == 9) szamjegy9(c);
    //if (szamjegy == 42) szamjegy4(c); szamjegy2(c);

	return 0;
}
