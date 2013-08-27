#pragma once

#include "mystdio.h"
#include <vector>
#include <string>
#include <map>

//constant parameters
const int weightChoice[][100]={
    {1,100,0,1000,0,0,0,0,55,1,0,0,1000,0,0,0,0,0,10000},
    {1,100,200,4000,1000,1000,0,0,55,1,100,50,250,50,50,0,0,0,50000},
    {-1,40,100,600,800,140,250,100,45,2,60,40,200,800,100,250,100,1,1000000}
};
const char weightChoiceName[][100] = {
     "1st generation",
     "2nd generation",
     "3rd generation (default)"
};
const int weightChoiceNum = SIZEOF(weightChoice); //number of weight choices

//move order
//priority:
// 1 8 2 3
// . 9 7 6
// . . 4 5
// . . . .
const int moveOrder[60] = {
    0,7,56,63,
    2,5,16,23,40,47,58,61,
    3,4,24,31,32,39,59,60,
    18,21,42,45,
    19,20,26,29,34,37,43,44,
    11,12,25,30,33,38,51,52,
    10,13,17,22,41,46,50,53,
    1,6,8,15,48,55,57,62,
    9,14,49,54
};

//weight's 'nickname'
int wd; //disk difference
int wm; //mobility difference
int wp; //potential mobility difference
int wc; //corner difference
int wxx; //x square difference
int wcc; //c square difference
int we; //edge configurations
int ws; //stable disks difference
//cutoff for using near end-game weights
int wcut;
//near end-game weights
int wd1;
int wm1;
int wp1;
int wc1;
int wxx1;
int wcc1;
int we1;
int ws1;
//use new csquare? 1=yes,0=no
int wnew;
//final disk difference (end-game)
int wf;

vint weightRaw;

//defaultation functions

//map weights to their nicknames
void nickname(){
	assert(weightRaw.size() == 19);
	wd = weightRaw[0];
	wm = weightRaw[1];
	wp = weightRaw[2];
	wc = weightRaw[3];
	wxx = weightRaw[4];
	wcc = weightRaw[5];
	we = weightRaw[6];
	ws = weightRaw[7];
	wcut = weightRaw[8];
	wd1 = weightRaw[9];
	wm1 = weightRaw[10];
	wp1 = weightRaw[11];
	wc1 = weightRaw[12];
	wxx1 = weightRaw[13];
	wcc1 = weightRaw[14];
	we1 = weightRaw[15];
	ws1 = weightRaw[16];
	wnew = weightRaw[17];
	wf = weightRaw[18];
}

//set weightRaws(and their nicknames) as weightSetting

//EXPERIMENTAL SECTION>>>>>>>>>>>>>>>>>>>>>>>>>>>>

char buffer[128];
int diskWidth, diskHeight, weightNum, middleHeight;

struct Property{
	enum {Int, Bool, Float, Vint, Texture};
	
	// Int => None
	// Bool => None
	// Float => None
	// Vint => num
	// Texture => row, column
	
	int type;
	
	int v_int;
	bool v_bool;
	float v_float;
	vint v_vint;
	texture v_texture;
	
	std::string desc;
	
	int num; // for vint
	int row, column; // for texture
	
	void set(int a){ v_int = a; }	
	void set(bool a){ v_bool = a; }
	void set(float a){ v_float = a; }
	void set(int* a){ v_vint = vint(a, a + num); }
	void set(std::string* a){ v_texture = texture(a, a + row); }
	void set(double a){ set(float(a)); } // for compatibility
	void set(std::string a){
		switch(type){
			case Int:
				v_int = std::stoi(a);
				break;
			case Bool:
				if(a == "on") v_bool = true;
				else if(a == "off") v_bool = false;
				else alert("bae bae");
				break;
			case Float:
				v_float = std::stof(a);
				break;
		}
	}
	
	bool scan(){
		std::string inp;
		switch(type){
			case Int:
				v_int = uget(int)();
				break;
			
			case Bool:
				inp = uget(sline)();
				if(inp == "on") v_bool = true;
				else if(inp == "off") v_bool = false;
				else alert("bae bae");
				break;
			
			case Float:
				v_float = uget(float)();
				break;
			
			case Vint:
				printf("fill %d numbers\n", num);
				for(int i = 0; i < num; ++i) v_vint[i] = uget(int)();
				break;
			
			case Texture:
				printf("row = %d, column = %d\n", row, column);
				texture inp;
				for(int i = 0; i < row; ++i){
					inp.push_back(uget(sline)());
					inp.back().pop_back();
					if(int(inp.back().size()) != column){
						alert("blah");
						return false;
					}
				}
				v_texture = inp;
		}
		return true;
	}
	
	void show(){
		switch(type){
			case Int:
				printf("%d", v_int);
				break;
			
			case Bool:
				printf("%s", v_bool ? "on" : "off");
				break;
			
			case Float:
				printf("%.2f", v_float);
				break;
			
			case Vint:
				for(int i = 0; i < int(v_vint.size()); ++i) printf("%d ", v_vint[i]);
				break;
			
			case Texture:
				for(int i = 0; i < int(v_texture.size()); ++i) printf("%s\n", v_texture[i].c_str());
				break;
		}
		printf("\n");
	}
	
	int& get_int(){ return v_int; }
	float& get_float(){ return v_float; }
	bool& get_bool(){ return v_bool; }
	vint& get_vint(){ return v_vint; }
	texture& get_texture(){ return v_texture; }
};

std::map<std::string,Property> setting, dSetting;

void initConfig(){
	setting["black"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("  X X  "), 
	 					 	 std::string("   X   "), 
	 					 	 std::string("  X X  ")};
 		setting["black"].column = tmp[0].size();
 		setting["black"].row = SIZEOF(tmp);
		setting["black"].set(tmp);
		setting["black"].desc = "olla";
	}
	
	setting["white"].type = Property::Texture;
	{
		std::string tmp[] = {std::string(" ooooo "), 
	 					 	 std::string(" ooooo "), 
	 					 	 std::string(" ooooo ")};
		setting["white"].column = tmp[0].size();
		setting["white"].row = SIZEOF(tmp);
		setting["white"].set(tmp);
		setting["white"].desc = "looa";
	}
	
	setting["rotate"].type = Property::Bool;
	setting["rotate"].set(true);
	
	setting["rotatetime"].type = Property::Int;
	setting["rotatetime"].set(50000);
	
	setting["flip"].type = Property::Bool;
	setting["flip"].set(true);
	
	setting["fliptime"].type = Property::Float;
	setting["fliptime"].set(0.3);
	
	setting["fliplook"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("   |   "), 
	 					 	 std::string("   |   "), 
	 					 	 std::string("   |   ")};
 		setting["fliplook"].column = tmp[0].size();
 		setting["fliplook"].row = SIZEOF(tmp);
		setting["fliplook"].set(tmp);
	}
	
	setting["move"].type = Property::Bool;
	setting["move"].set(true);
	
	setting["movelook"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("       "), 
	 					 	 std::string("   .   "), 
	 					 	 std::string("       ")};
		setting["movelook"].column = tmp[0].size();
		setting["movelook"].row = SIZEOF(tmp);
		setting["movelook"].set(tmp);
	}
	
	setting["rand"].type = Property::Bool;
	setting["rand"].set(true);
	
	setting["parallel"].type = Property::Bool;
	setting["parallel"].set(false);
	
	setting["raw"].type = Property::Bool;
	setting["raw"].set(false);
	
	setting["weight"].type = Property::Vint;
	{
		int vi[] = {-1, 40, 100, 600, 800, 140, 250, 100, 45, 2, 60, 40, 200, 800, 100, 250, 100, 1, 1000000};
		setting["weight"].num = SIZEOF(vi);
		setting["weight"].set(vi);
	}
	
	dSetting = setting;
	diskWidth = setting["black"].column;
	diskHeight = setting["black"].row;
	weightNum = setting["weight"].get_vint().size();
	middleHeight = (diskHeight - 1) / 2;
}

void weightInitialize(){
	weightRaw = setting["weight"].get_vint();
	nickname();
}