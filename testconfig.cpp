#include "mystdio.h"
#include <vector>
#include <string>
#include <map>

#define printf printw
#define SIZEOF(_a) (sizeof(_a) / sizeof(_a[0]))

typedef std::vector<int> vint;
typedef std::vector<std::string> texture;

char buffer[128];

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
	
	int num; // for vint
	int row, column; // for texture
	
	void set(int a){ v_int = a; }	
	void set(bool a){ v_bool = a; }
	void set(float a){ v_float = a; }
	void set(int* a){ v_vint = vint(a, a + num); }
	void set(std::string* a){ v_texture = texture(a, a + row); }
	void set(double a){ set(float(a)); }
	
	bool get(){
		std::string inp;
		switch(type){
			case Int:
				v_int = getInt();
				break;
			
			case Bool:
				inp = getString();
				if(inp == "on") v_bool = true;
				else if(inp == "off") v_bool = false;
				else alert("bae bae");
				break;
			
			case Float:
				v_float = getFloat();
				break;
			
			case Vint:
				printf("fill %d numbers\n", num);
				for(int i = 0; i < num; ++i) v_vint[i] = getInt();
				break;
			
			case Texture:
				printf("row = %d, column = %d\n", row, column);
				texture inp;
				for(int i = 0; i < row; ++i){
					inp.push_back(std::string(fgets(buffer, sizeof(buffer), stdin)));
					inp.back().resize(inp.back().size() - 1);
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
	}
};

std::map<std::string,Property> setting;

void init(){
	setting["black"].type = Property::Texture;
	{
		std::string tmp[] = {std::string(" x x "), 
	 					 	 std::string("  x  "), 
	 					 	 std::string(" x x ")};
 		setting["black"].column = tmp[0].size();
 		setting["black"].row = SIZEOF(tmp);
		setting["black"].set(tmp);
	}
	
	setting["white"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("ooooo"), 
	 					 	 std::string("ooooo"), 
	 					 	 std::string("ooooo")};
		setting["white"].column = tmp[0].size();
		setting["white"].row = SIZEOF(tmp);
		setting["white"].set(tmp);
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
		std::string tmp[] = {std::string("  |  "), 
	 					 	 std::string("  |  "), 
	 					 	 std::string("  |  ")};
 		setting["fliplook"].column = tmp[0].size();
 		setting["fliplook"].row = SIZEOF(tmp);
		setting["fliplook"].set(tmp);
	}
	
	setting["move"].type = Property::Bool;
	setting["move"].set(true);
	
	setting["movelook"].type = Property::Texture;
	{
		std::string tmp[] = {std::string("     "), 
	 					 	 std::string("  .  "), 
	 					 	 std::string("     ")};
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
}

int main(){
	init();
	setting["weight"].show();
	setting["weight"].get();
	setting["weight"].show();
	return 0;
}