#pragma once

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <functional>

#include "utility.h"
#include "sharedfunc.h"

#define PROMPT ">>> "

/* begin printf */
#define printf printw

void printw(const char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	fflush(stdout);
}
/* end printf */

/* begin getch */
#define _FGETCH ".ohello.getch"

char _getch(){
	char ch;
	system("stty -echo");
	if(system("read -n 1 ohello_getch; echo $ohello_getch > " _FGETCH) != 0) finish(0);
	system("stty echo");
	FILE* fp = fopen(_FGETCH, "r");
	fscanf(fp, "%c", &ch);
	fclose(fp);
	system("rm " _FGETCH);
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
/* end getch */

/* begin uget */
#define uget(...) _uget<__VA_ARGS__>::get

// default
template <typename T>
class _uget{
public:
	static T get(std::function<bool(T)> func, 
				 std::string warn = "invalid! please type again.",
				 std::string prompt = ""){
		T x;
		if(prompt != "") prompt += ": ";
		printf(">>> %s", prompt.c_str());
		std::cin >> x;
		while(not func(x)){
			printf("\n%s\n\n", warn.c_str());
			printf(">>> %s", prompt.c_str());
			std::cin >> x;
		}
		return x;
	};
	
	static T get(std::string prompt = ""){
		T x;
		if(prompt != "") prompt += ": ";
		printf(">>> %s", prompt.c_str());
		std::cin >> x;
		return x;
	};
};

// sline
struct sline{};

template <>
class _uget<sline>{
public:
	static std::string get(std::function<bool(std::string)> func,
						   std::string warn = "invalid! please type again.",
						   std::string prompt = ""){
		if(prompt != "") prompt += ": ";
		printf(">>> %s", prompt.c_str());
   		std::string x;
   		std::getline(std::cin, x);
   		while(not func(x)){
			printf("\n%s\n\n", warn.c_str());
			printf(">>> %s", prompt.c_str());
   			std::getline(std::cin, x);
   		}
   		return x;
	};
	
	static std::string get(std::string prompt = ""){
		if(prompt != "") prompt += ": ";
		printf(">>> %s", prompt.c_str());
   		std::string x;
   		std::getline(std::cin, x);
   		return x;
	};
};
/* end uget */

/* begin set position */
void space(int n){
	std::string a;
	appendSame(a, ' ', n);
	printf("%s", a.c_str());
}

void backspace(int n){
	std::string a;
	appendSame(a, '\b', n);
	printf("%s", a.c_str());
}
/* end set position */

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