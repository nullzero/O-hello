// project O-hello

//general info
const char __version__[] = "83";
const char __author__[] = "Nat Sothanaphan & Sorawee Porncharoenwase";
const char __date__[] = "August 30, 2013";
const char __language__[] = "C++";
const char __compiler__[] = "G++";

//version of save file
const int __saveNumber__ = 83;

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <sys/timeb.h> //required for 'ftime'

//(own header)
#include "edgeconfig2.h" //precomputed edge configurations
#include "mobtable.h" //precomputed mobility table

#include <stack>
#include <string>
#include <cassert>
#include <map>
#include <signal.h>
#include <unistd.h>
#include <boost/format.hpp>

//(own header)
#include "mystdio.h"
#include "precompute.h"
#include "config.h"
#include "sharedfunc.h"
#include "help.h"

#define CORRUPTION(x) {printf("\n\n%s is corrupted\n\n", filename);\
        fclose(save);\
		return false;}

#define INCOMPATIBLE(x) {printf("\n%s has incompatible save number\n\n", filename);\
        fclose(save);\
        return false;}

/*
        saveNumber
        [black]
        [white]
        rotate
        rotatetime
        flip
        fliptime
        [fliplook]
        move
        [movelook]
        rand
        parallel
        raw
        weight
        
        * texture doesn't work
*/
/*
        texture prototype
        for(int i = 0; i < diskHeight; i++) for(int j = 0; j < diskWidth; j++) fprintf(save, " %d", setting["black"].get_texture()[i][j]);\
*/
#define PRINTSETTING(x) {\
        fprintf(save, "%d", __saveNumber__);\
        fprintf(save, " %d", setting["rotate"].get_bool());\
        fprintf(save, " %d", setting["rotatetime"].get_int());\
        fprintf(save, " %d", setting["flip"].get_bool());\
        fprintf(save, " %.2f", setting["fliptime"].get_float());\
        fprintf(save, " %d", setting["move"].get_bool());\
        fprintf(save, " %d", setting["rand"].get_bool());\
        fprintf(save, " %d", setting["parallel"].get_bool());\
        fprintf(save, " %d", setting["raw"].get_bool());\
        for(int i = 0; i < weightNum; i++) fprintf(save, " %d", setting["weight"].get_vint()[i]);\
}

const int EXIT = 555;
const int MAXN = 8;
enum COLOR {BC, XC, OC};
enum INPUT_MODE {MENUMODE, PLAYMODE};
const int UNINIT = -1;
const int LARGE = 2147483647;

char UNIVERSALSTRING[100];

void setUstring(const char* name){
    strcpy(UNIVERSALSTRING, name);
}

void init(){
	srand(time(NULL));
	usleep(100000);
	system("stty erase ^?");
	signal(SIGINT, finish);
	
	initConfig();
}

void print_bottom(int player, std::string prefix){
	std::string splayer = (player == 1) ? "black" : "white";
	for(int i = 0; i < diskHeight; ++i){
		if(i == middleHeight){
			printf("%s: %s [%s]\n", prefix.c_str(), setting[splayer].get_texture()[i].c_str(), splayer.c_str());
		}else{
			space(prefix.size());
			printf("  %s\n", setting[splayer].get_texture()[i].c_str());
		}
	}
}

struct undoStruct{
	int lastMove, player;
	int board[64], no[2];
	
	undoStruct(int* _board, int* _no, int _player, int _lastMove){
		for(int i = 0; i < 64; ++i) board[i] = _board[i];
		no[0] = _no[0];
		no[1] = _no[1];
		player = _player;
		lastMove = _lastMove;
	}
};
//End of Oak's definitions

int depthShallow(int depth, bool endgame){
	int depthAdjust = depth;
	if(endgame) depthAdjust -= 6;
	if(depthAdjust >= 5) return depthAdjust - 4;
	else if(depthAdjust == 4 or depthAdjust == 3) return 1;
	else return 0;
}

FILE *save; //save file

//used in function 'flip' and 'move'
//to return board + a number
struct kirby{
	int board[64];
	int num;
};

//used in function 'gamefortest'
//to return WLD values
struct triad{
	int win, lose, draw;
};

//used in function 'settings'
//to return AI settings
struct comset{
	int mode, depth, depthperfect;
	float times;
};

//index array for mobility table
int mobindex[38];
int node; //count the number of nodes searched

//functions in this source code are arranged in this order
int main();
comset comsettings();
bool load(const char*);
void about();
void speedtest();
void sayhello();
int human(int board[64],int no[2],int player);
void humansave(int board[64],int player);
int comhuman(int board[64],int no[2],int player,int complayer,int mode,int depth,int depthperfect,float times);
void comhumansave(int board[64],int player,int complayer,int mode,int depth,int depthperfect,float times);
int comcom(int board[64],int no[2],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]);
void comcomsave(int board[64],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]);
void weightfortest(int doubleweight[2][100],int player);
void weighttest();
triad gamefortest(int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2],int doubleweight[2][100],int numgame,int numrand);
int input(int board[64],int& player,int no[2], std::vector<undoStruct>&, int&, int&);
void shownode(int node);
int random(int board[64],int player,int no[2],int display);
int fsearch(int board[64],int depthwant,int player,int no[2],int display);
int tsearch(int board[64],float times,int player,int no[2],int display);
int score(int board[64],int depthleft,int player,int no[2],int alphaGet,int betaGet,int display);
int score63(int board[64],int player,int no[2],int display);
void nodedisplay(int display);
void boarddisplay(int board[64],int playertoshowmove);
void display(int board[64],int player,int no[2],int lastmove);
void flipanimation(int board[64],int player,int position,int flipboard[64]);
void indexformob(int board[64]);
int mobility(int player);
int pmobility(int board[64]);
int triplesq(int board[64],int corner,int xsquare,int csquare);
float edgevalue(int board[64],int player);
int stabledisk(int board[64]);
kirby move(int board[64],int player);
kirby flip(int board[64],int position,int player);
int flipnum(int board[64],int position,int player);

texture newinput(int mode){
	texture inp;
	inp = getl();
	
	auto x = inp.read();
	if(x == "quit"){
        if(not inp.empty()){
            alert("too many parameters");
            return texture();
        }
        else finish(0);
    }
	if(x == "help"){
		if(not commandHelp(inp)) alert("invalid help entry");
		return texture();
	}
	if(x == "reset"){
		if(inp.empty()){
            alert("input variable or 'all'");
            inp = getl();
        }
		x = inp.read();
        if(not inp.empty()){
            alert("too many parameters");
        }else if(x == "all"){
            setting = dSetting;
            weightInitialize();
            alert("all variables reset");
        }else if(setting.find(x) == setting.end()){
			alert("invalid variable");
		}else{
    		setting[x] = dSetting[x];
            weightInitialize();
            printf("\n'%s' reset\n\n", x.c_str());
        }
		return texture();
	}
	if(x == "set"){
		if(inp.empty()){
			alert("input variable");
			inp = getl();
		}
		x = inp.read();
		if(setting.find(x) == setting.end()){
			alert("invalid variable");
			return texture();
		}
		texture ret;
		if(x == "move"){
			ret.push_back("~");
			ret.push_back("move");
		}
		if(inp.empty()){
			if(not setting[x].scan()) return texture();
		}else{
			auto y = inp.read();
			if(inp.empty()){
				if(not setting[x].set(y)) return texture();
			}else{
                if(setting[x].type == Property::Vint or setting[x].type == Property::Texture)
                    alert("invalid. can't set value directly.");
                else
				    alert("too many parameters");
				return texture();
			}
		}
		alert((boost::format("'%1%' set!") % x.c_str()).str());
		return ret;
	}else if(x == "show"){
		if(inp.empty()){
			alert("input variable or 'all'");
			inp = getl();
		}
		x = inp.read();
        if(not inp.empty()){
            alert("too many parameters");
        }else if(x == "all"){
            for(auto var : setting){
                printf("\nvalue %s = ", var.first.c_str());
                setting[var.first].showCompact();
            }
            printf("\n\n");
        }else if(setting.find(x) == setting.end()){
			alert("invalid variable");
		}else{
    		setting[x].show();
        }
		return texture();
	}
	inp.push_front(x);
	return inp;
}

//interface
void newgame(){
    int mode;
    int depth;
    int depthperfect;
    float times;
    int doublemode[2];
    int doubledepth[2];
    int doubledepthperfect[2];
    float doubletimes[2];
    int sentboard[64];
    int sentno[2];
    int sentdoublemode[2];
    int sentdoubledepth[2];
    int sentdoubledepthperfect[2];
    float sentdoubletimes[2];
    comset pack;
	
    clrscr(); //clear screen
    printf("\n version %s\n", __version__);
	printf("                                  -----------\n");
    printf("                                 |           |\n");
    printf("                                 |  O-hello  |\n");
	printf("                                 |           |\n");
    printf("                                  -----------\n\n\n");
    printf("welcome! please select game mode or function below:\n\n");
    printf("0. help\n");
    printf("1. human   [black] vs human   [white]\n");
    printf("2. human   [black] vs O-hello [white]\n"); //human starts first
    printf("3. O-hello [black] vs human   [white]\n"); //computer starts first
    printf("4. O-hello [black] vs O-hello [white]\n");
    printf("5. weight test\n");
    printf("6. about program\n\n");
	
    //set board
    //board: 0 = space, 1 = player_1, 2 = player_2
    int board[MAXN * MAXN] = {};
    board[27] = OC;
    board[28] = XC;
    board[35] = XC;
    board[36] = OC;
    int no[2] = {2, 2}; //set number of each player's disks
    int player = XC; //set current player
	
	while(true){
		texture vec = newinput(MENUMODE);
		
		if(vec.empty() or vec.front() == "~") continue;
		auto inp = vec.read();
		
		if(inp == "hello"){
            if(not vec.empty()){
                alert("too many parameters");
            }
			else sayhello();
			continue;
		}
		else if(inp == "speed"){
            if(not vec.empty()){
                alert("too many parameters");
            }
			else speedtest();
			continue;
		}
		else if(inp == "load"){
            if(vec.empty()){
                alert("input file name (no extension)");
                vec = getl();
            }
            inp = vec.read();
            if(not vec.empty()){
                alert("too many parameters");
                continue;
            }else{
			    if(not load(inp.c_str())) continue;
            }
		}
		else if(inp == "menu"){
		    if(not vec.empty()){
		        alert("too many parameters");
                continue;
		    }
		}
	    else if(inp.size() != 1){
			alert("invalid command");
			continue;
		}
		else if(inp == "0"){
			commandHelp();
            continue;
		}else if(inp == "1"){
			//human vs human
			do{
				for(int i = 0; i < 64; i++) sentboard[i] = board[i];
				for(int i = 0; i < 2; i++) sentno[i] = no[i];
			}while(human(sentboard, sentno, player) == EXIT);
		}else if(inp == "2" or inp == "3"){
			 //human vs computer,computer vs human
			clrscr();
			printf("\nselect search mode for O-hello:\n");
			printf("-------------------------------\n\n");
			//get settings
			pack = comsettings();
			mode = pack.mode;
			depth = pack.depth;
			depthperfect = pack.depthperfect;
			times = pack.times;
			int numvalue;
			if(inp[0] == '2') numvalue = OC;
			else numvalue = XC;
			do{
				for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			 	for(int i = 0; i < 2; i++) sentno[i] = no[i];
			}while(comhuman(sentboard, sentno, player, numvalue, mode, depth, depthperfect, times) == EXIT);
	    }else if(inp == "4"){
			//computer vs computer
			for(int i = 0; i < 2; i++){
				printf("\nselect search mode for O-hello %d:\n", i + 1);
				printf("---------------------------------\n\n");
				//get settings
	            pack = comsettings();
	            doublemode[i] = pack.mode;
	            doubledepth[i] = pack.depth;
	            doubledepthperfect[i] = pack.depthperfect;
	            doubletimes[i] = pack.times;
	            printf("\n");
			}
	        do{
				for(int i = 0; i < 64; i++) sentboard[i] = board[i];
				for(int i = 0; i < 2; i++) sentno[i] = no[i];
	            for(int i = 0; i < 2; i++) sentdoublemode[i] = doublemode[i];
	            for(int i = 0; i < 2; i++) sentdoubledepth[i] = doubledepth[i];
	            for(int i = 0; i < 2; i++) sentdoubledepthperfect[i] = doubledepthperfect[i];
	            for(int i = 0; i < 2; i++) sentdoubletimes[i] = doubletimes[i];
			}while(comcom(sentboard, sentno, player, sentdoublemode, sentdoubledepth, sentdoubledepthperfect, sentdoubletimes) == EXIT);
		}else if(inp == "5"){
			weighttest();
		}else if(inp == "6"){
			about();
		}else{
			alert("invalid mode");
			continue;
		}
		return;
	}
}

int main(){
	init();
	while(true) newgame();
}

//AI settings interface
comset comsettings(){
	comset pack;
	if(setting["raw"].get_bool()){
		// old interface
		printf("0. random : random play\n");
		printf("1. fsearch: fixed depth search\n");
		printf("2. tsearch: time limit search\n\n");
		
		pack.mode = uget(int)([](int x){ return 0 <= x and x <= 2; }, "invalid: mode must be between 0 and 2");
		
		setdetail:
		if(pack.mode == 1){
			printf("\ninput search depth and depthperfect: ");
			scanf("%d", &pack.depth);
			if(pack.depth < 1){
				printf("\ninvalid depth!\n");
				goto setdetail;
			}
			scanf("%d", &pack.depthperfect);
			if(pack.depthperfect < 1){
				printf("\ninvalid depthperfect!\n");
				goto setdetail;
			}
		}else if(pack.mode == 2){
		    printf("\nTHIS MODE IS UNDER DEVELOPMENT -- USE WITH CARE");
		    printf("\n\ninput time limit (sec): ");
		    scanf("%f", &pack.times);
		    if(pack.times <= 0){
				printf("\ninvalid time\n");
				goto setdetail;
			}
		}
	}else{
		// new interface
		printf("1. search with fixed depth\n");
		printf("2. search with time limit\n\n");
		
		newsetmode:
        pack.mode = uget(int)();
		if(pack.mode < 0 or pack.mode > 2){
			alert("invalid mode!"); 
			goto newsetmode;
		}

		if(pack.mode == 1){
			printf("\n");
			printf("select search depth:\n");
			printf("--------------------\n");
			printf(" 1  - depth 1\n");
			printf(" 2  - depth 2\n");
			printf(" 3  - depth 3\n");
			printf(" 4  - depth 4\n");
			printf(" 4a - depth 4  + last 10 perfect\n");
			printf(" 6  - depth 6\n");
			printf(" 6a - depth 6  + last 12 perfect\n");
			printf(" 8  - depth 8  + last 14 perfect\n");
			printf(" 10 - depth 10 + last 16 perfect\n\n");
			
			newsetdepth:
			
			std::string option = uget(sline)();
			if(option == "1"){
				pack.depth = 1;
				pack.depthperfect = 1;
			}else if(option == "2"){
				pack.depth = 2;
				pack.depthperfect = 2;
			}else if(option == "3"){
				pack.depth = 3;
				pack.depthperfect = 3;
			}else if(option == "4"){
				pack.depth = 4;
				pack.depthperfect = 4;
			}else if(option == "4a"){
				pack.depth = 4;
				pack.depthperfect = 10;
			}else if(option == "6"){
				pack.depth = 6;
				pack.depthperfect = 6;
			}else if(option == "6a"){
				pack.depth = 6;
				pack.depthperfect = 12;
			}else if(option == "8"){
				pack.depth = 8;
				pack.depthperfect = 14;
			}else if(option == "10"){
				pack.depth = 10;
				pack.depthperfect = 16;
			}else{
				alert("invalid selection");
				goto newsetdepth;
			}
		}else{
		    printf("\nTHIS MODE IS UNDER DEVELOPMENT -- USE WITH CARE\n\n");
		    pack.times = uget(float)([](int x){ return x > 0; }, "invalid: time must be more than 0", "input time limit (sec)");
		}
	}
	return pack;
}

//load engine
//return false if file is corrupted
//return true if load successfully
bool load(const char* filename2){
    char filename[100] = {};
    strcpy(filename, filename2);
    int board[64];
    int no[2];
    int player;
    int mode;
    int depth;
    int depthperfect;
    float times;
    int doublemode[2];
    int doubledepth[2];
    int doubledepthperfect[2];
    float doubletimes[2];
    //the sents: for sending to game engine
    //so that the original arrays are kept
    int sentboard[64];
    int sentno[2];
    int sentdoublemode[2];
    int sentdoubledepth[2];
    int sentdoubledepthperfect[2];
    float sentdoubletimes[2];
    
    save = fopen(strcat(filename, ".ohl"), "r");
    if(save == NULL){
		alert((boost::format("%1% can't be opened or doesn't exist") % filename).str()); 
		return false;
	}
    printf("\nopening %s\n", filename);
    
    //GET SETTINGS>>>>>>
    
    int tmp;
    float tmpFloat;
    /*
    texture prototype
    char tmpTexture[diskHeight][diskWidth];
    */
    
    //get saveNumber
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != __saveNumber__) INCOMPATIBLE();
    printf("\nvalue saveNumber = %d", __saveNumber__);
    
    /*
    texture prototype (DOESN'T WORK)
    //get black
    printf("\nvalue black =");
    for(int i = 0; i < diskHeight; i++){
        for(int j = 0; j < diskWidth; j++){
            fscanf(save, "%d", &tmpTexture[i][j]);
            printf(" %d", tmpTexture[i][j]);    
        }

    }
    setting["black"].set(tmpTexture);
    */
    
    //get rotate
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != 0 and tmp != 1) CORRUPTION();
    setting["rotate"].set((bool)tmp);
    printf("\nvalue rotate = %d", setting["rotate"].get_bool());

    //get rotatetime
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp <= 0) CORRUPTION();
    setting["rotatetime"].set(tmp);
    printf("\nvalue rotatetime = %d", setting["rotatetime"].get_int());
    
    //get flip
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != 0 and tmp != 1) CORRUPTION();
    setting["flip"].set((bool)tmp);
    printf("\nvalue flip = %d", setting["flip"].get_bool());
    
    //get fliptime
    tmpFloat = -1;
    fscanf(save, "%f", &tmpFloat);
    if(tmpFloat <= 0) CORRUPTION();
    setting["fliptime"].set(tmpFloat);
    printf("\nvalue fliptime = %.2f", setting["fliptime"].get_float());
    
    //get move
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != 0 and tmp != 1) CORRUPTION();
    setting["move"].set((bool)tmp);
    printf("\nvalue move = %d", setting["move"].get_bool());
    
    //get rand
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != 0 and tmp != 1) CORRUPTION();
    setting["rand"].set((bool)tmp);
    printf("\nvalue rand = %d", setting["rand"].get_bool());
    
    //get parallel
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != 0 and tmp != 1) CORRUPTION();
    setting["parallel"].set((bool)tmp);
    printf("\nvalue parallel = %d", setting["parallel"].get_bool());
    
    //get raw
    tmp = -1;
    fscanf(save, "%d", &tmp);
    if(tmp != 0 and tmp != 1) CORRUPTION();
    setting["raw"].set((bool)tmp);
    printf("\nvalue raw = %d", setting["raw"].get_bool());
    
    //get weight
    int tmpWeight[weightNum];
    printf("\nvalue weight =");
    for(int i = 0; i < weightNum; i++){
        fscanf(save, "%d", &tmpWeight[i]);
        printf(" %d", tmpWeight[i]);
    }
    setting["weight"].set(tmpWeight);
    weightInitialize();
    
    //>>>>>>>>>>>>>>>>>>
    
    //get board[64] (along with no[2])
    printf("\nvalue board = ");
    no[0] = 0;
    no[1] = 0;
    for(int i = 0; i < 64; ++i){
		board[i] = -1;
		fscanf(save, "%d", &board[i]);
		if(board[i] == 0){
			//the 4 centers can't be empty -- this will affect the searches
			//since they aren't included in moveOrder[60]
			if(i == 27 or i == 28 or i == 35 or i == 36){
                CORRUPTION();
            }else{
                printf("0");
            }
		}else if(board[i] == 1 or board[i] == 2){
			printf("%d", board[i]);
			no[board[i] - 1]++;
		}else{
			CORRUPTION();
		}
	}
    //get player
    player = -1;
    fscanf(save, "%d", &player);
    if(player != 1 and player != 2) CORRUPTION();
    printf("\nvalue player = %d", player);
    //get loadmode
    int loadmode = -1;
    fscanf(save, "%d", &loadmode);
    if(loadmode == 1){
		//human vs human
		printf("\nvalue loadmode = 1");
        printf("\n\n");
		presstogo();
		do{
			for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			for(int i = 0; i < 2; i++) sentno[i] = no[i];
		}while(human(sentboard,sentno,player) == EXIT);
		return true;
	}else if(loadmode == 2 or loadmode == 3){
		//computer vs human, human vs computer
		printf("\nvalue loadmode = %d", loadmode);
		//get mode
		mode = -1;
		fscanf(save, "%d", &mode);
		if(mode < 0 or mode > 2) CORRUPTION();
		printf("\nvalue mode = %d", mode);
		//get depth or times
		depth = -1;
		depthperfect = -1;
		times = -1;
		if(mode == 1){
			fscanf(save, "%d", &depth);
			if(depth < 1) CORRUPTION();
			printf("\nvalue depth = %d", depth);
			fscanf(save, "%d", &depthperfect);
			if(depthperfect < 1) CORRUPTION();
			printf("\nvalue depthperfect = %d", depthperfect);
		}else if(mode == 2){
			fscanf(save, "%f", &times);
			if(times < 0) CORRUPTION();
			printf("\nvalue times = %f", times);
		}
        printf("\n\n");
		presstogo();
		do{
			for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			for(int i = 0; i < 2; i++) sentno[i] = no[i];
		}while(comhuman(sentboard, sentno, player, (loadmode == 2) ? 2 : 1, mode, depth, depthperfect, times) == EXIT);
		return true;
	}else if(loadmode == 4){
		//computer vs computer
		printf("\nvalue loadmode = 4");
		for(int i = 0; i <= 1; i++){
			//get doublemode
			doublemode[i] = -1;
			fscanf(save, "%d", &doublemode[i]);
			if(doublemode[i] < 0 or doublemode[i] > 2) CORRUPTION();
			printf("\nvalue doublemode[%d] = %d", i, doublemode[i]);
			//get doubledepth or doubletimes
			doubledepth[i] = -1;
			doubledepthperfect[i] = -1;
			doubletimes[i] = -1;
			if(doublemode[i] == 1){
				fscanf(save, "%d", &doubledepth[i]);
				if(doubledepth[i] < 1) CORRUPTION();
				printf("\nvalue doubledepth[%d] = %d", i, doubledepth[i]);
				fscanf(save, "%d", &doubledepthperfect[i]);
				if(doubledepthperfect[i] < 1) CORRUPTION();
				printf("\nvalue doubledepthperfect[%d] = %d", i, doubledepthperfect[i]);
			}
			if(doublemode[i] == 2){
				fscanf(save, "%f", &doubletimes[i]);
				if(doubletimes[i] < 0) CORRUPTION();
				printf("\nvalue doubletimes[%d] = %f", i, doubletimes[i]);
			}
		}
        printf("\n\n");
		presstogo();
		do{
			for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			for(int i = 0; i < 2; i++) sentno[i] = no[i];
			for(int i = 0; i < 2; i++) sentdoublemode[i] = doublemode[i];
			for(int i = 0; i < 2; i++) sentdoubledepth[i] = doubledepth[i];
			for(int i = 0; i < 2; i++) sentdoubledepthperfect[i] = doubledepthperfect[i];
			for(int i = 0; i < 2; i++) sentdoubletimes[i] = doubletimes[i];
		}while(comcom(sentboard, sentno, player, sentdoublemode, sentdoubledepth, sentdoubledepthperfect, sentdoubletimes) == EXIT);
		return true;
	}else{
        CORRUPTION();
	}
}

//about program
void about(){
	clrscr();
	printf(R"(
About Program
-------------

project O-hello

version  %s
author   %s
date     %s
language %s
compiler %s

O-hello is a command-line program that mainly features an AI that plays othello.
It is only an elementary level program with low search speed and weak evaluation
function. However we hope it would be a good practice for all players aiming to
improve their skills at othello.

Note: there are several commands available in O-hello. You can type 'help' to
view the list of commands.


press 'h' to view information on how to play othello
press any other key to go back to the main menu
)", __version__, __author__, __date__, __language__, __compiler__);

	if(getch() == 'h'){
		clrscr();
		printf(R"(
How to Play Othello
-------------------

Othello is played on an 8x8 board using disks coloured black on one side and
white on the other side. The game starts with 2 black disks on d5 and e4, and 2
white disks on d4 and e5. The two players take turns to place a disk of his own
colour on the board, with black going first.

A move consists of placing a new disk on an empty square, and flipping every
opponent's disk or row of opponent's disks that is between the new disk and one
of the player's disks in any direction: horizontal, vertical or diagonal.

A move is legal if it flips at least one opponent's disk. A player with no legal
moves must pass his turn. On the contrary, a player with a legal move cannot
pass his turn.

The game ends when no one has a legal move. The winner is the player with a
larger number of disks, with a draw possible if both numbers are the same. The
remaining empty squares, if any, are usually awarded to the winner.


press any key to go back to the main menu
)");
		getch();
	}
	return;
}

//test the speed of this computer
void speedtest(){
	timeb time1, time2;
	int i = 0;
	printf("\nrunning test for 1 second");
	ftime(&time1);
	do{
		i++;
		ftime(&time2);
	}while(time2.time - time1.time + 0.001 * (time2.millitm - time1.millitm) < 1);
	printf("\n\n%d kiloloops were executed\n\n",i / 1000);
}

//say hello :)
void sayhello(){
	const char* greeting[] = {	"Hello, player.",
								"Hi, player.",
								"Greetings, player.",
								"Hello there, player.",
								"Hi there, player.",
								"Oh, hello player.",
								"Oh, hi player.",
								"Oh, hello -- that's my name!",
								"Hello player. Nice to meet you.",
								"Hello player. Pleased to meet you."
							};
	printf("\n%s\n\n", greeting[rand(sizeof(greeting) / sizeof(greeting[0]))]);
}

//human vs human
int human(int board[64],int no[2],int player){
    std::vector<undoStruct> undoData;
    int undoItr = -1;
    int i,j,k; //for reflect
    int position;
    int mobilities; //value from function 'mobility'
    kirby flips; //value from function 'flip'
    char letter; //for one-letter command
    int lastmove=-1;
    
    undoData.push_back(undoStruct(board, no, player, lastmove));
    undoItr++;

    loop2: //start here!
    clrscr(); //clear screen
    //display board
    display(board,player,no,lastmove);
    //diplay player
	
	print_bottom(player, "you are"); // TODO

    //compute mobility
    indexformob(board);
    mobilities=mobility(player);
    if(mobilities!=0){
                      //if player is movable
                      printf("\nplayer %d's turn: input position or command\n\n",player);
                      loop1:
                      position=input(board,player,no, undoData, undoItr, lastmove); //get position
                      //-999 means save
                      if(position==-999){humansave(board,player); goto loop2;}
                      //-1 means undo
                      if(position==-1){
	                       goto loop2;
                      }
                      //-2 means reflect
                      if(position==-2){
                                       for(i=0;i<8;i++){ //row i
                                       for(j=0;j<4;j++){ //column j
                                                        k=board[8*i+j];
                                                        board[8*i+j]=board[8*i+7-j];
                                                        board[8*i+7-j]=k;
                                                        }
                                                        }
                                       //last move
                                       if(lastmove!=-1) lastmove+=7-2*(lastmove%8);
                                       goto loop2;
                                       }
                      if(position==-3) return EXIT; //-3 means new
                      if(position==-4) return 0; //-4 means menu
                      if(position==-5) goto loop2; //-5 means move on-off
                      flips=flip(board,position,player); //flip!
                      //if nothing is flipped
                      if(flips.num==0){
                                       printf("\nillegal move!\n\n");
                                       goto loop1;
                                       }
                      //show flip animation
                      if(setting["flip"].get_bool()) flipanimation(board,player,position,flips.board);
                      lastmove = position;
                      //update old values
                      //get board from function 'flip'
                      for(i=0;i<64;i++) board[i]=flips.board[i];
                      //update no[2]
                      no[player-1]+=flips.num+1;
                      no[2-player]-=flips.num;
                      player=3-player; //switch player
                      while((int)undoData.size() - 1 > undoItr) undoData.pop_back();
                      undoData.push_back(undoStruct(board, no, player, lastmove));
                      undoItr++;
                      goto loop2;
                      }
    //if player is not movable
    else if(mobility(3-player)==0){ //if opponent is not movable too
         printf("\nno more moves can be made! game over!\n\n");
         //display result :)
         if(no[0]>no[1]){
                         printf("-------------------\n");
                         printf("PLAYER 1 WINS %d-%d\n",64-no[1],no[1]);
                         printf("-------------------\n");
                         }
         else if(no[0]<no[1]){
              printf("-------------------\n");
              printf("PLAYER 2 WINS %d-%d\n",64-no[0],no[0]);
              printf("-------------------\n");
              }
         else{
              printf("-----------------\n");
              printf("IT'S A DRAW 32-32\n");
              printf("-----------------\n");
              }
         printf("\n's' to save  'n' for new game  'm' for menu  'q' to quit");
         do{
            letter=getch();
            if(letter=='s'){printf("\n"); humansave(board,player); goto loop2;}
            if(letter=='n') return EXIT;
            if(letter=='m') return 0;
            if(letter=='q') finish(0);
            }while(1);
         }
    else{ //pass turn :)
         printf("\nplayer %d's turn: no moves left",player);
         printf("\n\npress any key to pass turn");
         getch();
         printf("\n");
         player=3-player; //switch player
         goto loop2;
         }
}

//save engine of human vs human
void humansave(int board[64],int player){
     char filename[100];
     strcpy(filename, UNIVERSALSTRING);
     save=fopen(strcat(filename, ".ohl"), "w");
     //print settings
     PRINTSETTING();
     //print board
     for(int i = 0; i < 64; i++) fprintf(save," %d",board[i]);
     //print player
     fprintf(save," %d",player);
     //print loadmode
     fprintf(save," 1");
     fclose(save);
     printf("\ngame saved in %s",filename);
     getch();
}

//human vs computer,computer vs human
//complayer=player that computer plays as
int comhuman(int board[64],int no[2],int player,int complayer,int mode,int depth,int depthperfect,float times){
    std::vector<undoStruct> undoData;
    int undoItr = -1;
    int i,j,k; //for reflect
    int position = UNINIT;
    int mobilities; //value from function 'mobility'
    kirby flips; //value from function 'flip'
    char letter; //for one-letter command
    int lastmove = -1;
    
    if(complayer == 2){
        undoData.push_back(undoStruct(board, no, player, lastmove));
        undoItr++;
    }
    
    loop2: //start here!
    clrscr(); //clear screen
    //display board
    display(board,player,no,lastmove);
    //diplay player
	print_bottom(player, (player == complayer) ? "O-hello is" : "you are");
    //compute mobility
    indexformob(board);
    mobilities=mobility(player);
    if(mobilities!=0){
                      //if player is movable
                      if(player == complayer){
                                            //for computer's turn
                                            printf("\nthinking");
											space(10);
                                            //get position
                                            if(mode==0) position=random(board,player,no,1);
                                            if(mode==1){
                                                        //for near-end evaluation, use depthperfect
                                                        if(no[0]+no[1]>=64-depthperfect) position=fsearch(board,depthperfect,player,no,1);
                                                        else position=fsearch(board,depth,player,no,1);
                                                        }
                                            if(mode==2) position=tsearch(board,times,player,no,1);
                                            printf("\n\n's' to save  'n' for new game  'm' for menu  'q' to quit");
                                            printf("\npress any other key to continue");
                                            letter=getch();
                                            if(letter=='s'){printf("\n"); comhumansave(board,player,complayer,mode,depth,depthperfect,times); goto loop2;}
                                            if(letter=='n') return EXIT;
                                            if(letter=='m') return 0;
                                            if(letter=='q') finish(0);
                                            lastmove=position;
                                            flips=flip(board,position,player); //flip!
                                            //show flip animation
                                            if(setting["flip"].get_bool()) flipanimation(board,player,position,flips.board);
                                            //get board from function 'flip'
                                            for(i=0;i<64;i++) board[i]=flips.board[i];
                                            //update no[2]
                                            no[player-1]+=flips.num+1;
                                            no[2-player]-=flips.num;
                                            player = 3-player;
                                            //UNDO UPDATE
                                            while((int)undoData.size() - 1 > undoItr) undoData.pop_back();
                                            undoData.push_back(undoStruct(board, no, player, lastmove));
                                            undoItr++;
                                            }
                      else{
                           //for human's turn
                           printf("\nyour turn: input position or command\n\n");
                           loop1:
                           position=input(board,player,no, undoData, undoItr, lastmove); //get position
                           //-999 means save
                           if(position==-999){comhumansave(board,player,complayer,mode,depth,depthperfect,times); goto loop2;}
                           //-1 means undo
                           if(position==-1){
							   goto loop2;
                                            }
                           //-2 means reflect
                           if(position==-2){
                                            for(i=0;i<8;i++){ //row i
                                            for(j=0;j<4;j++){ //column j
                                                             k=board[8*i+j];
                                                             board[8*i+j]=board[8*i+7-j];
                                                             board[8*i+7-j]=k;
                                                             }
                                                             }
                                            //last move
                                            if(lastmove!=-1) lastmove+=7-2*(lastmove%8);
                                            goto loop2;
                                            }
                           if(position==-3) return EXIT; //-3 means new
                           if(position==-4) return 0; //-4 means menu
                           if(position==-5) goto loop2; //-5 means move on-off
                           flips=flip(board,position,player); //flip!
                           //if nothing is flipped
                           if(flips.num==0){
                                            alert("illegal move!");
                                            goto loop1;
                                            }
                           //show flip animation
                           if(setting["flip"].get_bool()) flipanimation(board,player,position,flips.board);
                           lastmove = position;
                           
                           //update old values
                           //get board from function 'flip'
                           for(i=0;i<64;i++) board[i]=flips.board[i];
                           //update no[2]
                           no[player-1]+=flips.num+1;
                           no[2-player]-=flips.num;
                           player = 3-player;
                           }
                      goto loop2;
                      }
    //if player is not movable
    else if(mobility(3-player)==0){ //if opponent is not movable too
         printf("\n\nno more moves can be made! game over!\n\n");
         //display result :)
         if(no[complayer-1]>no[2-complayer]){
                                             printf("------------------\n");
                                             printf("O'HELLO WINS %d-%d\n",64-no[2-complayer],no[2-complayer]);
                                             printf("------------------\n");
                                             }
         else if(no[complayer-1]<no[2-complayer]){
              printf("-------------\n");
              printf("YOU WIN %d-%d\n",64-no[complayer-1],no[complayer-1]);
              printf("-------------\n");
              }
         else{
              printf("-----------------\n");
              printf("IT'S A DRAW 32-32\n");
              printf("-----------------\n");
              }
         printf("\n's' to save  'n' for new game  'm' for menu  'q' to quit");
         do{
            letter=getch();
            if(letter=='s'){printf("\n"); comhumansave(board,player,complayer,mode,depth,depthperfect,times); goto loop2;}
            if(letter=='n') return EXIT;
            if(letter=='m') return 0;
            if(letter=='q') finish(0);
            }while(1);
         }
    else{ //pass turn :)
         if(player==complayer){
                               printf("\n\nO-hello passed its turn");
                               printf("\n\npress any key to continue");
                               }
         else{
              printf("\n\nyour turn: no moves left",player);
              printf("\n\npress any key to pass turn");
              }
         getch();
         printf("\n");
         player=3-player; //switch player
         goto loop2;
         }
}

//save engine of human vs computer,computer vs human
void comhumansave(int board[64],int player,int complayer,int mode,int depth,int depthperfect,float times){
     char filename[100];
     strcpy(filename, UNIVERSALSTRING);
     save=fopen(strcat(filename,".ohl"),"w");
     //print settings
     PRINTSETTING();
     //print board
     for(int i = 0; i < 64; i++) fprintf(save," %d",board[i]);
     //print player
     fprintf(save," %d",player);
     //print loadmode
     fprintf(save," %d",4-complayer);
     //print mode
     fprintf(save," %d",mode);
     //print depth or times
     if(mode == 1) fprintf(save," %d %d",depth,depthperfect);
     if(mode == 2) fprintf(save," %f",times);
     fclose(save);
     printf("\ngame saved in %s",filename);
     getch();
}

//computer vs computer
int comcom(int board[64],int no[2],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]){
    printf(""); //prevent bug
    int i;
    int position = UNINIT;
    int mobilities; //value from function 'mobility'
    struct kirby flips; //value from function 'flip'
    char letter;
    int currmove=-1;
    int lastmove=-1;
    
    loop2: //start here!
    clrscr(); //clear screen
    //display board
    display(board,player,no,lastmove);
    //diplay player
	print_bottom(player, (player == 1) ? "O-hello 1 is" : "O-hello 2 is");
    //compute mobility
    indexformob(board);
    mobilities=mobility(player);
    if(mobilities!=0){ //if player is movable
                      printf("\nthinking");
					  space(10);
                      //get position
                      if(doublemode[player-1]==0) position=random(board,player,no,1);
                      if(doublemode[player-1]==1){
                                                  //for near-end evaluation, use depthperfect
                                                  if(no[0]+no[1]>=64-doubledepthperfect[player-1]) position=fsearch(board,doubledepthperfect[player-1],player,no,1);
                                                  else position=fsearch(board,doubledepth[player-1],player,no,1);
                                                  }
                      if(doublemode[player-1]==2) position=tsearch(board,doubletimes[player-1],player,no,1);
                      printf("\n\n's' to save  'n' for new game  'm' for menu  'q' to quit");
                      printf("\npress any other key to continue");
                      letter=getch();
                      if(letter=='s'){printf("\n"); comcomsave(board,player,doublemode,doubledepth,doubledepthperfect,doubletimes); goto loop2;}
                      if(letter=='n') return EXIT;
                      if(letter=='m') return 0;
                      if(letter=='q') finish(0);
                      currmove=position;
                      lastmove=currmove;
                      flips=flip(board,position,player); //flip!
                      //show flip animation
                      if(setting["flip"].get_bool()) flipanimation(board,player,position,flips.board);
                      //get board from function 'flip'
                      for(i=0;i<64;i++) board[i]=flips.board[i];
                      //update no[2]
                      no[player-1]+=flips.num+1;
                      no[2-player]-=flips.num;
                      player=3-player; //switch player
                      goto loop2;
                      }
    //if player is not movable
    else if(mobility(3-player)==0){ //if opponent is not movable too
         printf("\n\nno more moves can be made! game over!\n\n");
         //display result :)
         if(no[0]>no[1]){
                         printf("--------------------\n");
                         printf("O'HELLO 1 WINS %d-%d\n",64-no[1],no[1]);
                         printf("--------------------\n");
                         }
         else if(no[0]<no[1]){
              printf("--------------------\n");
              printf("O'HELLO 2 WINS %d-%d\n",64-no[0],no[0]);
              printf("--------------------\n");
              }
         else{
              printf("-----------------\n");
              printf("IT'S A DRAW 32-32\n");
              printf("-----------------\n");
              }
         printf("\n's' to save  'n' for new game  'm' for menu  'q' to quit");
         do{
            letter=getch();
            if(letter=='s'){printf("\n"); comcomsave(board,player,doublemode,doubledepth,doubledepthperfect,doubletimes); goto loop2;}
            if(letter=='n') return EXIT;;
            if(letter=='m') return 0;
            if(letter=='q') finish(0);
            }while(1);
         }
    else{ //pass turn :)
         printf("\n\nO-hello %d passed its turn\n\n",player);
         presstogo();
         printf("\n");
         player=3-player; //switch player
         goto loop2;
         }
}

//save engine of computer vs computer
void comcomsave(int board[64],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]){
     char filename[100];
     int i;
     alert("input file name (no extension)");
     texture vec;
     do{
         do{
             vec = getl();
         }while(vec.empty());
         auto option = vec.read();
         if(not vec.empty()){
             alert("too many parameters");
             continue;
         }
         setUstring(option.c_str());
     }while(false);
     strcpy(filename, UNIVERSALSTRING);
     save=fopen(strcat(filename,".ohl"),"w");
     //print settings
     PRINTSETTING();
     //print board
     for(i=0;i<64;i++) fprintf(save," %d",board[i]);
     //print player
     fprintf(save," %d",player);
     //print loadmode
     fprintf(save," 4");
     for(i=0;i<=1;i++){
                       //print doublemode
                       fprintf(save," %d",doublemode[i]);
                       //print doubledepth or doubletimes
                       if(doublemode[i]==1) fprintf(save," %d %d",doubledepth[i],doubledepthperfect[i]);
                       if(doublemode[i]==2) fprintf(save," %f",doubletimes[i]);
                       }
     fclose(save);
     printf("\ngame saved in %s",filename);
     getch();
}

//set weight for weight test
void weightfortest(int doubleweight[2][100],int player){
     int i;
     for(i=0;i<int(setting["weight"].get_vint().size());i++) weightRaw[i]=doubleweight[player-1][i]; // TODO
     nickname();
}

//weight test
void weighttest(){
	int doublemode[2];
	int doubledepth[2];
	int doubledepthperfect[2];
	float doubletimes[2];
	int doubleweight[2][100]; //weights for 2 players
	triad result;
	float score1, score2;

	printf("\nWEIGHT TEST SYTEM (FOR TECHNICAL USE)\n\n");
	printf("current weights are:\n");
	setting["weight"].show();
	printf("\n\n");
	printf("parameters: mode, depth/time, [weights]\n");
	printf("mode      : 0 = random, 1 = fsearch, 2 = tsearch\n");
	printf("for mode 1: input depth, depthperfect\n");
	printf("for mode 2: input time\n");

	for(int i = 0; i <= 1; i++){
		printf("\nenter computer %d parameters:\n\n", i + 1);
		//parameters = mode depth/time [weights]
		doublemode[i] = uget(int)([](int x){ return 0 <= x and x <= 2; }, "invalid: mode must be between 0 and 2");
		if(doublemode[i] == 1){
			doubledepth[i] = uget(int)([](int x){ return x <= 1; }, "invalid: depth must more than 0");
			doubledepthperfect[i] = uget(int)([](int x){ return x <= 1; }, "invalid: depthperfect must more than 0");
		}else if(doublemode[i] == 2){
			doubletimes[i] = uget(float)([](float x){ return x >= 0; }, "invalid: doubletimes must more than 0");
		}
		if(doublemode[i]!= 0){
			for(int j = 0; j < int(setting["weight"].get_vint().size()); j++)
				scanf("%d",&doubleweight[i][j]); // TODO
		}
	}
	printf("\n");
	printf("parameters: numgame, numrand, twoway\n");
	printf("numgame   : no. of test games\n");
	printf("numrand   : no. of random moves at beginning of game\n");
	printf("twoway    : 0 = com 1 as black, 1 = com 1 as black and white\n\n");
	printf("enter test parameters:\n\n");
	//parameters = numgame numrand twoway

	int numgame = uget(int)([](int x){ return x >= 1; }, "invalid: numgame must not less than 1"); // number of game to play
	int numrand = uget(int)([](int x){ return 0 <= x and x <= 60; }, "invalid: numrand must be between 0 and 60"); //number of random first moves
	int twoway =  uget(int)([](int x){ return x == 1 or x == 2; }, "invalid: twoway must be 1 or 2");
	//0 = computer 1 plays first only, 1 = computer 1,2 plays first

	//computer 1 vs computer 2
	printf("\noutput : ");
	result=gamefortest(doublemode,doubledepth,doubledepthperfect,doubletimes,doubleweight,numgame,numrand);
	printf("%d %d %d",result.win,result.lose,result.draw);

	score1=result.win+(float)result.draw/2;
	score2=result.lose+(float)result.draw/2;

	//percentage
	if(twoway == 0){
		printf("\n\ncom 1 won %.1f/%d games = %.2f%c",score1,numgame,100*score1/numgame,37);
		printf("\ncom 2 won %.1f/%d games = %.2f%c",score2,numgame,100*score2/numgame,37);
	}

	//computer 2 vs computer 1 (only if twoway=1)

	if(twoway == 1){
		printf("\noutput2: ");
		std::swap(doublemode[0], doublemode[1]);
		std::swap(doubledepth[0], doubledepth[1]);
		std::swap(doubledepthperfect[0], doubledepthperfect[1]);
		std::swap(doubletimes[0], doubletimes[1]);
		//weight
		for(int i = 0; i < int(setting["weight"].get_vint().size()); i++)
			std::swap(doubleweight[0][i], doubleweight[1][i]); // TODO
	
		result = gamefortest(doublemode,doubledepth,doubledepthperfect,doubletimes,doubleweight,numgame,numrand);     
		printf("%d %d %d",result.lose,result.win,result.draw);
		//here we switched position of result.lose and result.win

		score1 += result.lose + float(result.draw / 2.0);
		score2 += result.win + float(result.draw / 2.0);

		//percentage
		printf("\n\ncom 1 won %.1f/%d games = %.2f%c",score1,2*numgame,50*score1/numgame,37);
		printf("\ncom 2 won %.1f/%d games = %.2f%c",score2,2*numgame,50*score2/numgame,37);
	}
	printf("\n\n");
	presstogo();
	return;
}

triad gamefortest(int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2],int doubleweight[2][100],int numgame,int numrand){
     triad result;
     result.win = 0;
     result.lose = 0;
     result.draw = 0;
     int game = 0;
     
     int i;
     int position = UNINIT;
     int mobilities;
     kirby flips;
     
     //loop each game
     do{
        //set board
        int board[64] = {};
        board[27] = 2;
        board[28] = 1;
        board[35] = 1;
        board[36] = 2;
        int no[2] = {2,2};
        int player = 1;

        do{                 
           //compute mobility
           indexformob(board);
           mobilities=mobility(player);
           //if player is movable
           if(mobilities!=0){
                             //if random
                             if(doublemode[player-1]==0 or no[0]+no[1]<numrand+4) position=random(board,player,no,0);
                             else{
                                  //not random
                                  weightfortest(doubleweight,player); //set weight
                                  //get position
                                  if(doublemode[player-1]==1){
                                                              //for near-end evaluation, use depthperfect
                                                              if(no[0]+no[1]>=64-doubledepthperfect[player-1]) position=fsearch(board,doubledepthperfect[player-1],player,no,0);
                                                              else position=fsearch(board,doubledepth[player-1],player,no,0);
                                                              }
                                  if(doublemode[player-1]==2) position=tsearch(board,doubletimes[player-1],player,no,0);
                                  }
                             flips=flip(board,position,player); //flip!
                             //get board from function 'flip'
                             for(i=0;i<64;i++) board[i]=flips.board[i];
                             //update no[2]
                             no[player-1]+=flips.num+1;
                             no[2-player]-=flips.num;
                             }
           //if player is not movable
           else if(mobility(3-player)==0){ //if opponent is not movable too
                //report result
                if(no[0]>no[1]) result.win++;
                if(no[0]==no[1]) result.draw++;
                if(no[0]<no[1]) result.lose++;
                goto nextgame;
                }
           //else = pass turn = do nothing
           player=3-player; //switch player
           }while(1);
        nextgame:
        game++;
        }while(game<numgame);
     return result;
}

//return input from column/row format in 0-63 format 
int input(int board[64],int& player,int no[2], std::vector<undoStruct>& undoData, int& undoItr, int& lastMove){
    char row;
    char column;
	
    loop1:
	texture vec;
	do{
	 	vec = newinput(PLAYMODE);
	}while(vec.empty());
	
	auto option = vec.read();
	
	if(option == "~"){
		option = vec.read();
		// sure that get correct format
		if(option == "move") return -5;
	}
	
	if(option == "menu"){
        if(not vec.empty()){
            alert("too many parameters");
            goto loop1;
        }
		else return -4; //-4 means menu
	}
	if(option == "new"){
        if(not vec.empty()){
            alert("too many parameters");
            goto loop1;
        }
		else return -3; //-3 means new
	}
	if(option == "save"){
        if(vec.empty()){
            alert("input file name (no extension)");
            vec = getl();
        }
        option = vec.read();
        if(not vec.empty()){
            alert("too many parameters");
            goto loop1;
        }
        else{
            setUstring(option.c_str());
            return -999; //-999 means save
        }
    }
	if(option == "undo"){
        int step = -1;
        if(not vec.empty()){
            option = vec.read();
            if(option != "all") step = std::stoi(option);
        }else{
            step = 1;
        }
        if(step == -1){
            undoItr = 0;
        }else{
            if(undoItr - step < 0){
                alert("cannot undo");
                goto loop1;
            }else{
                undoItr -= step;
            }
        }
        for(int i=0;i<64;i++) board[i] = undoData[undoItr].board[i];
        no[0] = undoData[undoItr].no[0];
        no[1] = undoData[undoItr].no[1];
        player = undoData[undoItr].player;
        lastMove = undoData[undoItr].lastMove;
		return -1; //-1 means undo
	}
	if(option == "redo"){
        int step = -1;
        if(not vec.empty()){
            option = vec.read();
            if(option != "all") step = std::stoi(option);
        }else{
            step = 1;
        }
        if(step == -1){
            undoItr = (int)undoData.size() - 1;
        }else{
            if(undoItr + step >= (int)undoData.size()){
                alert("cannot redo");
                goto loop1;
            }else{
                undoItr += step;
            }
        }
        for(int i=0;i<64;i++) board[i] = undoData[undoItr].board[i];
        no[0] = undoData[undoItr].no[0];
        no[1] = undoData[undoItr].no[1];
        player= undoData[undoItr].player;
        lastMove = undoData[undoItr].lastMove;
		return -1; //-1 means undo
	}
	if(option == "reflect"){
        if(not vec.empty()){
            alert("too many parameters");
            goto loop1;
        }
		else return -2; //-2 means reflect
	}
	if(option == "fsearch"){
        int depth;
        if(vec.empty()){
            alert("input depth");
            depth = uget(int)([](int a){ return a >= 1; }, "invalid depth. please input depth again.");
        }else{
            depth = std::stoi(vec.read());
            if(not vec.empty()){
                alert("too many parameters");
                goto loop1;
            }
            if(depth < 1){
                alert("invalid depth");
                goto loop1;
            }
        }
        printf("\nthinking");
		space(10);
        fsearch(board, depth, player, no, 2);
        printf("\n\n");
        goto loop1;
	}
	if(option == "tsearch"){
        float times;
        if(vec.empty()){
            alert("input time (sec)");
            times = uget(float)([](float a){ return a > 0; }, "invalid time. please input time again.");
        }else{
            times = std::stof(vec.read());
            if(not vec.empty()){
                alert("too many parameters");
                goto loop1;
            }
            if(times <= 0){
                alert("invalid time");
                goto loop1;
            }
        }
        printf("\nthinking ");
		space(10);
        tsearch(board,times,player,no,2);
        printf("\n\n");
        goto loop1;
	}

    if(option.size()!=2){
        alert("invalid command");
        goto loop1;
        }
    column=option[0];
    row=option[1];
    //column to 0-7
	column -= 'a';
	if(column < 0 or column > 7){
		alert("invalid position");
		goto loop1;
	}
	row -= '1';
    //row to 0-7
	if(row < 0 or row > 7){
		alert("invalid position");
		goto loop1;
	}
	return row * 8 + column;
}

//display node with prefixes :)
void shownode(int node){
     if(node<1000) printf("%d n",node);
     else if(node<10000) printf("%.2f kn",0.01*floor(0.1*node));
     else if(node<100000) printf("%.1f kn",0.1*floor(0.01*node));
     else if(node<1000000) printf("%.0f kn",floor(0.001*node));
     else if(node<10000000) printf("%.2f Mn",0.01*floor(0.0001*node));
     else if(node<100000000) printf("%.1f Mn",0.1*floor(0.00001*node));
     else if(node<1000000000) printf("%.0f Mn",floor(0.000001*node));
     else printf("%.2f Gn",0.01*floor(0.0000001*node));
     printf("  ");
}

//-- random,fsearch,tsearch --
//determine the position that computer places a disk
//int display = mode of display
//0 = nothing (for weight test)
//1 = for computer's turn
//2 = for in-game search

//random move
int random(int board[64],int player,int no[2],int display){
    int position;
    struct kirby moves; //from function 'move'
    
    moves=move(board,player); //get move list
    
    //opening move
    if(no[0]+no[1]==5){
                       //if no parallel opening
                       if(not setting["parallel"].get_bool()){
                                            //recompute moves.board
                                            if(board[19]!=0){moves.board[0]=18; moves.board[1]=34;}
                                            if(board[26]!=0){moves.board[0]=18; moves.board[1]=20;}
                                            if(board[37]!=0){moves.board[0]=43; moves.board[1]=45;}
                                            if(board[44]!=0){moves.board[0]=29; moves.board[1]=45;}
                                            if(board[20]!=0){moves.board[0]=21; moves.board[1]=37;}
                                            if(board[29]!=0){moves.board[0]=19; moves.board[1]=21;}
                                            if(board[34]!=0){moves.board[0]=42; moves.board[1]=44;}
                                            if(board[43]!=0){moves.board[0]=26; moves.board[1]=42;}
                                            
                                            if(setting["rand"].get_bool()) position = moves.board[rand(2)];
                                            else position=moves.board[0];
                                            }
                       //if allow parallel opening
                       else{
                            if(setting["rand"].get_bool()) position = moves.board[rand(3)];
                            else position=moves.board[0];
                            }
                       }
    else position=moves.board[rand(moves.num)]; //random move
    
    if(display!=0){
                   backspace(18); //clear "thinking "
                   printf("O-hello decided to place a disk at ");
                   switch(position%8){
                                      case 0:printf("a"); break;
                                      case 1:printf("b"); break;
                                      case 2:printf("c"); break;
                                      case 3:printf("d"); break;
                                      case 4:printf("e"); break;
                                      case 5:printf("f"); break;
                                      case 6:printf("g"); break;
                                      case 7:printf("h"); break;
                                      default:;
                                      }
                   printf("%d",position/8+1);
                   }
    return position;
}

//fsearch: fixed depth search
//act like function 'score' that operates on the first depth
//*now it can perform shallow search first before the deep search
int fsearch(int board[64],int depthwant,int player,int no[2],int display){
    struct timeb time1,time2; //for 'ftime'
    int scores[64]; //keep scores of each position
    int bestscore = UNINIT; //best of scores[64]
    struct kirby flips; //from function 'flip'
    struct kirby moves; //from function 'move'
    int position;
    int candidate[64]; //keep positions with bestscore
    int numcan=0; //number of candidates
    int nonew[2]; //no[2] after a flip
    int k;
    int a;
    int depthshallow; //for shallow search
    int enginestate=0; //0=normal,1=only reply,2=opening
    int perfect=0;
	int alpha,beta;
    
    ftime(&time1); //get current time
    node=0; //reset node
    moves=move(board,player); //get move list

    //if only one legal move -- no need to search
    if(moves.num==1){position=moves.board[0]; depthwant=0; enginestate=1; goto finish;}

    //for starting position -- no need to search
    if(no[0]+no[1]==4){
                       //if random feature is on
                       if(setting["rand"].get_bool()) position = moves.board[rand(4)];
                       else position=moves.board[0];
                       depthwant=0;
                       enginestate=2;
                       goto finish;
                       }
    if(no[0]+no[1]==5){
                       //if no parallel opening
                       if(not setting["parallel"].get_bool()){
                                            //recompute moves.board
                                            if(board[19]!=0){moves.board[0]=18; moves.board[1]=34;}
                                            if(board[26]!=0){moves.board[0]=18; moves.board[1]=20;}
                                            if(board[37]!=0){moves.board[0]=43; moves.board[1]=45;}
                                            if(board[44]!=0){moves.board[0]=29; moves.board[1]=45;}
                                            if(board[20]!=0){moves.board[0]=21; moves.board[1]=37;}
                                            if(board[29]!=0){moves.board[0]=19; moves.board[1]=21;}
                                            if(board[34]!=0){moves.board[0]=42; moves.board[1]=44;}
                                            if(board[43]!=0){moves.board[0]=26; moves.board[1]=42;}
                                            
                                            if(setting["rand"].get_bool()) position = moves.board[rand(2)];
                                            else position=moves.board[0];
                                            }
                       //if allow parallel opening
                       else{
                            if(setting["rand"].get_bool()) position = moves.board[rand(3)];
                            else position=moves.board[0];
                            }
                       
                       depthwant=0;
                       enginestate=2;
                       goto finish;
                       }
    
    //if searches up to end-game (or 1 square left)
    if(depthwant>=63-no[0]-no[1]){
                                  depthwant=64-no[0]-no[1];
                                  depthshallow=depthShallow(depthwant, true);
                                  enginestate=0;
                                  perfect=1;
                                  }
    else{
         depthshallow=depthShallow(depthwant, false);
         enginestate=0;
         perfect=0;
         }
    
    //for the shallow search------------------------------------
    
    if(depthshallow>0){
                       //set extreme value
                       alpha = -LARGE;
                       beta = LARGE;   
                       //get score for all legal moves
                       for(k=0;k<moves.num;k++){
                                                position=moves.board[k];
                                                flips=flip(board,position,player); //flip!
                                                //update no[2] (as nonew[2])
                                                nonew[player-1]=no[player-1]+flips.num+1;
                                                nonew[2-player]=no[2-player]-flips.num;
                                                //get score
                                                scores[k]=score(flips.board,depthshallow-1,3-player,nonew,alpha,beta,display);
												//NO UPDATE alpha,beta IN SHALLOW SEARCH
												/*
                                                if(player==1){ //if player 1: max mode
                                                               if(scores[k]>alpha) alpha=scores[k];
                                                               }
                                                else{ //if player 2: min mode
                                                      if(scores[k]<beta) beta=scores[k];
                                                      }*/
                                                }
                       //rearrange moves by scores from shallow search
                       order:
                       for(k=0;k<moves.num-1;k++){
                                                  if((player==1 and scores[k]<scores[k+1]) or (player==2 and scores[k]>scores[k+1])){
                                                                a=scores[k]; //switch score
                                                                scores[k]=scores[k+1];
                                                                scores[k+1]=a;
                                                                a=moves.board[k]; //switch position
                                                                moves.board[k]=moves.board[k+1];
                                                                moves.board[k+1]=a;
                                                                goto order;
                                                                }
                                                  }
                       }
    
    //for the deep search---------------------------------------
    
    //set extreme value
    alpha = -LARGE;
    beta = LARGE;          
    //get score for all legal moves
    for(k=0;k<moves.num;k++){
                             position=moves.board[k];
                             flips=flip(board,position,player); //flip!
                             //update no[2] (as nonew[2])
                             nonew[player-1]=no[player-1]+flips.num+1;
                             nonew[2-player]=no[2-player]-flips.num;
                             //get score
                             scores[k]=score(flips.board,depthwant-1,3-player,nonew,alpha,beta,display);
                             if(player==1){ //if player 1: max mode
                                           if(scores[k]>alpha) alpha=scores[k];
                                           }
                             else{ //if player 2: min mode
                                  if(scores[k]<beta) beta=scores[k];
                                  }
                             }
                             
    //---------------------------------------------------------
    
    //set extreme value
    if(player==1) bestscore=-LARGE;
    else bestscore=LARGE;
    //determine candidates
    for(k=0;k<moves.num;k++){
                             position=moves.board[k];
							 //ONLY ONE CANDIDATE
							 /*
                             if(scores[k]==bestscore){ //add a candidate
                                                      numcan++;
                                                      candidate[numcan-1]=position;
                                                      }*/
                             if(player==1){ //if player 1: max mode
                                           if(scores[k]>bestscore){
                                                                   bestscore=scores[k];
                                                                   //be the first candidate
                                                                   numcan=1;
                                                                   candidate[0]=position;
                                                                   }
                                           }
                             else{ //if player 2: min mode
                                  if(scores[k]<bestscore){
                                                          bestscore=scores[k];
                                                          //be the first candidate
                                                          numcan=1;
                                                          candidate[0]=position;
                                                          }
                                  }
                             }

    //arrrange candidates (for same result in randoff mode)
    order2:
    for(k=0;k<numcan-1;k++){
                            if(candidate[k]>candidate[k+1]){
                                                            a=candidate[k];
                                                            candidate[k]=candidate[k+1];
                                                            candidate[k+1]=a;
                                                            goto order2;
                                                            }
                            }
    //if random feature is on
    if(setting["rand"].get_bool()) position = candidate[rand(numcan)]; //random from candidates 
    else position=candidate[0];

    finish:
    ftime(&time2); //get current time
    if(display != 0){
		if(display == 1){
			backspace(18); //clear "thinking "
			printf("O-hello decided to place a disk at ");
		}else{
			backspace(18); //clear "thinking "
			printf("search result: ");
		}
		switch(position % 8){
			case 0: printf("a"); break;
			case 1: printf("b"); break;
			case 2: printf("c"); break;
			case 3: printf("d"); break;
			case 4: printf("e"); break;
			case 5: printf("f"); break;
			case 6: printf("g"); break;
			case 7: printf("h"); break;
		}
		printf("%d ", position / 8 + 1);
		//.time = seconds, .millitm = milliseconds
		printf("\n\n%.2f sec  ", time2.time - time1.time + 0.001 * (time2.millitm - time1.millitm));
		shownode(node); //display node
		printf("depth %d  ", depthwant);
		//display score
		switch(enginestate){
			case 0:
				if(player == 1){
					 if(bestscore >= wf) printf(": win +%d", bestscore / wf);
					 else if(bestscore <= -wf) printf(": lose %d", bestscore / wf);
					 else if(bestscore == 0 and perfect == 1) printf(": draw");
					 else if(bestscore >= 0) printf(": score +%d", bestscore);
					 else printf(": score %d", bestscore);
				}else{
					if(bestscore <= -wf) printf(": win +%d", -bestscore / wf);
					else if(bestscore >= wf) printf(": lose %d", -bestscore / wf);
					else if(bestscore == 0 and perfect == 1) printf(": draw");
					else if(bestscore <= 0) printf(": score +%d", -bestscore);
					else printf(": score %d", -bestscore);
				}
				break;
			case 1: printf(": only move"); break;
			case 2: printf(": opening"); break;
		}
	}
    return position;
}

//tsearch: time limit search
//search depth 1 until time's up or reach end-game
//and arrange move order according to the scores from the previous depth
//checkpoints are at each position
//so it can stop between the depths (time control is still not good)
//but scores from different depths cannot be compared
//time taken will be about 0.5x-2x time limit
int tsearch(int board[64],float times,int player,int no[2],int display){
    struct timeb time1,time2; //for 'ftime'
    int scores[64]; //keep scores of each position
    int bestscore = UNINIT; //best of scores[64]
    struct kirby flips; //from function 'flip'
    struct kirby moves; //from function 'move'
    int position;
    int candidate[64]; //keep positions with bestscore
    int numcan=0; //number of candidates
    int nonew[2]; //no[2] after a flip
    int k;
    int a;
    int depth; //current depth
    int posreach; //position reached when time's up
    int enginestate=0; //0=normal,1=only reply,2=opening
    int perfect=0;
	int alpha,beta;
    
    ftime(&time1); //get current time
    node=0; //reset node
    moves=move(board,player); //get move list

    //if only one legal move -- no need to search
    if(moves.num==1){
                     position=moves.board[0];
                     depth=1;
                     posreach=0;
                     enginestate=1;
                     goto finish;
                     }

    //for starting position -- no need to search
    if(no[0]+no[1]==4){
                       //if random feature is on
                       if(setting["rand"].get_bool()) position = moves.board[rand(4)];
                       else position=moves.board[0];
                       depth=1;
                       posreach=0;
                       enginestate=2;
                       goto finish;
                       }
    if(no[0]+no[1]==5){
                       //if no parallel opening
                       if(not setting["parallel"].get_bool()){
                                            //recompute moves.board
                                            if(board[19]!=0){moves.board[0]=18; moves.board[1]=34;}
                                            if(board[26]!=0){moves.board[0]=18; moves.board[1]=20;}
                                            if(board[37]!=0){moves.board[0]=43; moves.board[1]=45;}
                                            if(board[44]!=0){moves.board[0]=29; moves.board[1]=45;}
                                            if(board[20]!=0){moves.board[0]=21; moves.board[1]=37;}
                                            if(board[29]!=0){moves.board[0]=19; moves.board[1]=21;}
                                            if(board[34]!=0){moves.board[0]=42; moves.board[1]=44;}
                                            if(board[43]!=0){moves.board[0]=26; moves.board[1]=42;}
                                            
                                            if(setting["rand"].get_bool()) position = moves.board[rand(2)];
                                            else position=moves.board[0];
                                            }
                       //if allow parallel opening
                       else{
                            if(setting["rand"].get_bool()) position = moves.board[rand(3)];
                            else position=moves.board[0];
                            }
                       
                       depth=1;
                       posreach=0;
                       enginestate=2;
                       goto finish;
                       }

    //increase depth until board is completely filled
    //(or time's up -- the time condition is dealt with inside the loop)
    for(depth=1;depth<=64-no[0]-no[1];depth++){
                                               //skip the before-last depth
                                               if(depth==63-no[0]-no[1]) depth++;
                                               //set extreme value
                                               alpha = - LARGE;
											   beta = LARGE;
                                               //get score for all legal moves
                                               for(k=0;k<moves.num;k++){
                                                                        position=moves.board[k];
                                                                        flips=flip(board,position,player); //flip!
                                                                        //update no[2] (as nonew[2])
                                                                        nonew[player-1]=no[player-1]+flips.num+1;
                                                                        nonew[2-player]=no[2-player]-flips.num;
                                                                        //get score
                                                                        scores[k]=score(flips.board,depth-1,3-player,nonew,alpha,beta,display);
                                                                        
                                                                        //-----check time-----
                                                                        ftime(&time2); //get current time
                                                                        if(k==moves.num-1){
                                                                                           //if last move of the depth, check half of time limit
                                                                                           if(time2.time-time1.time+0.001*(time2.millitm-time1.millitm)>times/2){
                                                                                           posreach=k+1; //reach move no.k+1
                                                                                           goto timesup;
                                                                                           }
                                                                                           }
                                                                        else{
                                                                             if(time2.time-time1.time+0.001*(time2.millitm-time1.millitm)>times){
                                                                             posreach=k+1; //reach move no.k+1
                                                                             goto timesup;
                                                                             }
                                                                             }
																			 //NO UPDATE alpha,beta ???
																			 /*
                                                                        if(player==1){ //if player 1: max mode
                                                                                      if(scores[k]>alpha) alpha=scores[k];
                                                                                      }
                                                                        else{ //if player 2: min mode
                                                                             if(scores[k]<beta) beta=scores[k];
                                                                             }*/
                                                                        }
                                               //rearrange moves by scores from previous depth
                                               order:
                                               for(k=0;k<moves.num-1;k++){
                                                                          if((player==1 and scores[k]<scores[k+1]) or (player==2 and scores[k]>scores[k+1])){
                                                                                        a=scores[k]; //switch score
                                                                                        scores[k]=scores[k+1];
                                                                                        scores[k+1]=a;
                                                                                        a=moves.board[k]; //switch position
                                                                                        moves.board[k]=moves.board[k+1];
                                                                                        moves.board[k+1]=a;
                                                                                        goto order;
                                                                                        }
                                                                          }
                                               }
    //reached end-game
    depth--; //correct depth
    posreach=moves.num;
    perfect=1;
    
    timesup:
    //set extreme value
    if(player==1) bestscore=-LARGE;
    else bestscore=LARGE;
    //determine candidates
    for(k=0;k<posreach;k++){ //only consider up to posreach,not moves.num
                            position=moves.board[k];
                            if(scores[k]==bestscore){ //add a candidate
                                                     numcan++;
                                                     candidate[numcan-1]=position;
                                                     }
                            if(player==1){ //if player 1: max mode
                                          if(scores[k]>bestscore){
                                                                  bestscore=scores[k];
                                                                  //be the first candidate
                                                                  numcan=1;
                                                                  candidate[0]=position;
                                                                  }
                                          }
                            else{ //if player 2: min mode
                                 if(scores[k]<bestscore){
                                                         bestscore=scores[k];
                                                         //be the first candidate
                                                         numcan=1;
                                                         candidate[0]=position;
                                                         }
                                 }
                            }

    //arrrange candidates (for same result in randoff mode)
    order2:
    for(k=0;k<numcan-1;k++){
                            if(candidate[k]>candidate[k+1]){
                                                            a=candidate[k];
                                                            candidate[k]=candidate[k+1];
                                                            candidate[k+1]=a;
                                                            goto order2;
                                                            }
                            }
    //if random feature is on
    if(setting["rand"].get_bool()) position = candidate[rand(numcan)]; //random from candidates
    else position = candidate[0];

    finish:
    ftime(&time2); //get current time
    if(display!=0){
                   if(display==1){
                                  backspace(18); //clear "thinking "
                                  printf("O-hello decided to place a disk at ");
                                  }
                   else{
                        backspace(18); //clear "thinking "
                        printf("search result: ");
                        }
                   switch(position%8){
                                      case 0:printf("a"); break;
                                      case 1:printf("b"); break;
                                      case 2:printf("c"); break;
                                      case 3:printf("d"); break;
                                      case 4:printf("e"); break;
                                      case 5:printf("f"); break;
                                      case 6:printf("g"); break;
                                      case 7:printf("h"); break;
                                      default:;
                                      }
                   printf("%d ",position/8+1);
                   //.time = seconds, .millitm = milliseconds
                   printf("\n\n%.2f sec  ",time2.time-time1.time+0.001*(time2.millitm-time1.millitm));
                   shownode(node); //display node
                   printf("depth %.1f  ",depth-1+(float)(posreach)/moves.num);
                   //display score
                   switch(enginestate){
                                       case 0: if(player==1){
                                                             if(bestscore>=wf) printf(": win +%d",bestscore/wf);
                                                             else if(bestscore<=-wf) printf(": lose %d",bestscore/wf);
                                                             else if(bestscore==0 and perfect==1) printf(": draw");
                                                             else if(bestscore>=0) printf(": score +%d",bestscore);
                                                             else printf(": score %d",bestscore);
                                                             }
                                               else{
                                                    if(bestscore<=-wf) printf(": win +%d",-bestscore/wf);
                                                    else if(bestscore>=wf) printf(": lose %d",-bestscore/wf);
                                                    else if(bestscore==0 and perfect==1) printf(": draw");
                                                    else if(bestscore<=0) printf(": score +%d",-bestscore);
                                                    else printf(": score %d",-bestscore);
                                                    }
                                               break;
                                       case 1: printf(": only move"); break;
                                       case 2: printf(": opening"); break;
                                       default:;
                                       }
                   }
    return position;
}

//compute score by depth-first search using minimax algorithm
//with alpha-beta pruning
//cmpscore=compare score, for alpha-beta pruning

//*evaluation function*
//sigma(weight*component)
//if game ends -- wf(disk diff)
//(so that a win or a lose outweighs the 'normal' score)
//if board is nearly filled(>=wcut disks) -- use the "near end-game" weight
int score(int board[64],int depthleft,int player,int no[2],int alphaGet,int betaGet,int display){
     int scores; //keep the current best score
     int candidate; //candidate score
     struct kirby flips; //from function 'flip'
     int position;
     int nonew[2]; //no[2] after a flip
     int mo[2]; //mobility of each player (in evaluation part)
     int index; //index of move order
     int depthshallow; //for shallow search
     int scorearray[64]; //score array (for shallow search)
     int posarray[64]; //position array associated with score array
     int movenum; //number of moves (dimension of score array)
     int k;
     int a;
	 int alpha,beta;

     //if completely filled -- game ends
     if(no[0]+no[1]==64){
                         node++;
                         if(node% setting["rotatetime"].get_int()==0) nodedisplay(display);
                         if(no[0]>no[1]) return wf*(64-2*no[1]);
                         else if(no[0]<no[1]) return wf*(2*no[0]-64);
                         else return 0;
                         }

     //terminal -- return evaluation function ----------------------------------

     if(depthleft==0){
                      node++;
                      if(node%setting["rotatetime"].get_int()==0) nodedisplay(display);
                      indexformob(board);
                      
                      //if nearly filled
                      if(no[0]+no[1]>=wcut){
                                          mo[player-1]=mobility(player);
                                          //if player is movable
                                          if(mo[player-1]!=0){
                                                              scores=0;
                                                              if(wd1!=0) scores+=wd1*(no[0]-no[1]);
                                                              if(wm1!=0){
                                                                          mo[2-player]=mobility(3-player);
                                                                          //use player1-player2
                                                                          scores+=wm1*(mo[0]-mo[1]);
                                                                          }
                                                              if(wp1!=0) scores+=wp1*pmobility(board);
                                                              scores+=triplesq(board,wc1,wxx1,wcc1);
                                                              if(we1!=0) scores+=(int)(we1*edgevalue(board,player));
                                                              if(ws1!=0) scores+=ws1*stabledisk(board);
                                                              return scores;
                                                              }
                                          mo[2-player]=mobility(3-player);
                                          //if opponent is movable
                                          if(mo[2-player]!=0){
                                                              scores=0;
                                                              if(wd1!=0) scores+=wd1*(no[0]-no[1]);
                                                              if(wm1!=0) scores+=wm1*(mo[0]-mo[1]);
                                                              if(wp1!=0) scores+=wp1*pmobility(board);
                                                              scores+=triplesq(board,wc1,wxx1,wcc1);
                                                              if(we1!=0) scores+=(int)(we1*edgevalue(board,player));
                                                              if(ws1!=0) scores+=ws1*stabledisk(board);
                                                              return scores;
                                                              }
                                          //if no one is movable (game ends)
                                          if(no[0]>no[1]) return wf*(64-2*no[1]);
                                          else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                          else return 0;
                                          }
                      
                      //if not nearly filled
                      mo[player-1]=mobility(player);
                      //if player is movable
                      if(mo[player-1]!=0){
                                          scores=0;
                                          if(wd!=0) scores+=wd*(no[0]-no[1]);
                                          if(wm!=0){
                                                    mo[2-player]=mobility(3-player);
                                                    //use player1-player2
                                                    scores+=wm*(mo[0]-mo[1]);
                                                    }
                                          if(wp!=0) scores+=wp*pmobility(board);
                                          scores+=triplesq(board,wc,wxx,wcc);
                                          if(we!=0) scores+=(int)(we*edgevalue(board,player));
                                          if(ws!=0) scores+=ws*stabledisk(board);
                                          return scores;
                                          }
                      mo[2-player]=mobility(3-player);
                      //if opponent is movable
                      if(mo[2-player]!=0){
                                          scores=0;
                                          if(wd!=0) scores+=wd*(no[0]-no[1]);
                                          if(wm!=0) scores+=wm*(mo[0]-mo[1]);
                                          if(wp!=0) scores+=wp*pmobility(board);
                                          scores+=triplesq(board,wc,wxx,wcc);
                                          if(we!=0) scores+=(int)(we*edgevalue(board,player));
                                          if(ws!=0) scores+=ws*stabledisk(board);
                                          return scores;
                                          }
                      //if no one is movable (game ends)
                      if(no[0]>no[1]) return wf*(64-2*no[1]);
                      else if(no[0]<no[1]) return wf*(2*no[0]-64);
                      else return 0;
                      }

     //not terminal ------------------------------------------------------------
     
     //1 square left -- use simplified score function to save time
     if(no[0]+no[1]==63) return score63(board,player,no,display);
     
     //determine shallow depth

	depthshallow = depthShallow(depthleft, depthleft >= 63 - no[0] - no[1]);
     
     //if no shallow search -- no record on position scores,move list, etc.
     //===================================================================
     if(depthshallow<=0){
     
     if(player==1){
                   //if player 1: max mode
                   alpha = alphaGet;
				   beta = betaGet;
				   bool IsFlip = false;
                   for(index=0;index<60;index++){
                                                 position=moveOrder[index];
                                                 //check before flip
                                                 if(board[position]==0){
                                                                        flips=flip(board,position,player); //flip!
                                                                        //if move is legal
                                                                        if(flips.num!=0){
																			IsFlip = true;
                                                                                         //update no[2] (as nonew[2])
                                                                                         nonew[player-1]=no[player-1]+flips.num+1;
                                                                                         nonew[2-player]=no[2-player]-flips.num;
                                                                                         //get score
                                                                                         //scores is used as cmpscore
                                                                                         candidate=score(flips.board,depthleft-1,3-player,nonew,alpha,beta,display);
                                                                                         if(candidate>=beta) return candidate; //alpha-beta pruning
                                                                                         if(candidate>alpha) alpha=candidate; //update score
                                                                                         }
                                                                        }
                                                 }
                   //if no legal move
                   if(!IsFlip){
                                          //if game ends
                                          indexformob(board);
                                          if(mobility(3-player)==0){
                                                                          node++;
                                                                          if(node%setting["rotatetime"].get_int()==0) nodedisplay(display);
                                                                          if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                          else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                          else return 0;
                                                                          }
                                          //if pass turn
                                          //use depthleft not depthleft-1 to preserve no. of disks
                                          return score(board,depthleft,3-player,no,alpha,beta,display);
                                          }
                   return alpha;
                   }
     else{
          //if player 2: min mode
		 alpha = alphaGet;
          beta = betaGet;
		  bool IsFlip = false;
          for(index=0;index<60;index++){
                                        position=moveOrder[index];
                                        if(board[position]==0){
                                                               flips=flip(board,position,player); //flip!
                                                               //if move is legal
                                                               if(flips.num!=0){
																   IsFlip = true;
                                                                                //update no[2] (as nonew[2])
                                                                                nonew[player-1]=no[player-1]+flips.num+1;
                                                                                nonew[2-player]=no[2-player]-flips.num;
                                                                                //get score
                                                                                //scores is used as cmpscore
                                                                                candidate=score(flips.board,depthleft-1,3-player,nonew,alpha,beta,display);
                                                                                if(candidate<=alpha) return candidate; //alpha-beta pruning
                                                                                if(candidate<beta) beta=candidate; //update score
                                                                                }
                                                               }
                                        }
          //if no legal move
          if(!IsFlip){
                                //if game ends
                                indexformob(board);
                                if(mobility(3-player)==0){
                                                                node++;
                                                                if(node%setting["rotatetime"].get_int()==0) nodedisplay(display);
                                                                if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                else return 0;
                                                                }
                                //if pass turn
                                //use depthleft not depthleft-1 to preserve no. of disks
                                return score(board,depthleft,3-player,no,alpha,beta,display);
                                }
          return beta;
          }
     
     }
     
     //if shallow search first
     //=======================
     
     else{
     
     //this part is for the shallow search (score array is used)
     //---------------------------------------------------------
     
     //initialize number of moves
     movenum=0;
     
     if(player==1){
                   //if player 1: max mode
                   alpha = alphaGet;
				   beta = betaGet;
                   for(index=0;index<60;index++){
                                                 position=moveOrder[index];
                                                 //check before flip
                                                 if(board[position]==0){
                                                                        flips=flip(board,position,player); //flip!
                                                                        //if move is legal
                                                                        if(flips.num!=0){
                                                                                         //increase number of moves
                                                                                         movenum++;
                                                                                         //update no[2] (as nonew[2])
                                                                                         nonew[player-1]=no[player-1]+flips.num+1;
                                                                                         nonew[2-player]=no[2-player]-flips.num;
                                                                                         //get score
                                                                                         //scores is used as cmpscore
                                                                                         candidate=score(flips.board,depthshallow-1,3-player,nonew,alpha,beta,display);
                                                                                         scorearray[movenum-1]=candidate;
                                                                                         posarray[movenum-1]=position;
                                                                                         //[this line to return score is deleted in shallow search]
                                                                                         if(candidate>alpha) alpha=candidate; //update score
                                                                                         }
                                                                        }
                                                 }
                   //if no legal move (allow to return score)
                   if(movenum==0){
                                          //if game ends
                                          indexformob(board);
                                          if(mobility(3-player)==0){
                                                                          node++;
                                                                          if(node%setting["rotatetime"].get_int()==0) nodedisplay(display);
                                                                          if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                          else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                          else return 0;
                                                                          }
                                          //if pass turn
                                          //use depthleft not depthleft-1 to preserve no. of disks
                                          return score(board,depthleft,3-player,no,alpha,beta,display);
                                          }
                   //[this line to return score is deleted in shallow search]
                   }
     else{
          //if player 2: min mode
		 alpha = alphaGet;
          beta = betaGet;
          for(index=0;index<60;index++){
                                        position=moveOrder[index];
                                        if(board[position]==0){
                                                               flips=flip(board,position,player); //flip!
                                                               //if move is legal
                                                               if(flips.num!=0){
                                                                                //increase number of moves
                                                                                movenum++;
                                                                                //update no[2] (as nonew[2])
                                                                                nonew[player-1]=no[player-1]+flips.num+1;
                                                                                nonew[2-player]=no[2-player]-flips.num;
                                                                                //get score
                                                                                //scores is used as cmpscore
                                                                                candidate=score(flips.board,depthshallow-1,3-player,nonew,alpha,beta,display);
                                                                                scorearray[movenum-1]=candidate;
                                                                                posarray[movenum-1]=position;
                                                                                //[this line to return score is deleted in shallow search]
                                                                                if(candidate<beta) beta=candidate; //update score
                                                                                }
                                                               }
                                        }
          //if no legal move (allow to return score)
          if(movenum==0){
                                //if game ends
                                indexformob(board);
                                if(mobility(3-player)==0){
                                                                node++;
                                                                if(node%setting["rotatetime"].get_int()==0) nodedisplay(display);
                                                                if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                else return 0;
                                                                }
                                //if pass turn
                                //use depthleft not depthleft-1 to preserve no. of disks
                                return score(board,depthleft,3-player,no,alpha,beta,display);
                                }
          //[this line to return score is deleted in shallow search]
          }
     
     //this part is to rearrange the score and position array from shallow search
     //--------------------------------------------------------------------------
     
     order:
     for(k=0;k<movenum-1;k++){
                              if((player==1 and scorearray[k]<scorearray[k+1]) or (player==2 and scorearray[k]>scorearray[k+1])){
                                            a=scorearray[k]; //switch score
                                            scorearray[k]=scorearray[k+1];
                                            scorearray[k+1]=a;
                                            a=posarray[k]; //switch position
                                            posarray[k]=posarray[k+1];
                                            posarray[k+1]=a;
                                            goto order;
                                            }
                              }
     
     //this part is for the deep search
     //we use the prototype from fsearch since we already have position array
     //----------------------------------------------------------------------
     
	 alpha = alphaGet;
	 beta = betaGet;
     //get score for all legal moves
     for(k=0;k<movenum;k++){
                            position=posarray[k];
                            flips=flip(board,position,player); //flip!
                            //update no[2] (as nonew[2])
                            nonew[player-1]=no[player-1]+flips.num+1;
                            nonew[2-player]=no[2-player]-flips.num;
                            //get score
                            candidate=score(flips.board,depthleft-1,3-player,nonew,alpha,beta,display);
                            if(player==1){ //if player 1: max mode
                                           if(candidate>=beta) return candidate; //*add alpha-beta pruning here
                                           if(candidate>alpha) alpha=candidate;
                                           }
                            else{ //if player 2: min mode
                                  if(candidate<=alpha) return candidate; //*add alpha-beta pruning here
                                  if(candidate<beta) beta=candidate;
                                  }
                            }
			if(player == 1) return alpha;
			else return beta;
     }
}

//compute score when there is only 1 square left
//all unnecessary computations are removed to save time
int score63(int board[64],int player,int no[2],int display){
    node++; //this is one node exactly!
    if(node%setting["rotatetime"].get_int()==0) nodedisplay(display);
    
    int flipplayer;
    int flipnonplayer;
    int i=0;
    while(board[i]!=0) i++; //find the remaining square
    flipplayer=flipnum(board,i,player);
    if(flipplayer!=0){
                      if(player==1){
                                    no[0]+=flipplayer+1;
                                    no[1]-=flipplayer;
                                    }
                      else{
                           no[1]+=flipplayer+1;
                           no[0]-=flipplayer;
                           }
                      if(no[0]>no[1]) return wf*(64-2*no[1]);
                      else if(no[0]<no[1]) return wf*(2*no[0]-64);
                      else return 0;
                      }
    else{
         //player must pass
         flipnonplayer=flipnum(board,i,3-player);
         if(flipnonplayer!=0){
                              if(player==1){
                                            no[1]+=flipnonplayer+1;
                                            no[0]-=flipnonplayer;
                                            }
                              else{
                                   no[0]+=flipnonplayer+1;
                                   no[1]-=flipnonplayer;
                                   }
                              if(no[0]>no[1]) return wf*(64-2*no[1]);
                              else if(no[0]<no[1]) return wf*(2*no[0]-64);
                              else return 0;
                              }
         //game ends
         else{
              if(no[0]>no[1]) return wf*(64-2*no[1]);
              else if(no[0]<no[1]) return wf*(2*no[0]-64);
              else return 0;
              }
         }
}

//display rotation effect
void nodedisplay(int display){
     if(display!=0 and setting["rotate"].get_bool()){
		 backspace(9);
                   switch((node/setting["rotatetime"].get_int())%4){
                                               case 1: printf("- "); break; // horizontal 
                                               case 2: printf("\\ "); break;  // backslash
                                               case 3: printf("| "); break; // vertical
                                               case 0: printf("/ "); break;  // slash
                                               default:;
                                               }
                   //display exactly nine blocks
                   if(node<10) printf("%d n    ",node);
                   else if(node<100) printf("%d n   ",node);
                   else if(node<1000) printf("%d n  ",node);
                   else if(node<10000) printf("%.2f kn",0.01*floor(0.1*node));
                   else if(node<100000) printf("%.1f kn",0.1*floor(0.01*node));
                   else if(node<1000000) printf("%.0f kn ",floor(0.001*node));
                   else if(node<10000000) printf("%.2f Mn",0.01*floor(0.0001*node));
                   else if(node<100000000) printf("%.1f Mn",0.1*floor(0.00001*node));
                   else if(node<1000000000) printf("%.0f Mn ",floor(0.000001*node));
                   else printf("%.2f Gn",0.01*floor(0.0000001*node));
                   }
}

//display board string
//to be used in function 'display' and 'flipanimation'
//interpretation of board input: 0=empty,1=black,2=white,3=flipping
void boarddisplay(int board[64],int playertoshowmove){
	std::string output;

	//ascii for components
	char upLeftCorner = '+';
	char upRightCorner= '+';
	char downLeftCorner = '+';
	char downRightCorner = '+';
	char horizontal = '-';
	char horizontalDown = '+';
	char horizontalUp = '+';
	char vertical = '|';
	char verticalRight = '+';
	char verticalLeft = '+';
	char fourArm = '+';

	int heightPutChar = (diskHeight + 1) / 2;
	int spaceBeforeChar = (diskWidth + 1) / 2;
	int spaceAfterChar = diskWidth - spaceBeforeChar;

	//construct board string

	//header
	appendSame(output, ' ', 3);
	for(char i = 'a'; i <= 'h'; ++i){
		appendSame(output, ' ', spaceBeforeChar);
		output += i;
		appendSame(output, ' ', spaceAfterChar);
	}
	output += "\n";

	appendSame(output, ' ', 3);
	output += upLeftCorner;

	for(int i = 1; i <= 7; i++){
		appendSame(output, horizontal, diskWidth);
		output += horizontalDown;
	}
	appendSame(output, horizontal, diskWidth);
	output += upRightCorner;
	output += '\n';

	// loop
	for(int i = 0; i < 8; ++i){
		for(int j = 0; j < diskHeight; ++j){
			if(j == heightPutChar - 1){
				output += ' ';
				output += '1' + i;
				output += ' ';
			}else{
			 	appendSame(output, ' ', 3);
			}
			output += vertical;
			for(int k = 8 * i; k < 8 * i + 8; ++k){
				if(board[k] == 1) output += setting["black"].get_texture()[j];
				else if(board[k] == 2) output += setting["white"].get_texture()[j];
				else if(board[k] == 3) output += setting["fliplook"].get_texture()[j];
				else{
					//if move is to be shown (not in flipping process)
					if(playertoshowmove != 0){
						//if move is legal and show move is on
						if(setting["move"].get_bool() and flip(board, k, playertoshowmove).num != 0) output += setting["movelook"].get_texture()[j];
						else appendSame(output, ' ', diskWidth);
					}else{
						appendSame(output, ' ', diskWidth);
					}
				}
				output += vertical;
			}
			output += '\n';
		}
		if(i != 7){
			//not last row
			appendSame(output, ' ', 3);
			output += verticalRight;
			for(int j = 1; j <= 7; j++){
				appendSame(output, horizontal, diskWidth);
				output += fourArm;
			}
			appendSame(output, horizontal, diskWidth);
			output += verticalLeft;
			output += '\n';
		}else{
			appendSame(output, ' ', 3);
			output += downLeftCorner;
			for(int j = 1; j <= 7; j++){
				appendSame(output, horizontal, diskWidth);
				output += horizontalUp;
			}
			appendSame(output, horizontal, diskWidth);
			output += downRightCorner;
		}
	}
	output += "\n\n";
	printf("%s", output.c_str());
}

//display board and game information
void display(int board[64], int player, int no[2], int lastmove){
	int totalWidth = (diskWidth + 1) * MAXN + 4;
	int distanceRight = 2 * diskWidth + 11;
	int distanceLeft = 13;

	//display board
	boarddisplay(board, player);

	for(int i = 0; i < diskHeight; ++i){
		if(i == middleHeight){
			printf("last move: ");
			if(lastmove == -1) printf("- ");
			else printf("%c%d", 'a' + (lastmove % 8), lastmove / 8 + 1);
			space(totalWidth - distanceRight - distanceLeft);
			printf("%s x%2d  %s x%2d\n", setting["black"].get_texture()[i].c_str(), no[0],
										 setting["white"].get_texture()[i].c_str(), no[1]);
		}else{
			space(totalWidth - distanceRight);
			printf("%s      %s\n", setting["black"].get_texture()[i].c_str(), setting["white"].get_texture()[i].c_str());
		}
	}
}

//show flip animation!
void flipanimation(int board[64],int player,int position,int flipboard[64]){
     timeb time1,time2;
     int dboard[64]; //displayed board
     int i;
     
     //in this function the parameter playertoshowmove
     //in function 'boarddisplay' must be 0

     for(i=0;i<64;i++) dboard[i]=board[i];
     
     //display board+newly placed disk
     dboard[position]=player;
     clrscr();
     boarddisplay(dboard,0);
     
     ftime(&time1);
     do{
        ftime(&time2);
        }while(time2.time-time1.time+0.001*(time2.millitm-time1.millitm)< setting["fliptime"].get_float());
     
     //display flipping in progress
     for(i=0;i<64;i++){
                       if(dboard[i]!=flipboard[i]) dboard[i]=3; //determine position being flipped
                       }
     clrscr();
     boarddisplay(dboard,0);
     
     ftime(&time1);
     do{
        ftime(&time2);
        }while(time2.time-time1.time+0.001*(time2.millitm-time1.millitm)<setting["fliptime"].get_float());
     
     return;
}

//generate index array
void indexformob(int board[64]){
     mobindex[0]=2187*board[0]+729*board[1]+243*board[2]+81*board[3]+27*board[4]+9*board[5]+3*board[6]+board[7];
     mobindex[1]=2187*board[8]+729*board[9]+243*board[10]+81*board[11]+27*board[12]+9*board[13]+3*board[14]+board[15];
     mobindex[2]=2187*board[16]+729*board[17]+243*board[18]+81*board[19]+27*board[20]+9*board[21]+3*board[22]+board[23];
     mobindex[3]=2187*board[24]+729*board[25]+243*board[26]+81*board[27]+27*board[28]+9*board[29]+3*board[30]+board[31];
     mobindex[4]=2187*board[32]+729*board[33]+243*board[34]+81*board[35]+27*board[36]+9*board[37]+3*board[38]+board[39];
     mobindex[5]=2187*board[40]+729*board[41]+243*board[42]+81*board[43]+27*board[44]+9*board[45]+3*board[46]+board[47];
     mobindex[6]=2187*board[48]+729*board[49]+243*board[50]+81*board[51]+27*board[52]+9*board[53]+3*board[54]+board[55];
     mobindex[7]=2187*board[56]+729*board[57]+243*board[58]+81*board[59]+27*board[60]+9*board[61]+3*board[62]+board[63];
     mobindex[8]=2187*board[0]+729*board[8]+243*board[16]+81*board[24]+27*board[32]+9*board[40]+3*board[48]+board[56];
     mobindex[9]=2187*board[1]+729*board[9]+243*board[17]+81*board[25]+27*board[33]+9*board[41]+3*board[49]+board[57];
     mobindex[10]=2187*board[2]+729*board[10]+243*board[18]+81*board[26]+27*board[34]+9*board[42]+3*board[50]+board[58];
     mobindex[11]=2187*board[3]+729*board[11]+243*board[19]+81*board[27]+27*board[35]+9*board[43]+3*board[51]+board[59];
     mobindex[12]=2187*board[4]+729*board[12]+243*board[20]+81*board[28]+27*board[36]+9*board[44]+3*board[52]+board[60];
     mobindex[13]=2187*board[5]+729*board[13]+243*board[21]+81*board[29]+27*board[37]+9*board[45]+3*board[53]+board[61];
     mobindex[14]=2187*board[6]+729*board[14]+243*board[22]+81*board[30]+27*board[38]+9*board[46]+3*board[54]+board[62];
     mobindex[15]=2187*board[7]+729*board[15]+243*board[23]+81*board[31]+27*board[39]+9*board[47]+3*board[55]+board[63];
     mobindex[16]=9*board[40]+3*board[49]+board[58];
     mobindex[17]=27*board[32]+9*board[41]+3*board[50]+board[59];
     mobindex[18]=81*board[24]+27*board[33]+9*board[42]+3*board[51]+board[60];
     mobindex[19]=243*board[16]+81*board[25]+27*board[34]+9*board[43]+3*board[52]+board[61];
     mobindex[20]=729*board[8]+243*board[17]+81*board[26]+27*board[35]+9*board[44]+3*board[53]+board[62];
     mobindex[21]=2187*board[0]+729*board[9]+243*board[18]+81*board[27]+27*board[36]+9*board[45]+3*board[54]+board[63];
     mobindex[22]=729*board[1]+243*board[10]+81*board[19]+27*board[28]+9*board[37]+3*board[46]+board[55];
     mobindex[23]=243*board[2]+81*board[11]+27*board[20]+9*board[29]+3*board[38]+board[47];
     mobindex[24]=81*board[3]+27*board[12]+9*board[21]+3*board[30]+board[39];
     mobindex[25]=27*board[4]+9*board[13]+3*board[22]+board[31];
     mobindex[26]=9*board[5]+3*board[14]+board[23];
     mobindex[27]=9*board[16]+3*board[9]+board[2];
     mobindex[28]=27*board[24]+9*board[17]+3*board[10]+board[3];
     mobindex[29]=81*board[32]+27*board[25]+9*board[18]+3*board[11]+board[4];
     mobindex[30]=243*board[40]+81*board[33]+27*board[26]+9*board[19]+3*board[12]+board[5];
     mobindex[31]=729*board[48]+243*board[41]+81*board[34]+27*board[27]+9*board[20]+3*board[13]+board[6];
     mobindex[32]=2187*board[56]+729*board[49]+243*board[42]+81*board[35]+27*board[28]+9*board[21]+3*board[14]+board[7];
     mobindex[33]=729*board[57]+243*board[50]+81*board[43]+27*board[36]+9*board[29]+3*board[22]+board[15];
     mobindex[34]=243*board[58]+81*board[51]+27*board[44]+9*board[37]+3*board[30]+board[23];
     mobindex[35]=81*board[59]+27*board[52]+9*board[45]+3*board[38]+board[31];
     mobindex[36]=27*board[60]+9*board[53]+3*board[46]+board[39];
     mobindex[37]=9*board[61]+3*board[54]+board[47];
}

//compute mobility: number of legal moves
int mobility(int player){
    bool mobboard[64]={};
    int i;
    int index;
    
    player--;
    
    //horizontal ---------------------------------------------------------------
    
    index=mobindex[0];
    if(mobtable[index][player][0]) mobboard[0] = true;
    if(mobtable[index][player][1]) mobboard[1] = true;
    if(mobtable[index][player][2]) mobboard[2] = true;
    if(mobtable[index][player][3]) mobboard[3] = true;
    if(mobtable[index][player][4]) mobboard[4] = true;
    if(mobtable[index][player][5]) mobboard[5] = true;
    if(mobtable[index][player][6]) mobboard[6] = true;
    if(mobtable[index][player][7]) mobboard[7] = true;
    
    index=mobindex[1];
    if(mobtable[index][player][0]) mobboard[8] = true;
    if(mobtable[index][player][1]) mobboard[9] = true;
    if(mobtable[index][player][2]) mobboard[10] = true;
    if(mobtable[index][player][3]) mobboard[11] = true;
    if(mobtable[index][player][4]) mobboard[12] = true;
    if(mobtable[index][player][5]) mobboard[13] = true;
    if(mobtable[index][player][6]) mobboard[14] = true;
    if(mobtable[index][player][7]) mobboard[15] = true;
    
    index=mobindex[2];
    if(mobtable[index][player][0]) mobboard[16] = true;
    if(mobtable[index][player][1]) mobboard[17] = true;
    if(mobtable[index][player][2]) mobboard[18] = true;
    if(mobtable[index][player][3]) mobboard[19] = true;
    if(mobtable[index][player][4]) mobboard[20] = true;
    if(mobtable[index][player][5]) mobboard[21] = true;
    if(mobtable[index][player][6]) mobboard[22] = true;
    if(mobtable[index][player][7]) mobboard[23] = true;
    
    index=mobindex[3];
    if(mobtable[index][player][0]) mobboard[24] = true;
    if(mobtable[index][player][1]) mobboard[25] = true;
    if(mobtable[index][player][2]) mobboard[26] = true;
    if(mobtable[index][player][3]) mobboard[27] = true;
    if(mobtable[index][player][4]) mobboard[28] = true;
    if(mobtable[index][player][5]) mobboard[29] = true;
    if(mobtable[index][player][6]) mobboard[30] = true;
    if(mobtable[index][player][7]) mobboard[31] = true;
    
    index=mobindex[4];
    if(mobtable[index][player][0]) mobboard[32] = true;
    if(mobtable[index][player][1]) mobboard[33] = true;
    if(mobtable[index][player][2]) mobboard[34] = true;
    if(mobtable[index][player][3]) mobboard[35] = true;
    if(mobtable[index][player][4]) mobboard[36] = true;
    if(mobtable[index][player][5]) mobboard[37] = true;
    if(mobtable[index][player][6]) mobboard[38] = true;
    if(mobtable[index][player][7]) mobboard[39] = true;
    
    index=mobindex[5];
    if(mobtable[index][player][0]) mobboard[40] = true;
    if(mobtable[index][player][1]) mobboard[41] = true;
    if(mobtable[index][player][2]) mobboard[42] = true;
    if(mobtable[index][player][3]) mobboard[43] = true;
    if(mobtable[index][player][4]) mobboard[44] = true;
    if(mobtable[index][player][5]) mobboard[45] = true;
    if(mobtable[index][player][6]) mobboard[46] = true;
    if(mobtable[index][player][7]) mobboard[47] = true;
    
    index=mobindex[6];
    if(mobtable[index][player][0]) mobboard[48] = true;
    if(mobtable[index][player][1]) mobboard[49] = true;
    if(mobtable[index][player][2]) mobboard[50] = true;
    if(mobtable[index][player][3]) mobboard[51] = true;
    if(mobtable[index][player][4]) mobboard[52] = true;
    if(mobtable[index][player][5]) mobboard[53] = true;
    if(mobtable[index][player][6]) mobboard[54] = true;
    if(mobtable[index][player][7]) mobboard[55] = true;
    
    index=mobindex[7];
    if(mobtable[index][player][0]) mobboard[56] = true;
    if(mobtable[index][player][1]) mobboard[57] = true;
    if(mobtable[index][player][2]) mobboard[58] = true;
    if(mobtable[index][player][3]) mobboard[59] = true;
    if(mobtable[index][player][4]) mobboard[60] = true;
    if(mobtable[index][player][5]) mobboard[61] = true;
    if(mobtable[index][player][6]) mobboard[62] = true;
    if(mobtable[index][player][7]) mobboard[63] = true;
    
    //vertical -----------------------------------------------------------------
    
    index=mobindex[8];
    if(mobtable[index][player][0]) mobboard[0] = true;
    if(mobtable[index][player][1]) mobboard[8] = true;
    if(mobtable[index][player][2]) mobboard[16] = true;
    if(mobtable[index][player][3]) mobboard[24] = true;
    if(mobtable[index][player][4]) mobboard[32] = true;
    if(mobtable[index][player][5]) mobboard[40] = true;
    if(mobtable[index][player][6]) mobboard[48] = true;
    if(mobtable[index][player][7]) mobboard[56] = true;
    
    index=mobindex[9];
    if(mobtable[index][player][0]) mobboard[1] = true;
    if(mobtable[index][player][1]) mobboard[9] = true;
    if(mobtable[index][player][2]) mobboard[17] = true;
    if(mobtable[index][player][3]) mobboard[25] = true;
    if(mobtable[index][player][4]) mobboard[33] = true;
    if(mobtable[index][player][5]) mobboard[41] = true;
    if(mobtable[index][player][6]) mobboard[49] = true;
    if(mobtable[index][player][7]) mobboard[57] = true;
    
    index=mobindex[10];
    if(mobtable[index][player][0]) mobboard[2] = true;
    if(mobtable[index][player][1]) mobboard[10] = true;
    if(mobtable[index][player][2]) mobboard[18] = true;
    if(mobtable[index][player][3]) mobboard[26] = true;
    if(mobtable[index][player][4]) mobboard[34] = true;
    if(mobtable[index][player][5]) mobboard[42] = true;
    if(mobtable[index][player][6]) mobboard[50] = true;
    if(mobtable[index][player][7]) mobboard[58] = true;
    
    index=mobindex[11];
    if(mobtable[index][player][0]) mobboard[3] = true;
    if(mobtable[index][player][1]) mobboard[11] = true;
    if(mobtable[index][player][2]) mobboard[19] = true;
    if(mobtable[index][player][3]) mobboard[27] = true;
    if(mobtable[index][player][4]) mobboard[35] = true;
    if(mobtable[index][player][5]) mobboard[43] = true;
    if(mobtable[index][player][6]) mobboard[51] = true;
    if(mobtable[index][player][7]) mobboard[59] = true;
    
    index=mobindex[12];
    if(mobtable[index][player][0]) mobboard[4] = true;
    if(mobtable[index][player][1]) mobboard[12] = true;
    if(mobtable[index][player][2]) mobboard[20] = true;
    if(mobtable[index][player][3]) mobboard[28] = true;
    if(mobtable[index][player][4]) mobboard[36] = true;
    if(mobtable[index][player][5]) mobboard[44] = true;
    if(mobtable[index][player][6]) mobboard[52] = true;
    if(mobtable[index][player][7]) mobboard[60] = true;
    
    index=mobindex[13];
    if(mobtable[index][player][0]) mobboard[5] = true;
    if(mobtable[index][player][1]) mobboard[13] = true;
    if(mobtable[index][player][2]) mobboard[21] = true;
    if(mobtable[index][player][3]) mobboard[29] = true;
    if(mobtable[index][player][4]) mobboard[37] = true;
    if(mobtable[index][player][5]) mobboard[45] = true;
    if(mobtable[index][player][6]) mobboard[53] = true;
    if(mobtable[index][player][7]) mobboard[61] = true;
    
    index=mobindex[14];
    if(mobtable[index][player][0]) mobboard[6] = true;
    if(mobtable[index][player][1]) mobboard[14] = true;
    if(mobtable[index][player][2]) mobboard[22] = true;
    if(mobtable[index][player][3]) mobboard[30] = true;
    if(mobtable[index][player][4]) mobboard[38] = true;
    if(mobtable[index][player][5]) mobboard[46] = true;
    if(mobtable[index][player][6]) mobboard[54] = true;
    if(mobtable[index][player][7]) mobboard[62] = true;
    
    index=mobindex[15];
    if(mobtable[index][player][0]) mobboard[7] = true;
    if(mobtable[index][player][1]) mobboard[15] = true;
    if(mobtable[index][player][2]) mobboard[23] = true;
    if(mobtable[index][player][3]) mobboard[31] = true;
    if(mobtable[index][player][4]) mobboard[39] = true;
    if(mobtable[index][player][5]) mobboard[47] = true;
    if(mobtable[index][player][6]) mobboard[55] = true;
    if(mobtable[index][player][7]) mobboard[63] = true;
    
    //diagonal \ ---------------------------------------------------------------
    
    //one
    index=mobindex[16];
    if(mobtable[index][player][5]) mobboard[40] = true;
    //49 cannot flip
    if(mobtable[index][player][7]) mobboard[58] = true;
    
    //two
    index=mobindex[17];
    if(mobtable[index][player][4]) mobboard[32] = true;
    if(mobtable[index][player][5]) mobboard[41] = true;
    if(mobtable[index][player][6]) mobboard[50] = true;
    if(mobtable[index][player][7]) mobboard[59] = true;
    
    //three
    index=mobindex[18];
    if(mobtable[index][player][3]) mobboard[24] = true;
    if(mobtable[index][player][4]) mobboard[33] = true;
    if(mobtable[index][player][5]) mobboard[42] = true;
    if(mobtable[index][player][6]) mobboard[51] = true;
    if(mobtable[index][player][7]) mobboard[60] = true;
    
    //four
    index=mobindex[19];
    if(mobtable[index][player][2]) mobboard[16] = true;
    if(mobtable[index][player][3]) mobboard[25] = true;
    if(mobtable[index][player][4]) mobboard[34] = true;
    if(mobtable[index][player][5]) mobboard[43] = true;
    if(mobtable[index][player][6]) mobboard[52] = true;
    if(mobtable[index][player][7]) mobboard[61] = true;
    
    //five
    index=mobindex[20];
    if(mobtable[index][player][1]) mobboard[8] = true;
    if(mobtable[index][player][2]) mobboard[17] = true;
    if(mobtable[index][player][3]) mobboard[26] = true;
    if(mobtable[index][player][4]) mobboard[35] = true;
    if(mobtable[index][player][5]) mobboard[44] = true;
    if(mobtable[index][player][6]) mobboard[53] = true;
    if(mobtable[index][player][7]) mobboard[62] = true;
    
    //six
    index=mobindex[21];
    if(mobtable[index][player][0]) mobboard[0] = true;
    if(mobtable[index][player][1]) mobboard[9] = true;
    if(mobtable[index][player][2]) mobboard[18] = true;
    if(mobtable[index][player][3]) mobboard[27] = true;
    if(mobtable[index][player][4]) mobboard[36] = true;
    if(mobtable[index][player][5]) mobboard[45] = true;
    if(mobtable[index][player][6]) mobboard[54] = true;
    if(mobtable[index][player][7]) mobboard[63] = true;
    
    //seven
    index=mobindex[22];
    if(mobtable[index][player][1]) mobboard[1] = true;
    if(mobtable[index][player][2]) mobboard[10] = true;
    if(mobtable[index][player][3]) mobboard[19] = true;
    if(mobtable[index][player][4]) mobboard[28] = true;
    if(mobtable[index][player][5]) mobboard[37] = true;
    if(mobtable[index][player][6]) mobboard[46] = true;
    if(mobtable[index][player][7]) mobboard[55] = true;
    
    //eight
    index=mobindex[23];
    if(mobtable[index][player][2]) mobboard[2] = true;
    if(mobtable[index][player][3]) mobboard[11] = true;
    if(mobtable[index][player][4]) mobboard[20] = true;
    if(mobtable[index][player][5]) mobboard[29] = true;
    if(mobtable[index][player][6]) mobboard[38] = true;
    if(mobtable[index][player][7]) mobboard[47] = true;
    
    //nine
    index=mobindex[24];
    if(mobtable[index][player][3]) mobboard[3] = true;
    if(mobtable[index][player][4]) mobboard[12] = true;
    if(mobtable[index][player][5]) mobboard[21] = true;
    if(mobtable[index][player][6]) mobboard[30] = true;
    if(mobtable[index][player][7]) mobboard[39] = true;
    
    //ten
    index=mobindex[25];
    if(mobtable[index][player][4]) mobboard[4] = true;
    if(mobtable[index][player][5]) mobboard[13] = true;
    if(mobtable[index][player][6]) mobboard[22] = true;
    if(mobtable[index][player][7]) mobboard[31] = true;
    
    //eleven
    index=mobindex[26];
    if(mobtable[index][player][5]) mobboard[5] = true;
    //14 cannot flip
    if(mobtable[index][player][7]) mobboard[23] = true;

    //diagonal / ---------------------------------------------------------------
    
    //one
    index=mobindex[27];
    if(mobtable[index][player][5]) mobboard[16] = true;
    //9 cannot flip
    if(mobtable[index][player][7]) mobboard[2] = true;
    
    //two
    index=mobindex[28];
    if(mobtable[index][player][4]) mobboard[24] = true;
    if(mobtable[index][player][5]) mobboard[17] = true;
    if(mobtable[index][player][6]) mobboard[10] = true;
    if(mobtable[index][player][7]) mobboard[3] = true;
    
    //three
    index=mobindex[29];
    if(mobtable[index][player][3]) mobboard[32] = true;
    if(mobtable[index][player][4]) mobboard[25] = true;
    if(mobtable[index][player][5]) mobboard[18] = true;
    if(mobtable[index][player][6]) mobboard[11] = true;
    if(mobtable[index][player][7]) mobboard[4] = true;
    
    //four
    index=mobindex[30];
    if(mobtable[index][player][2]) mobboard[40] = true;
    if(mobtable[index][player][3]) mobboard[33] = true;
    if(mobtable[index][player][4]) mobboard[26] = true;
    if(mobtable[index][player][5]) mobboard[19] = true;
    if(mobtable[index][player][6]) mobboard[12] = true;
    if(mobtable[index][player][7]) mobboard[5] = true;
    
    //five
    index=mobindex[31];
    if(mobtable[index][player][1]) mobboard[48] = true;
    if(mobtable[index][player][2]) mobboard[41] = true;
    if(mobtable[index][player][3]) mobboard[34] = true;
    if(mobtable[index][player][4]) mobboard[27] = true;
    if(mobtable[index][player][5]) mobboard[20] = true;
    if(mobtable[index][player][6]) mobboard[13] = true;
    if(mobtable[index][player][7]) mobboard[6] = true;
    
    //six
    index=mobindex[32];
    if(mobtable[index][player][0]) mobboard[56] = true;
    if(mobtable[index][player][1]) mobboard[49] = true;
    if(mobtable[index][player][2]) mobboard[42] = true;
    if(mobtable[index][player][3]) mobboard[35] = true;
    if(mobtable[index][player][4]) mobboard[28] = true;
    if(mobtable[index][player][5]) mobboard[21] = true;
    if(mobtable[index][player][6]) mobboard[14] = true;
    if(mobtable[index][player][7]) mobboard[7] = true;
    
    //seven
    index=mobindex[33];
    if(mobtable[index][player][1]) mobboard[57] = true;
    if(mobtable[index][player][2]) mobboard[50] = true;
    if(mobtable[index][player][3]) mobboard[43] = true;
    if(mobtable[index][player][4]) mobboard[36] = true;
    if(mobtable[index][player][5]) mobboard[29] = true;
    if(mobtable[index][player][6]) mobboard[22] = true;
    if(mobtable[index][player][7]) mobboard[15] = true;
    
    //eight
    index=mobindex[34];
    if(mobtable[index][player][2]) mobboard[58] = true;
    if(mobtable[index][player][3]) mobboard[51] = true;
    if(mobtable[index][player][4]) mobboard[44] = true;
    if(mobtable[index][player][5]) mobboard[37] = true;
    if(mobtable[index][player][6]) mobboard[30] = true;
    if(mobtable[index][player][7]) mobboard[23] = true;
    
    //nine
    index=mobindex[35];
    if(mobtable[index][player][3]) mobboard[59] = true;
    if(mobtable[index][player][4]) mobboard[52] = true;
    if(mobtable[index][player][5]) mobboard[45] = true;
    if(mobtable[index][player][6]) mobboard[38] = true;
    if(mobtable[index][player][7]) mobboard[31] = true;
    
    //ten
    index=mobindex[36];
    if(mobtable[index][player][4]) mobboard[60] = true;
    if(mobtable[index][player][5]) mobboard[53] = true;
    if(mobtable[index][player][6]) mobboard[46] = true;
    if(mobtable[index][player][7]) mobboard[39] = true;
    
    //eleven
    index=mobindex[37];
    if(mobtable[index][player][5]) mobboard[61] = true;
    //54 cannot flip
    if(mobtable[index][player][7]) mobboard[47] = true;    
    
    //summation --------------------------------------------
    
    index=0;
    for(i=0;i<64;i++){
                      if(mobboard[i]) index++;
                      }
    return index;
    
}

//return potential mobility difference (player1-player2)
//potential mobility of player 1 = no. of player 2's frontier disks
//= no. of player 2's disks adjacent to empty squares (excluding edges)
int pmobility(int board[64]){
    int pmobility=0;
    int position=8; //start at 8
    
    qlp:
    position++;
    if(position==55) return pmobility;
    if(position%8==7) position+=2;
    
    if(board[position]==1){
                           //for player 1's disks
                           if(!board[position-9]){pmobility--; goto qlp;} //upleft
                           if(!board[position-8]){pmobility--; goto qlp;} //up
                           if(!board[position-7]){pmobility--; goto qlp;} //upright
                           if(!board[position-1]){pmobility--; goto qlp;} //left
                           if(!board[position+1]){pmobility--; goto qlp;} //right
                           if(!board[position+7]){pmobility--; goto qlp;} //downleft
                           if(!board[position+8]){pmobility--; goto qlp;} //down
                           if(!board[position+9]){pmobility--; goto qlp;} //downright
                           }
    if(board[position]==2){
                           //for player 2's disks
                           if(!board[position-9]){pmobility++; goto qlp;} //upleft
                           if(!board[position-8]){pmobility++; goto qlp;} //up
                           if(!board[position-7]){pmobility++; goto qlp;} //upright
                           if(!board[position-1]){pmobility++; goto qlp;} //left
                           if(!board[position+1]){pmobility++; goto qlp;} //right
                           if(!board[position+7]){pmobility++; goto qlp;} //downleft
                           if(!board[position+8]){pmobility++; goto qlp;} //down
                           if(!board[position+9]){pmobility++; goto qlp;} //downright
                           }
    goto qlp;
}

//combines function corner, xsquare and csquare!
int triplesq(int board[64],int corner,int xsquare,int csquare){
    int value=0;
    
    //upleft corner
    if(board[0]==0){
                    if(board[9]==1) value-=xsquare;
                    if(board[9]==2) value+=xsquare;
                    if(board[1]==1) value-=csquare;
                    if(board[1]==2) value+=csquare;
                    if(board[8]==1) value-=csquare;
                    if(board[8]==2) value+=csquare;
                    }
    else if(board[0]==1) value+=corner;
    else value-=corner;
    
    //upright corner
    if(board[7]==0){
                    if(board[14]==1) value-=xsquare;
                    if(board[14]==2) value+=xsquare;
                    if(board[6]==1) value-=csquare;
                    if(board[6]==2) value+=csquare;
                    if(board[15]==1) value-=csquare;
                    if(board[15]==2) value+=csquare;
                    }
    else if(board[7]==1) value+=corner;
    else value-=corner;
    
    //downleft corner
    if(board[56]==0){
                    if(board[49]==1) value-=xsquare;
                    if(board[49]==2) value+=xsquare;
                    if(board[48]==1) value-=csquare;
                    if(board[48]==2) value+=csquare;
                    if(board[57]==1) value-=csquare;
                    if(board[57]==2) value+=csquare;
                    }
    else if(board[56]==1) value+=corner;
    else value-=corner;
    
    //downright corner
    if(board[63]==0){
                    if(board[54]==1) value-=xsquare;
                    if(board[54]==2) value+=xsquare;
                    if(board[55]==1) value-=csquare;
                    if(board[55]==2) value+=csquare;
                    if(board[62]==1) value-=csquare;
                    if(board[62]==2) value+=csquare;
                    }
    else if(board[63]==1) value+=corner;
    else value-=corner;
    
    //if new csquare - counts -xxxxxx- only once
    if(wnew==1){
                if(mobindex[0]==1092) value+=csquare;
                if(mobindex[0]==2184) value-=csquare;
                if(mobindex[7]==1092) value+=csquare;
                if(mobindex[7]==2184) value-=csquare;
                if(mobindex[8]==1092) value+=csquare;
                if(mobindex[8]==2184) value-=csquare;
                if(mobindex[15]==1092) value+=csquare;
                if(mobindex[15]==2184) value-=csquare;
                }
    
    return value;
}

//compute values for edge configurations
//based on a precomputed array!
float edgevalue(int board[64],int player){
      return edgeconfig[mobindex[0]][player-1]+ //north edge
             edgeconfig[mobindex[7]][player-1]+ //south edge
             edgeconfig[mobindex[8]][player-1]+ //west edge
             edgeconfig[mobindex[15]][player-1]; //east edge
}

//compute difference of number of (some) stable disks (player1-player2)
//only count disks protected by corners up to 4 layers
int stabledisk(int board[64]){
    int stable[64]={};
    int value=0;
    int a;
    int colour;
    
    //second layer =============================================================
    
    //north edge ---------------------------------------------------------------
    
    if(board[0]!=0){
                    colour=board[0];
                    a=0;
                    if(board[1]==colour and board[2]==colour and board[8]==colour and board[9]==colour){
                                        stable[9]=colour;
                                        a++;
                                        if(board[3]==colour and board[10]==colour){
                                                            stable[10]=colour;
                                                            a++;
                                                            if(board[4]==colour and board[11]==colour){
                                                                                stable[11]=colour;
                                                                                a++;
                                                                                if(board[5]==colour and board[12]==colour){
                                                                                                    stable[12]=colour;
                                                                                                    a++;
                                                                                                    if(board[6]==colour and board[13]==colour){
                                                                                                                        stable[13]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[7]==colour and board[14]==colour){
                                                                                                                                            stable[14]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(board[7]!=0){
                    colour=board[7];
                    a=0;
                    if(board[5]==colour and board[6]==colour and board[14]==colour and board[15]==colour and stable[14]==0){
                                        stable[14]=colour;
                                        a++;
                                        if(board[4]==colour and board[13]==colour and stable[13]==0){
                                                            stable[13]=colour;
                                                            a++;
                                                            if(board[3]==colour and board[12]==colour and stable[12]==0){
                                                                                stable[12]=colour;
                                                                                a++;
                                                                                if(board[2]==colour and board[11]==colour and stable[11]==0){
                                                                                                    stable[11]=colour;
                                                                                                    a++;
                                                                                                    if(board[1]==colour and board[10]==colour and stable[10]==0){
                                                                                                                        stable[10]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[0]==colour and board[9]==colour and stable[9]==0){
                                                                                                                                            stable[9]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //west edge ----------------------------------------------------------------
    
    if(board[0]!=0){
                    colour=board[0];
                    a=0;
                    if(board[1]==colour and board[8]==colour and board[9]==colour and board[16]==colour){
                                        if(stable[9]==0){
                                                         stable[9]=colour;
                                                         a++;
                                                         }
                                        if(board[17]==colour and board[24]==colour){
                                                            stable[17]=colour;
                                                            a++;
                                                            if(board[25]==colour and board[32]==colour){
                                                                                stable[25]=colour;
                                                                                a++;
                                                                                if(board[33]==colour and board[40]==colour){
                                                                                                    stable[33]=colour;
                                                                                                    a++;
                                                                                                    if(board[41]==colour and board[48]==colour){
                                                                                                                        stable[41]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[49]==colour and board[56]==colour){
                                                                                                                                            stable[49]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(board[56]!=0){
                    colour=board[56];
                    a=0;
                    if(board[40]==colour and board[48]==colour and board[49]==colour and board[57]==colour and stable[49]==0){
                                        stable[49]=colour;
                                        a++;
                                        if(board[32]==colour and board[41]==colour and stable[41]==0){
                                                            stable[41]=colour;
                                                            a++;
                                                            if(board[24]==colour and board[33]==colour and stable[33]==0){
                                                                                stable[33]=colour;
                                                                                a++;
                                                                                if(board[16]==colour and board[25]==colour and stable[25]==0){
                                                                                                    stable[25]=colour;
                                                                                                    a++;
                                                                                                    if(board[8]==colour and board[17]==colour and stable[17]==0){
                                                                                                                        stable[17]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[0]==colour and board[9]==colour and stable[9]==0){
                                                                                                                                            stable[9]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //east edge ----------------------------------------------------------------
    
    if(board[7]!=0){
                    colour=board[7];
                    a=0;
                    if(board[6]==colour and board[14]==colour and board[15]==colour and board[23]==colour){
                                        if(stable[14]==0){
                                                         stable[14]=colour;
                                                         a++;
                                                         }
                                        if(board[22]==colour and board[31]==colour){
                                                            stable[22]=colour;
                                                            a++;
                                                            if(board[30]==colour and board[39]==colour){
                                                                                stable[30]=colour;
                                                                                a++;
                                                                                if(board[38]==colour and board[47]==colour){
                                                                                                    stable[38]=colour;
                                                                                                    a++;
                                                                                                    if(board[46]==colour and board[55]==colour){
                                                                                                                        stable[46]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[54]==colour and board[63]==colour){
                                                                                                                                            stable[54]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(board[63]!=0){
                    colour=board[63];
                    a=0;
                    if(board[47]==colour and board[54]==colour and board[55]==colour and board[62]==colour and stable[54]==0){
                                        stable[54]=colour;
                                        a++;
                                        if(board[39]==colour and board[46]==colour and stable[46]==0){
                                                            stable[46]=colour;
                                                            a++;
                                                            if(board[31]==colour and board[38]==colour and stable[38]==0){
                                                                                stable[38]=colour;
                                                                                a++;
                                                                                if(board[23]==colour and board[30]==colour and stable[30]==0){
                                                                                                    stable[30]=colour;
                                                                                                    a++;
                                                                                                    if(board[15]==colour and board[22]==colour and stable[22]==0){
                                                                                                                        stable[22]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[7]==colour and board[14]==colour and stable[14]==0){
                                                                                                                                            stable[14]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }

    
    //south edge ---------------------------------------------------------------
    
    if(board[56]!=0){
                    colour=board[56];
                    a=0;
                    if(board[48]==colour and board[49]==colour and board[57]==colour and board[58]==colour){
                                        if(stable[49]==0){
                                                         stable[49]=colour;
                                                         a++;
                                                         }
                                        if(board[50]==colour and board[59]==colour){
                                                            stable[50]=colour;
                                                            a++;
                                                            if(board[51]==colour and board[60]==colour){
                                                                                stable[51]=colour;
                                                                                a++;
                                                                                if(board[52]==colour and board[61]==colour){
                                                                                                    stable[52]=colour;
                                                                                                    a++;
                                                                                                    if(board[53]==colour and board[62]==colour){
                                                                                                                        stable[53]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[54]==colour and board[63]==colour and stable[54]==0){
                                                                                                                                            stable[54]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(board[63]!=0){
                    colour=board[63];
                    a=0;
                    if(board[54]==colour and board[55]==colour and board[61]==colour and board[62]==colour){
                                        if(stable[54]==0){
                                                          stable[54]=colour;
                                                          a++;
                                                          }
                                        if(board[53]==colour and board[60]==colour and stable[53]==0){
                                                            stable[53]=colour;
                                                            a++;
                                                            if(board[52]==colour and board[59]==colour and stable[52]==0){
                                                                                stable[52]=colour;
                                                                                a++;
                                                                                if(board[51]==colour and board[58]==colour and stable[51]==0){
                                                                                                    stable[51]=colour;
                                                                                                    a++;
                                                                                                    if(board[50]==colour and board[57]==colour and stable[50]==0){
                                                                                                                        stable[50]=colour;
                                                                                                                        a++;
                                                                                                                        if(board[49]==colour and board[56]==colour and stable[49]==0){
                                                                                                                                            stable[49]=colour;
                                                                                                                                            a++;
                                                                                                                                            }
                                                                                                                        }
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //third layer ==============================================================
    
    //north edge ---------------------------------------------------------------
    
    if(stable[9]!=0){
                    colour=stable[9];
                    a=0;
                    if(stable[10]==colour and stable[11]==colour and stable[17]==colour and board[18]==colour){
                                        stable[18]=colour;
                                        a++;
                                        if(stable[12]==colour and board[19]==colour){
                                                            stable[19]=colour;
                                                            a++;
                                                            if(stable[13]==colour and board[20]==colour){
                                                                                stable[20]=colour;
                                                                                a++;
                                                                                if(stable[14]==colour and board[21]==colour){
                                                                                                    stable[21]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[14]!=0){
                    colour=stable[14];
                    a=0;
                    if(stable[12]==colour and stable[13]==colour and stable[22]==colour and board[21]==colour and stable[21]==0){
                                        stable[21]=colour;
                                        a++;
                                        if(stable[11]==colour and board[20]==colour and stable[20]==0){
                                                            stable[20]=colour;
                                                            a++;
                                                            if(stable[10]==colour and board[19]==colour and stable[19]==0){
                                                                                stable[19]=colour;
                                                                                a++;
                                                                                if(stable[9]==colour and board[18]==colour and stable[18]==0){
                                                                                                    stable[18]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //west edge ----------------------------------------------------------------
    
    if(stable[9]!=0){
                    colour=stable[9];
                    a=0;
                    if(stable[10]==colour and stable[17]==colour and stable[25]==colour and board[18]==colour){
                                        if(stable[18]==0){
                                                          stable[18]=colour;
                                                          a++;
                                                          }
                                        if(stable[33]==colour and board[26]==colour){
                                                            stable[26]=colour;
                                                            a++;
                                                            if(stable[41]==colour and board[34]==colour){
                                                                                stable[34]=colour;
                                                                                a++;
                                                                                if(stable[49]==colour and board[42]==colour){
                                                                                                    stable[42]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[49]!=0){
                    colour=stable[49];
                    a=0;
                    if(stable[33]==colour and stable[41]==colour and stable[50]==colour and board[42]==colour and stable[42]==0){
                                        stable[42]=colour;
                                        a++;
                                        if(stable[25]==colour and board[34]==colour and stable[34]==0){
                                                            stable[34]=colour;
                                                            a++;
                                                            if(stable[17]==colour and board[26]==colour and stable[26]==0){
                                                                                stable[26]=colour;
                                                                                a++;
                                                                                if(stable[9]==colour and board[18]==colour and stable[18]==0){
                                                                                                    stable[18]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }

    //east edge ----------------------------------------------------------------
    
    if(stable[14]!=0){
                    colour=stable[14];
                    a=0;
                    if(stable[13]==colour and stable[22]==colour and stable[30]==colour and board[21]==colour){
                                        if(stable[21]==0){
                                                          stable[21]=colour;
                                                          a++;
                                                          }
                                        if(stable[38]==colour and board[29]==colour){
                                                            stable[29]=colour;
                                                            a++;
                                                            if(stable[46]==colour and board[37]==colour){
                                                                                stable[37]=colour;
                                                                                a++;
                                                                                if(stable[54]==colour and board[45]==colour){
                                                                                                    stable[45]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[54]!=0){
                    colour=stable[54];
                    a=0;
                    if(stable[38]==colour and stable[46]==colour and stable[53]==colour and board[45]==colour and stable[45]==0){
                                        stable[45]=colour;
                                        a++;
                                        if(stable[30]==colour and board[37]==colour and stable[37]==0){
                                                            stable[37]=colour;
                                                            a++;
                                                            if(stable[22]==colour and board[29]==colour and stable[29]==0){
                                                                                stable[29]=colour;
                                                                                a++;
                                                                                if(stable[14]==colour and board[21]==colour and stable[21]==0){
                                                                                                    stable[21]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //south edge ---------------------------------------------------------------
    
    if(stable[49]!=0){
                    colour=stable[49];
                    a=0;
                    if(stable[41]==colour and stable[50]==colour and stable[51]==colour and board[42]==colour){
                                        if(stable[42]==0){
                                                          stable[42]=colour;
                                                          a++;
                                                          }
                                        if(stable[52]==colour and board[43]==colour){
                                                            stable[43]=colour;
                                                            a++;
                                                            if(stable[53]==colour and board[44]==colour){
                                                                                stable[44]=colour;
                                                                                a++;
                                                                                if(stable[54]==colour and board[45]==colour and stable[45]==0){
                                                                                                    stable[45]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[54]!=0){
                    colour=stable[54];
                    a=0;
                    if(stable[46]==colour and stable[52]==colour and stable[53]==colour and board[45]==colour){
                                        if(stable[45]==0){
                                                          stable[45]=colour;
                                                          a++;
                                                          }
                                        if(stable[51]==colour and board[44]==colour and stable[44]==0){
                                                            stable[44]=colour;
                                                            a++;
                                                            if(stable[50]==colour and board[43]==colour and stable[43]==0){
                                                                                stable[43]=colour;
                                                                                a++;
                                                                                if(stable[49]==colour and board[42]==colour and stable[42]==0){
                                                                                                    stable[42]=colour;
                                                                                                    a++;
                                                                                                    }
                                                                                }
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //fourth layer =============================================================
    
    //north edge ---------------------------------------------------------------
    
    if(stable[18]!=0){
                    colour=stable[18];
                    a=0;
                    if(stable[19]==colour and stable[20]==colour and stable[26]==colour and board[27]==colour){
                                        stable[27]=colour;
                                        a++;
                                        if(stable[21]==colour and board[28]==colour){
                                                            stable[28]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[21]!=0){
                    colour=stable[21];
                    a=0;
                    if(stable[19]==colour and stable[20]==colour and stable[29]==colour and board[28]==colour and stable[28]==0){
                                        stable[28]=colour;
                                        a++;
                                        if(stable[18]==colour and board[27]==colour and stable[27]==0){
                                                            stable[27]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //west edge ----------------------------------------------------------------
    
    if(stable[18]!=0){
                    colour=stable[18];
                    a=0;
                    if(stable[19]==colour and stable[26]==colour and stable[34]==colour and board[27]==colour){
                                        if(stable[27]==0){
                                                          stable[27]=colour;
                                                          a++;
                                                          }
                                        if(stable[42]==colour and board[35]==colour){
                                                            stable[35]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[42]!=0){
                    colour=stable[42];
                    a=0;
                    if(stable[26]==colour and stable[34]==colour and stable[43]==colour and board[35]==colour and stable[35]==0){
                                        stable[35]=colour;
                                        a++;
                                        if(stable[18]==colour and board[27]==colour and stable[27]==0){
                                                            stable[27]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //east edge ----------------------------------------------------------------
    
    if(stable[21]!=0){
                    colour=stable[21];
                    a=0;
                    if(stable[20]==colour and stable[29]==colour and stable[37]==colour and board[28]==colour){
                                        if(stable[28]==0){
                                                          stable[28]=colour;
                                                          a++;
                                                          }
                                        if(stable[45]==colour and board[36]==colour){
                                                            stable[36]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[45]!=0){
                    colour=stable[45];
                    a=0;
                    if(stable[29]==colour and stable[37]==colour and stable[44]==colour and board[36]==colour and stable[36]==0){
                                        stable[36]=colour;
                                        a++;
                                        if(stable[21]==colour and board[28]==colour and stable[28]==0){
                                                            stable[28]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    //south edge ---------------------------------------------------------------
    
    if(stable[42]!=0){
                    colour=stable[42];
                    a=0;
                    if(stable[34]==colour and stable[43]==colour and stable[44]==colour and board[35]==colour){
                                        if(stable[35]==0){
                                                          stable[35]=colour;
                                                          a++;
                                                          }
                                        if(stable[45]==colour and board[36]==colour and stable[36]==0){
                                                            stable[36]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    if(stable[45]!=0){
                    colour=stable[45];
                    a=0;
                    if(stable[37]==colour and stable[43]==colour and stable[44]==colour and board[36]==colour){
                                        if(stable[36]==0){
                                                          stable[36]=colour;
                                                          a++;
                                                          }
                                        if(stable[42]==colour and board[35]==colour and stable[35]==0){
                                                            stable[35]=colour;
                                                            a++;
                                                            }
                                        }
                    if(colour==1) value+=a;
                    else value-=a;
                    }
    
    return value;
}

//return all legal moves (as kirby)
//move.board - these moves
//move.num - number of these moves (= function 'mobility')
kirby move(int board[64],int player){
    kirby move;
    move.num = 0; //start at 0
    int index = -1; //for move order
    int position;
    int poscheck;
    
    prove:
    index++;
    if(index==60) return move;
    
    position=moveOrder[index];
    if(board[position]!=0) goto prove; //deal with the occupied
    
    //search 8 directions away from position
    //ignore 'non-player'
    //if reach border -- no good
    //if reach empty -- no good
    //if reach player (that is not next to position) -- good
    //else(reach non-player) search further

    //upleft
    poscheck=position;
    loop1:
    poscheck-=9;
    if(poscheck<0 or poscheck%8==7); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position-9){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop1;
       
    //up
    poscheck=position;
    loop2:
    poscheck-=8;
    if(poscheck<0); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position-8){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop2;
       
    //upright
    poscheck=position;
    loop3:
    poscheck-=7;
    if(poscheck<0 or poscheck%8==0); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position-7){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop3;
       
    //left
    poscheck=position;
    loop4:
    poscheck-=1;
    if(poscheck<0 or poscheck%8==7); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position-1){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop4;
       
    //right
    poscheck=position;
    loop5:
    poscheck+=1;
    if(poscheck%8==0); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position+1){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop5;
       
    //downleft
    poscheck=position;
    loop6:
    poscheck+=7;
    if(poscheck>63 or poscheck%8==7); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position+7){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop6;
       
    //down
    poscheck=position;
    loop7:
    poscheck+=8;
    if(poscheck>63); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position+8){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop7;
    
    //downright
    poscheck=position;
    loop8:
    poscheck+=9;
    if(poscheck>63 or poscheck%8==0); //do nothing
    else if(board[poscheck]==0); //do nothing
    else if(board[poscheck]==player){
                                     if(poscheck!=position+9){ //update and to next position!
                                                              move.board[move.num]=position;
                                                              move.num++;
                                                              goto prove;
                                                              }
                                     }
    else goto loop8;
    
    goto prove;
}

//flip board when player places a disk at a position
//flip.board=flipped board, flip.num=number of flipped disks
//if flip.num=0 return some random board (save time) 
kirby flip(int board[64],int position,int player){
       kirby flip;
       int i;
       int poscheck;
	   int opPlayer = opposite[player];
       
       flip.num=0; //start at 0
       if(board[position]) return flip; //if nonempty
       
       //search 8 directions away from position
       //first check conditions: position is movable and first sq is opponent
       //then look further in that direction
       //if reach player - flip!
       //if reach opponent and not border - look further
       //else - end process
       
       //upleft
       if(IsUpleftMovable[position]){
       if(board[position-9]==opPlayer){
       
       poscheck=position-18;
       loop1:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position-9;i>poscheck;i-=9){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsUpleftBorder[poscheck]){poscheck-=9; goto loop1;}
       
       }
       }
       
       //up
       if(IsUpMovable[position]){
       if(board[position-8]==opPlayer){
       
       poscheck=position-16;
       loop2:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position-8;i>poscheck;i-=8){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsUpBorder[poscheck]){poscheck-=8; goto loop2;}
       
       }
       }
       
       //upright
       if(IsUprightMovable[position]){
       if(board[position-7]==opPlayer){
       
       poscheck=position-14;
       loop3:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position-7;i>poscheck;i-=7){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsUprightBorder[poscheck]){poscheck-=7; goto loop3;}
       
       }
       }
       
       //left
       if(IsLeftMovable[position]){
       if(board[position-1]==opPlayer){
       
       poscheck=position-2;
       loop4:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position-1;i>poscheck;i-=1){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsLeftBorder[poscheck]){poscheck-=1; goto loop4;}
       
       }
       }
       
       //right
       if(IsRightMovable[position]){
       if(board[position+1]==opPlayer){
       
       poscheck=position+2;
       loop5:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position+1;i<poscheck;i+=1){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsRightBorder[poscheck]){poscheck+=1; goto loop5;}
       
       }
       }
       
       //downleft
       if(IsDownleftMovable[position]){
       if(board[position+7]==opPlayer){
       
       poscheck=position+14;
       loop6:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position+7;i<poscheck;i+=7){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsDownleftBorder[poscheck]){poscheck+=7; goto loop6;}
       
       }
       }
       
       //down
       if(IsDownMovable[position]){
       if(board[position+8]==opPlayer){
       
       poscheck=position+16;
       loop7:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position+8;i<poscheck;i+=8){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsDownBorder[poscheck]){poscheck+=8; goto loop7;}
       
       }
       }
       
       //downright
       if(IsDownrightMovable[position]){
       if(board[position+9]==opPlayer){
       
       poscheck=position+18;
       loop8:
       if(board[poscheck]==player){
                                   if(flip.num==0) for(i=0;i<64;i++) flip.board[i]=board[i];
                                   for(i=position+9;i<poscheck;i+=9){
                                                                     flip.board[i]=player;
                                                                     flip.num++;
                                                                     }
                                   }
       else if(board[poscheck]) if(!IsDownrightBorder[poscheck]){poscheck+=9; goto loop8;}
       
       }
       }
       
       //place player's disk at position
       flip.board[position]=player;
       return flip;
}

//flip function that return number of disks flipped -- used in end-game position
//all unnecessary computations are removed to save time
int flipnum(int board[64],int position,int player){
       int i;
       int poscheck;
       int flipnum=0; //start at 0
	   int opPlayer = opposite[player];
       
       if(board[position]) return 0; //if nonempty
       
       //search 8 directions away from position
       //first check conditions: position is movable and first sq is opponent
       //then look further in that direction
       //if reach player - flip!
       //if reach opponent and not border - look further
       //else - end process
       
       //upleft
       if(IsUpleftMovable[position]){
       if(board[position-9]==opPlayer){
       
       poscheck=position-18;
       i=1;
       loop1:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsUpleftBorder[poscheck]){poscheck-=9; i++; goto loop1;}
       
       }
       }
       
       //up
       if(IsUpMovable[position]){
       if(board[position-8]==opPlayer){
       
       poscheck=position-16;
       i=1;
       loop2:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsUpBorder[poscheck]){poscheck-=8; i++; goto loop2;}
       
       }
       }
       
       //upright
       if(IsUprightMovable[position]){
       if(board[position-7]==opPlayer){
       
       poscheck=position-14;
       i=1;
       loop3:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsUprightBorder[poscheck]){poscheck-=7; i++; goto loop3;}
       
       }
       }
       
       //left
       if(IsLeftMovable[position]){
       if(board[position-1]==opPlayer){
       
       poscheck=position-2;
       i=1;
       loop4:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsLeftBorder[poscheck]){poscheck-=1; i++; goto loop4;}
       
       }
       }
       
       //right
       if(IsRightMovable[position]){
       if(board[position+1]==opPlayer){
       
       poscheck=position+2;
       i=1;
       loop5:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsRightBorder[poscheck]){poscheck+=1; i++; goto loop5;}
       
       }
       }
       
       //downleft
       if(IsDownleftMovable[position]){
       if(board[position+7]==opPlayer){
       
       poscheck=position+14;
       i=1;
       loop6:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsDownleftBorder[poscheck]){poscheck+=7; i++; goto loop6;}
       
       }
       }
       
       //down
       if(IsDownMovable[position]){
       if(board[position+8]==opPlayer){
       
       poscheck=position+16;
       i=1;
       loop7:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsDownBorder[poscheck]){poscheck+=8; i++; goto loop7;}
       
       }
       }
       
       //downright
       if(IsDownrightMovable[position]){
       if(board[position+9]==opPlayer){
       
       poscheck=position+18;
       i=1;
       loop8:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsDownrightBorder[poscheck]){poscheck+=9; i++; goto loop8;}
       
       }
       }
       
       return flipnum;
}
