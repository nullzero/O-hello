#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#define FILENAME ".ohello.getch"

char getch(){
	char ch;
	system("read -e -n 1 ohello_getch; echo $ohello_getch > " FILENAME);
	FILE* fp = fopen(FILENAME, "r");
	fscanf(fp, "%c", &ch);
	fclose(fp);
	system("rm " FILENAME);
	return ch;
}

void printw(const char* fmt, ...){
	//printf("oak\n");
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	fflush(stdout);
}