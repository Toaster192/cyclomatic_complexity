#include <stdio.h>

int main(int argc, char *argv[])
{
	int a = argv[0][0];
	int b = argv[1][0];
	if (a > 0 && b > 0){
		while(a > b){
			if(a%2 && b%2)
				printf("%d\n", a);
			else
				printf("%d\n", b);
			a--;
		}
	}
	return 0;
}
