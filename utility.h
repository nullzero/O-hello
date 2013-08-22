#pragma once

#include <string>

#define SIZEOF(_a) (sizeof(_a) / sizeof(_a[0]))

void appendSame(std::string &s, char c, int n){
	for(int i = 0; i < n; ++i){
		s += c;
	}
}