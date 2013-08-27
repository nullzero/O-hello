#pragma once

#include <string>
#include <vector>
#include <deque>
#include <sstream>

template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
class Deque : public std::deque<_Tp, _Alloc>{
public:
	using std::deque<_Tp, _Alloc> :: deque;
	_Tp read(){
		_Tp x = this->front();
		this->pop_front();
		return x;
	}
};

typedef Deque<int> vint;
typedef Deque<std::string> texture;

#define SIZEOF(_a) (sizeof(_a) / sizeof(_a[0]))

void appendSame(std::string &s, char c, int n){
	for(int i = 0; i < n; ++i){
		s += c;
	}
}

int rand(int a){
	return rand() % a;
}

int rand(int a, int b){
	return (rand() % (b - a + 1)) + a;
}

texture split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
	texture elems;
    while (std::getline(ss, item, delim)) elems.push_back(item);
    return elems;
}

texture simplify(texture t_old){
	texture t_new;
	for(auto x : t_old){
		if(x == "") continue;
		t_new.push_back(x);
	}
	return t_new;
}