#pragma once

#include "mystdio.h"
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <functional>

//constant parameters
const int weightChoice[][19]={
    {1,100,0,1000,0,0,0,0,55,1,0,0,1000,0,0,0,0,0,10000},
    {1,100,200,4000,1000,1000,0,0,55,1,100,50,250,50,50,0,0,0,50000},
    {-1,40,100,600,800,140,250,100,45,2,60,40,200,800,100,250,100,1,1000000}
};
const char weightChoiceName[][100] = {
     "1st generation",
     "2nd generation",
     "3rd generation"
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

char buffer[128];
int diskWidth, diskHeight, weightNum, middleHeight;

struct Property{
	enum {Int, Bool, Float, Vint, Texture};
	
	// Int => None
	// Bool => None
	// Float => None
	// Vint => num
	// Texture => row, column
	
    std::function<bool(void)> func;
    
	int type;
	
	int v_int;
	bool v_bool;
	float v_float;
	vint v_vint;
	texture v_texture;
	
	std::string desc;
	
	int num; // for vint
	int row, column; // for texture
	
	bool set(int a){
        v_int = a;
        return true;
    }	
	bool set(bool a){
        v_bool = a;
        return true;
    }
	bool set(float a){
        v_float = a;
        return true;
    }
	bool set(int* a){
        v_vint = vint(a, a + num);
        return true;
    }
	bool set(std::string* a){
        v_texture = texture(a, a + row);
        return true;
    }
	bool set(double a){
        set(float(a));
        return true;
    } // for compatibility
	bool set(std::string a){
		switch(type){
			case Int:
				v_int = std::stoi(a);
				break;
			case Bool:
				if(a == "on") v_bool = true;
				else if(a == "off") v_bool = false;
				else{
                    alert("invalid. value must be on/off.");
                    return false;
                }
				break;
			case Float:
				v_float = std::stof(a);
				break;
            default:
                alert("invalid. can't set value directly.");
                return false;
		}
        return true;
	}
	
	bool scan(bool suppressFunc=false){
        if((not suppressFunc) and func) return func();
		std::string inp;
		switch(type){
			case Int:
                alert("input value (integer)");
				v_int = uget(int)();
				break;
			
			case Bool:
                alert("input value (on/off)");
                inp = uget(sline)([](std::string x){ return x == "on" or x == "off"; }, "invalid. input value again.");
				v_bool = (inp == "on");
				break;
			
			case Float:
                alert("input value (real number)");
				v_float = uget(float)();
				break;
			
			case Vint:
            {
				printf("\nenter %d integers\n\n", num);
                vint vint_tmp;
                while(int(vint_tmp.size()) < num){
                    auto vec = getl();
                    for(auto ele : vec) vint_tmp.push_back(std::stoi(ele));
                    if(int(vint_tmp.size()) > num){
                        printf("\ntoo many parameters. input again.\n");
                        return false;
                    }
                }
				v_vint = vint_tmp;
				break;
			}
			case Texture:
            {
                printf("\ninput value (row = %d, column = %d)\n\n", row, column);
				texture texture_tmp;
				for(int i = 0; i < row; ++i){
					texture_tmp.push_back(uget(sline)());
					if(int(texture_tmp.back().size()) != column){
						alert("column has invalid length!");
						return false;
					}
				}
				v_texture = texture_tmp;
                break;
            }
		}
		return true;
	}
	
	void show(){
		switch(type){
			case Int:
				printf("\nvalue = %d\n", v_int);
				break;
			
			case Bool:
				printf("\nvalue = %s\n", v_bool ? "on" : "off");
				break;
			
			case Float:
				printf("\nvalue = %.2f\n", v_float);
				break;
			
			case Vint:
                printf("\nvalue =");
				for(int i = 0; i < int(v_vint.size()); ++i) printf(" %d", v_vint[i]);
                printf("\n");
				break;
			
			case Texture:
                printf("\n");
				for(int i = 0; i < int(v_texture.size()); ++i) printf("%s\n", v_texture[i].c_str());
				break;
		}
		printf("\n");
	}
    
	void showCompact(){
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
				for(int i = 0; i < int(v_texture.size()); ++i) printf("\n%s", v_texture[i].c_str());
				break;
		}
	}
	
	int& get_int(){ return v_int; }
	float& get_float(){ return v_float; }
	bool& get_bool(){ return v_bool; }
	vint& get_vint(){ return v_vint; }
	texture& get_texture(){ return v_texture; }
};

bool customweight();

std::map<std::string,Property> setting, dSetting;

void weightInitialize(){
	weightRaw = setting["weight"].get_vint();
	nickname();
}

void initConfig(){
	setting["black"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("  X X  "), 
	 					 	 std::string("   X   "), 
	 					 	 std::string("  X X  ")};
 		setting["black"].column = tmp[0].size();
 		setting["black"].row = SIZEOF(tmp);
		setting["black"].set(tmp);
		setting["black"].desc = "black disk appearance";
	}
	
	setting["white"].type = Property::Texture;
	{
		std::string tmp[] = {std::string(" ooooo "), 
	 					 	 std::string(" ooooo "), 
	 					 	 std::string(" ooooo ")};
		setting["white"].column = tmp[0].size();
		setting["white"].row = SIZEOF(tmp);
		setting["white"].set(tmp);
		setting["white"].desc = "white disk appearance";
	}
	
	setting["rotate"].type = Property::Bool;
	setting["rotate"].set(true);
    setting["rotate"].desc = "display rotation effect when thinking";
	
	setting["rotatetime"].type = Property::Int;
	setting["rotatetime"].set(50000);
    setting["rotatetime"].desc = "number of nodes between each rotation";
	
	setting["flip"].type = Property::Bool;
	setting["flip"].set(true);
    setting["flip"].desc = "display disk flipping";
	
	setting["fliptime"].type = Property::Float;
	setting["fliptime"].set(0.3);
    setting["fliptime"].desc = "time (sec) between each frame of flipping";
	
	setting["fliplook"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("   |   "),
	 					 	 std::string("   |   "),
	 					 	 std::string("   |   ")};
 		setting["fliplook"].column = tmp[0].size();
 		setting["fliplook"].row = SIZEOF(tmp);
		setting["fliplook"].set(tmp);
        setting["fliplook"].desc = "disk flipping appearance";
	}
	
	setting["move"].type = Property::Bool;
	setting["move"].set(true);
    setting["move"].desc = "display legal moves";
	
	setting["movelook"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("       "), 
	 					 	 std::string("   .   "), 
	 					 	 std::string("       ")};
		setting["movelook"].column = tmp[0].size();
		setting["movelook"].row = SIZEOF(tmp);
		setting["movelook"].set(tmp);
        setting["movelook"].desc = "legal move appearance";
	}
	
	setting["rand"].type = Property::Bool;
	setting["rand"].set(true);
    setting["rand"].desc = "randomize between equally good moves";
	
	setting["parallel"].type = Property::Bool;
	setting["parallel"].set(false);
	setting["parallel"].desc = "allow parallel opening";
    
	setting["raw"].type = Property::Bool;
	setting["raw"].set(false);
    setting["raw"].desc = "use raw mode to configure O-hello levels";
	
	setting["weight"].type = Property::Vint;
	{
        setting["weight"].num = SIZEOF(weightChoice[2]);
		setting["weight"].set((int*)weightChoice[2]);
        setting["weight"].desc = "weights of evaluation function";
        setting["weight"].func = customweight;
	}
	
    weightInitialize();
	dSetting = setting;
	diskWidth = setting["black"].column;
	diskHeight = setting["black"].row;
	weightNum = setting["weight"].get_vint().size();
	middleHeight = (diskHeight - 1) / 2;
}

bool customweight(){
	alert("CHOOSE EVALUATION FUNCTION WEIGHTS");
	printf("0. customize weights\n\n");
	for(int i = 0; i < weightChoiceNum; i++){
		printf("%d. %s\n\n", i + 1, weightChoiceName[i]);
		printf("  ");
		for(int j = 0; j < int(setting["weight"].get_vint().size()); j++) printf(" %d", weightChoice[i][j]);
		printf("\n\n");
	}
	int input = uget(int)([](int x){ return x >= 0 and x <= weightChoiceNum; }, "invalid. choose again.");
	if(input != 0){
		setting["weight"].set((int*)weightChoice[input - 1]);
	}else{
		while(not setting["weight"].scan(true));
	}
    weightInitialize();
    return true;
}
