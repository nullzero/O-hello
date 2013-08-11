#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <string>

#define printf printw
#define FILENAME ".ohello.getch"
const char* PROMPT = ">>> ";

void printw(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	fflush(stdout);
}

char _getch(){
	char ch;
	if(system("read -s -n 1 ohello_getch; echo $ohello_getch > " FILENAME) != 0) exit(0);
	FILE* fp = fopen(FILENAME, "r");
	fscanf(fp, "%c", &ch);
	fclose(fp);
	system("rm " FILENAME);
	return ch;
}

char getch(bool ignoreBack=false){
	if(not ignoreBack) return _getch();
	char ch;
	do{
		ch = _getch();
	}while(ch == 127);
	return ch;
}

void alert(std::string s){
	printf("\n%s\n\n", s.c_str());
}

void clrscr(){
	system("clear");
}

void presstogo(){
	printf("press any key to continue");
	getch();
}

std::string getString(){
	std::string x;
	std::cin >> x;
	return x;
}

int getInt(){
	int a;
	scanf("%d", &a);
	return a;
}

float getFloat(){
	float a;
	scanf("%f", &a);
	return a;
}
