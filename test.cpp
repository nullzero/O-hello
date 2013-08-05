#include <cstdio>

struct Oak{
	char a[3][3];
	int b;
};

Oak a = {
	.a = {"12", "12", "12"};
	.b = 0;
};

int main(){
	printf("%c", a.a[0][0]);
	return 0;
}