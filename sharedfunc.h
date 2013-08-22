#pragma once

#include <cstdlib>

void finish(int sig){
	system("stty echo");
    exit(0);
}