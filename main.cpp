// project O'hello

//general info
const char version[] = "test72";
const char author[] = "Nat Sothanaphan & Sorawee Porncharoenwase";
const char date[] = "August 4, 2013";
const char language[] = "C++";
const char compiler[] = "LLVM-G++";

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <sys/timeb.h> //required for 'ftime'

//(own header)
#include "edgeconfig2.h" //precomputed edge configurations
#include "mobtable.h" //precomputed mobility table

//Oak's definitions

#include <iostream>
#include <string>
#include <signal.h>
#include <cassert>

//(own header)
#include "mystdio.h"
#include "precompute.h"
#include "config.h"

#define printf printw

const int EXIT = 555;
const int MAXN = 8;

int rand(int a){
	return rand() % a;
}

int rand(int a, int b){
	return (rand() % (b - a + 1)) + a;
	
}

void alert(std::string s){
	printf("\n%s\n\n", s.c_str());
}

void clrscr(){
	system("clear");
}

void finish(int sig){
    exit(0);
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

//End of Oak's definitions

void space(int n){
	std::string a;
	for(int i = 0; i < n; ++i) a += " ";
	printf("%s", a.c_str());
}

void backspace(int n){
	std::string a;
	for(int i = 0; i < n; ++i) a += "\b";
	printf("%s", a.c_str());
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
int load();
void syntax();
void about();
void speedtest();
void sayhello();
void settings();
void aisinterface();
void customweight();
void rotateoption();
void flipoption();
void openingoption();
int human(int board[64],int no[2],int player);
void humansave(int board[64],int player);
int comhuman(int board[64],int no[2],int player,int complayer,int mode,int depth,int depthperfect,float times);
void comhumansave(int board[64],int player,int complayer,int mode,int depth,int depthperfect,float times);
int comcom(int board[64],int no[2],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]);
void comcomsave(int board[64],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]);
void weightfortest(int doubleweight[2][100],int player);
void weighttest();
struct triad gamefortest(int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2],int doubleweight[2][100],int numgame,int numrand);
int input(int board[64],int player,int no[2]);
void shownode(int node);
int random(int board[64],int player,int no[2],int display);
int fsearch(int board[64],int depthwant,int player,int no[2],int display);
int tsearch(int board[64],float times,int player,int no[2],int display);
int score(int board[64],int depthleft,int player,int no[2],int cmpscore,int display);
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
struct kirby move(int board[64],int player);
struct kirby flip(int board[64],int position,int player);
int flipnum(int board[64],int position,int player);

//interface
int main(){
	system("stty erase ^?");
	signal(SIGINT, finish);
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
    int loadvalue;
    comset pack;
    srand(time(NULL)); //set up the random number generator
	
    start:
	
    clrscr(); //clear screen
    printf("\n version %s\n",version);
	printf("                                  ----------- \n");
    printf("                                 |           |\n");
    printf("                                 |  O'hello  |\n");
	printf("                                 |           |\n");
    printf("                                  -----------\n\n\n");
    printf("welcome! please select game mode or function below:\n\n");
    printf("0. program settings\n");
    printf("1. human   [black] vs human   [white]\n");
    printf("2. human   [black] vs O'hello [white]\n"); //human starts first
    printf("3. O'hello [black] vs human   [white]\n"); //computer starts first
    printf("4. O'hello [black] vs O'hello [white]\n");
    printf("5. weight test\n");
    printf("6. about program\n\n");
	
    //set weight as defaults
    weightdefault();
    //set board
    //board: 0=space,1=player 1,2=player 2
    int board[64] = {};
    board[27] = 2;
    board[28] = 1;
    board[35] = 1;
    board[36] = 2;
    int no[2] = {2, 2}; //set number of each player's disks
    int player = 1; //set current player

    loop1:
	std::string option = getString();
	
    if(option == "randon"){
		rands = true;
		alert("random feature on!");
		goto loop1;
	}
    if(option == "randoff"){
		rands = false;
		alert("random feature off!");
		goto loop1;
	}
    if(option == "moveon"){
		smove = true;
		alert("move display on!");
		goto loop1;
	}
    if(option == "moveoff"){
		smove = false;
		alert("move display off!");
		goto loop1;
	}
    if(option == "spinon"){
		rotateon = true;
		alert("rotation effect on!");
		goto loop1;
	}
    if(option == "spinoff"){
		rotateon = false;
		alert("rotation effect off!");
		goto loop1;
	}
    if(option == "flipon"){
		flipon = true;
		alert("flip animation on!");
		goto loop1;
	}
    if(option == "flipoff"){
		flipon = false;
		alert("flip animation off!");
		goto loop1;
	}
    if(option == "menu") goto start;
    if(option == "quit") finish(0);
    if(option == "syntax"){
		syntax();
		goto loop1;
	}
    if(option == "load"){
		loadvalue = load();
		if(loadvalue == EXIT) goto loop1;
		else goto start;
	}
    if(option == "speed"){
		speedtest();
		goto loop1;
	}
    if(option == "hello"){
		sayhello();
		goto loop1;
	}
    if(option.size() != 1){
		alert("incorrect syntax!");
		goto loop1;
	}
    if(option == "0"){
		settings();
		goto start;
	}
    if(option == "1"){
		//human vs human
		do{
			for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			for(int i = 0; i < 2; i++) sentno[i] = no[i];
		}while(human(sentboard, sentno, player) == EXIT);
		goto start;
	}else if(option == "2" or option == "3"){
		 //human vs computer,computer vs human
		clrscr();
		printf("\nselect search mode for O'hello:\n");
		printf("-------------------------------\n\n");
		//get settings
		pack = comsettings();
		mode = pack.mode;
		depth = pack.depth;
		depthperfect = pack.depthperfect;
		times = pack.times;
		if(option[0] == '2'){
			 do{
			 	for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			 	for(int i = 0; i < 2; i++) sentno[i] = no[i];
			 }while(comhuman(sentboard, sentno, player, 2, mode, depth, depthperfect, times) == EXIT);
			 goto start;
		}else{
		 do{
		 	for(int i = 0; i < 64; i++) sentboard[i] = board[i];
		 	for(int i = 0; i < 2; i++) sentno[i] = no[i];
		 }while(comhuman(sentboard, sentno, player, 1, mode, depth, depthperfect, times) == EXIT);
		 goto start;
		}
    }else if(option[0] == '4'){
		//computer vs computer
		for(int i = 0; i < 2; i++){
			printf("\nselect search mode for O'hello %d:\n", i + 1);
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
        goto start;
	}else if(option[0] == '5'){
		weighttest();
		goto start;
	}else if(option[0] == '6'){
		about();
		goto start;
	}else{
		alert("invalid mode!");
		goto loop1;
	}
	finish(0);
}

//AI settings interface
comset comsettings(){
	comset pack;
	if(comsetchoice == 2){
		// old interface
		printf("0. random: random play\n");
		printf("1. fsearch: fixed depth search\n");
		printf("2. tsearch: time limit search\n\n");
		
		setmode:
		scanf("%d", &pack.mode);
		if(pack.mode < 0 or pack.mode > 2){
			alert("invalid mode!");
			goto setmode;
		}
		if(pack.mode == 2) pack.mode = 3; //make compatible with other functions
		pack.depth = -1;
		pack.depthperfect = 1;
		pack.times = -1;
		
		setdetail:
		if(pack.mode == 1){
			printf("\ninput search depth and depthperfect: ");
			scanf("%d", &pack.depth);
			scanf("%d", &pack.depthperfect);
			if(pack.depth < 1 or pack.depthperfect < 1){
				printf("\ninvalid depth!\n");
				goto setdetail;
			}
		}else if(pack.mode == 3){
		    printf("\nNOTE: time taken will be about 0.5x-2x time limit");
		    printf("\n\ninput time limit (sec): ");
		    scanf("%f", &pack.times);
		    if(pack.times<0){
				printf("\ninvalid time!\n");
				goto setdetail;
			}
		}
	}else{
		// new interface
		assert(comsetchoice == 1);
		printf("1. search with fixed depth\n");
		printf("2. search with time limit\n\n");
		
		newsetmode:
		scanf("%d", &pack.mode);
		if(pack.mode < 0 or pack.mode > 2){
			alert("invalid mode!"); 
			goto newsetmode;
		}
		if(pack.mode == 2) pack.mode = 3; //make compatible with other functions
		pack.depth = -1;
		pack.depthperfect = 1;
		pack.times = -1;
		if(pack.mode == 1){
			printf("\n");
			printf("select search depth:\n");
			printf("--------------------\n");
			printf(" 1  - depth 1\n");
			printf(" 2  - depth 2\n");
			printf(" 3  - depth 3\n");
			printf(" 4  - depth 4\n");
			printf(" 4a - depth 4 + last 10 perfect\n");
			printf(" 6  - depth 6\n");
			printf(" 6a - depth 6 + last 12 perfect\n");
			printf(" 8  - depth 8 + last 14 perfect\n");
			printf(" 10 - depth 10 + last 16 perfect\n\n");
			
			newsetdepth:
			
			std::string option = getString();
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
				alert("invalid selection!");
				goto newsetdepth;
			}
		}else{
		    newsettimes:
		    printf("\nNOTE: time taken will be about 0.5x-2x time limit");
		    printf("\n\ninput time limit (sec): ");
		    scanf("%f", &pack.times);
		    if(pack.times < 0){
				printf("\ninvalid time!\n");
				goto newsettimes;
			}
		}
	}
	return pack;
}

//load engine
//return EXIT if file is corrupted
//return 0 if load successfully
int load(){
    char filename[100];
    int prerand;
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
    
    printf("\nfile name (no extension): ");
    scanf("%s", filename);
    save = fopen(strcat(filename, ".ohl"), "r");
    if(save == NULL){
		printf("\n\ncan't open %s or file doesn't exist\n\n",filename); 
		return EXIT;
	}
    printf("\n\nopening %s\n",filename);
    //get rands
    prerand = -1;
    fscanf(save, "%d", &prerand);
    if(prerand != 0 and prerand != 1){
		printf("\n%s is corrupted!\n\n", filename);
		fclose(save);
		return EXIT;
	}
    rands = prerand;
    printf("\nvalue rands=%d", rands);
    //get board[64] (along with no[2])
    printf("\nvalue board=");
    no[0] = 0;
    no[1] = 0;
    for(int i = 0; i < 64; ++i){
		board[i] = -1;
		fscanf(save, "%d", &board[i]);
		if(board[i] == 0){
			//the 4 centers can't be empty -- this will affect the searches
			//since they aren't included in moveorder[60]
			if(i == 27 or i == 28 or i == 35 or i == 36){
				printf("\n\n%s is corrupted!\n\n", filename);
				fclose(save);
				return EXIT;
			}else{
				printf("0");
			}
		}else if(board[i] == 1){
			printf("1");
			no[0]++;
		}else if(board[i] == 2){
			printf("2");
			no[1]++;
		}
		else{
			printf("\n\n%s is corrupted!\n\n", filename);
			fclose(save);
			return EXIT;
		}
	}
    //get player
    player = -1;
    fscanf(save, "%d", &player);
    if(player != 1 and player != 2){
		printf("\n\n%s is corrupted!\n\n", filename);
		fclose(save);
		return EXIT;
	}
    printf("\nvalue player=%d", player);
    //get loadmode
    int loadmode = -1;
    fscanf(save, "%d", &loadmode);
    if(loadmode == 1){
		//human vs human
		printf("\nvalue loadmode=1\n\n");
		printf("game successfully loaded!\n\n");
		presstogo();
		do{
			for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			for(int i = 0; i < 2; i++) sentno[i] = no[i];
		}while(human(sentboard,sentno,player) == EXIT);
		return 0;
	}else if(loadmode == 2 or loadmode == 3){
		//computer vs human, human vs computer
		printf("\nvalue loadmode=%d", loadmode);
		//get mode
		mode = -1;
		fscanf(save, "%d", &mode);
		if(mode < 0 or mode > 3){
			printf("\n\n%s is corrupted!\n\n", filename);
			fclose(save);
			return EXIT;
		}
		printf("\nvalue mode=%d", mode);
		//get depth or times
		depth = -1;
		depthperfect = -1;
		times = -1;
		if(mode == 1 or mode == 2){
			fscanf(save, "%d", &depth);
			if(depth < 1){
				printf("\n\n%s is corrupted!\n\n", filename);
				fclose(save);
				return EXIT;
			}
			printf("\nvalue depth=%d", depth);
			fscanf(save, "%d", &depthperfect);
			if(depthperfect < 1){
				printf("\n\n%s is corrupted!\n\n", filename);
				fclose(save);
				return EXIT;
			}
			printf("\nvalue depthperfect=%d", depthperfect);
		}else if(mode == 3){
			fscanf(save, "%f", &times);
			if(times < 0){
				printf("\n\n%s is corrupted!\n\n", filename);
				fclose(save);
				return EXIT;
			}
			printf("\nvalue times=%f", times);
		}
		printf("\n\ngame successfully loaded!");
		printf("\n\npress any key to continue");
		getch();
		if(loadmode == 2){
			do{
				for(int i = 0; i < 64; i++) sentboard[i] = board[i];
				for(int i = 0; i < 2; i++) sentno[i] = no[i];
			}while(comhuman(sentboard, sentno, player, 2, mode, depth, depthperfect, times) == EXIT);
			return 0;
		}else{
			do{
				for(int i = 0; i < 64; i++) sentboard[i]=board[i];
				for(int i = 0; i < 2; i++) sentno[i]=no[i];
			}while(comhuman(sentboard, sentno, player, 1, mode, depth, depthperfect, times) == EXIT);
			return 0;
		}
	}else if(loadmode == 4){
		//computer vs computer
		printf("\nvalue loadmode=4");
		for(int i = 0; i <= 1; i++){
			//get doublemode
			doublemode[i] = -1;
			fscanf(save, "%d", &doublemode[i]);
			if(doublemode[i] < 0 or doublemode[i] > 3){
				printf("\n\n%s is corrupted!\n\n", filename);
				fclose(save);
				return EXIT;
			}
			printf("\nvalue doublemode[%d]=%d", i, doublemode[i]);
			//get doubledepth or doubletimes
			doubledepth[i] = -1;
			doubledepthperfect[i] = -1;
			doubletimes[i] = -1;
			if(doublemode[i] == 1 or doublemode[i] == 2){
				fscanf(save, "%d", &doubledepth[i]);
				if(doubledepth[i] < 1){
					printf("\n\n%s is corrupted!\n\n", filename);
					fclose(save);
					return EXIT;
				}
				printf("\nvalue doubledepth[%d]=%d", i, doubledepth[i]);
				fscanf(save, "%d", &doubledepthperfect[i]);
				if(doubledepthperfect[i] < 1){
					printf("\n\n%s is corrupted!\n\n", filename);
					fclose(save);
					return EXIT;
				}
				printf("\nvalue doubledepthperfect[%d]=%d", i, doubledepthperfect[i]);
			}
			if(doublemode[i] == 3){
				fscanf(save, "%f", &doubletimes[i]);
				if(doubletimes[i] < 0){
					printf("\n\n%s is corrupted!\n\n", filename);
					fclose(save);
					return EXIT;
				}
				printf("\nvalue doubletimes[%d]=%f", i, doubletimes[i]);
			}
		}
		printf("\n\ngame successfully loaded!");
		printf("\n\npress any key to continue");
		getch();
		do{
			for(int i = 0; i < 64; i++) sentboard[i] = board[i];
			for(int i = 0; i < 2; i++) sentno[i] = no[i];
			for(int i = 0; i < 2; i++) sentdoublemode[i] = doublemode[i];
			for(int i = 0; i < 2; i++) sentdoubledepth[i] = doubledepth[i];
			for(int i = 0; i < 2; i++) sentdoubledepthperfect[i] = doubledepthperfect[i];
			for(int i = 0; i < 2; i++) sentdoubletimes[i] = doubletimes[i];
		}while(comcom(sentboard, sentno, player, sentdoublemode, sentdoubledepth, sentdoubledepthperfect, sentdoubletimes) == EXIT);
		return 0;
	}else{
		printf("\n\n%s is corrupted!\n\n", filename);
		return EXIT;
	}
}

//show syntax list
void syntax(){
	printf("\n\
syntax list\n\
-----------\n\
\n\
m = available in the main menu\n\
g = available in-game\n\
+ = available as one-letter syntaxes on computer's turns\n\
\n\
load        m    - load game from a file\n\
speed       m    - test the speed of your computer\n\
quit        m g+ - exit program\n\
menu        m g+ - go to the main menu\n\
save          g+ - save game in a file\n\
new           g+ - restart the game using current settings\n\
undo          g  - undo a move (can only go back 1 move)\n\
reflect       g  - reflect the board horizontally\n\
randon/off  m g  - enable/disable random feature\n\
moveon/off  m g  - enable/disable move display\n\
spinon/off  m g  - enable/disable rotation effect\n\
flipon/off  m g  - enable/disable flip animation\n\
syntax      m g  - view this text\n\
fsearch       g  - fixed depth search ex. fsearch 6\n\
tsearch       g  - time limit search ex. tsearch 1\n\
hello       m    - say hello to the program\n\n");
}

//about program
void about(){
	clrscr();
	printf("\n\
about program\n\
-------------\n\
\n\
project O'hello\n\
\n\
version  %s\n\
author   %s\n\
date     %s\n\
language %s\n\
compiler %s\n\
\n\
O'hello is a command-line program that mainly features an AI that plays othello.\n\
It is only an elementary level program with low search speed and weak evaluation\n\
function. However we hope it would be a good practice for all players aiming to\n\
improve their skills at othello.\n\
\n\
Note: there are several commands available in O'hello. You can type 'syntax' to\n\
view the list of all these commands.\n\
\n\n\
press 'h' to view information on how to play othello\n\
press any other key to go back to the main menu", version, author, date, language, compiler);

	if(getch() == 'h'){
		clrscr();
		printf("\n\
how to play othello\n\
-------------------\n\
\n\
Othello is played on an 8x8 board using disks coloured black on one side and\n\
white on the other side. The game starts with 2 black disks on d5 and e4, and 2\n\
white disks on d4 and e5. The two players take turns to place a disk of his own\n\
colour on the board, with black going first.\n\
\n\
A move consists of placing a new disk on an empty square, and flipping every\n\
opponent's disk or row of opponent's disks that is between the new disk and one\n\
of the player's disks in any direction: horizontal, vertical or diagonal.\n\
\n\
A move is legal if it flips at least one opponent's disk. A player with no legal\n\
moves must pass his turn. On the contrary, a player with a legal move cannot\n\
pass his turn.\n\
\n\
The game ends when no one has a legal move. The winner is the player with a\n\
larger number of disks, with a draw possible if both numbers are the same. The\n\
remaining empty squares, if any, are usually awarded to the winner.\n\
\n\n\
press any key to go back to the main menu");
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
	const char* greeting[] = {	"hello, player",
								"hi, player",
								"greetings, player",
								"hello there, player",
								"hi there, player",
								"Oh, hello player",
								"Oh, hi player",
								"Oh, hello - that's my name!",
								"hello player, nice to meet you",
								"hello player, pleased to meet you"
							};
	printf("\n%s\n\n", greeting[rand(sizeof(greeting) / sizeof(greeting[0]))]);
}

//settings
void settings(){
     printf("\nselect settings:\n\n");
     printf("1. AI settings interface\n");
     printf("2. evaluation function weights\n");
     printf("3. rotation effect options\n");
     printf("4. flip animation options\n");
     printf("5. opening move options\n");
     printf("\n");
	 bool invalid;
	 do{
		invalid = false;
		switch(getInt()){
			case 1: aisinterface(); break;
			case 2: customweight(); break;
			case 3: rotateoption(); break;
			case 4: flipoption(); break;
			case 5: openingoption(); break;

			default:
			alert("invalid selection!");
			invalid = true;
		}
	}while(invalid);
}

//set value of comsetchoice
void aisinterface(){
	printf("\n");
	printf("select interface:\n");
	printf("-----------------\n");
	printf("1. new one (default)\n");
	printf("2. old one\n");
	printf("3. back\n\n");
	bool invalid;
	do{
		invalid = false;
		int input = getInt();
		if(input == 1 or input == 2){
			comsetchoice = input;
			printf("\nsuccessfully set interface\n");
			presstogo();
		}else if(input == 3);
		else{
		  	alert("invalid selection!");
		  	invalid = true;
		}
	}while(invalid);
}

//customize weights
void customweight(){
	clrscr(); //clear screen
	printf("\n");
	printf("current weights:\n\n");
	for(int i = 0; i < weightnum; i++) printf(" %d", dweight[i]);
	printf("\n\n\n");
	printf("choose evaluation function weights:\n");
	printf("-----------------------------------\n\n");
	printf("0. customize weights\n\n");
	for(int i = 0; i < weightchoicenum; i++){
		printf("%d. %s\n\n", i + 1, weightchoicename[i]);
		printf("  ");
		for(int j = 0; j < weightnum; j++) printf(" %d", weightchoice[i][j]);
		printf("\n\n");
	}
	printf("%d. back\n\n", weightchoicenum + 1);
	bool invalid;
	int input;
	do{
		invalid = false;
		input = getInt();
		if(input < 0 or input > weightchoicenum + 1){
			alert("invalid selection!");
			invalid = true;
		}
	}while(invalid);
	if(input == weightchoicenum + 1);
	else if(input != 0){
		for(int i = 0; i < weightnum; i++) dweight[i] = weightchoice[input-1][i];
		printf("\nsuccessfully set weights\n");
		presstogo();
	}else{
		printf("\nenter evaluation function weights:\n\n");
		for(int i = 0; i < weightnum; i++) scanf("%d", &dweight[i]);
		printf("\n\nsuccessfully set weights\n");
		presstogo();
	}
}

//rotation effect options
void rotateoption(){
     int input;
     
     clrscr(); //clear screen
     printf("\n");
     printf("current rotation settings:\n");
     printf("--------------------------\n\n");
     if(rotateon) printf("rotation effect is enabled\n\n");
     else printf("rotation effect is disabled\n\n");
     printf("display effect every %d node(s)",rotatetime);
     printf("\n\n\n");
     printf("select options:\n");
     printf("---------------\n");
     printf("1.enable effect\n");
     printf("2.disable effect\n");
     printf("3.customize frequency\n");
     printf("4.back\n\n");
     select:
     scanf("%d",&input);
     if(input<1 or input>4){printf("\ninvalid selection!\n\n"); goto select;}
     else if(input==4) return;
     else if(input==1){
                       rotateon = true;
                       printf("\nsuccessfully enabled effect\n");
                       printf("press any key to continue");
                       getch();
                       return;
                       }
     else if(input==2){
                       rotateon = false;
                       printf("\nsuccessfully disabled effect\n");
                       printf("press any key to continue");
                       getch();
                       return;
                       }
     else{
          printf("\n");
          select2:
          printf("enter number of nodes between effects: ");
          scanf("%d",&rotatetime);
          if(rotatetime<1){printf("\ninvalid number!\n\n"); goto select2;}
          printf("\nsuccessfully set effect frequency\n");
          printf("press any key to continue");
          getch();
          return;
          }
}

//flip animation options
void flipoption(){
     int input;
     
     clrscr(); //clear screen
     printf("\n");
     printf("current flip settings:\n");
     printf("----------------------\n\n");
     if(flipon) printf("flip animation is enabled\n\n");
     else printf("flip animation is disabled\n\n");
     printf("time between each frame: %.2f second(s)",fliptime);
     printf("\n\n\n");
     printf("select options:\n");
     printf("---------------\n");
     printf("1.enable effect\n");
     printf("2.disable effect\n");
     printf("3.customize flip speed\n");
     printf("4.back\n\n");
     select:
     scanf("%d",&input);
     if(input<1 or input>4){printf("\ninvalid selection!\n\n"); goto select;}
     else if(input==4) return;
     else if(input==1){
                       flipon = true;
                       printf("\nsuccessfully enabled effect\n");
                       printf("press any key to continue");
                       getch();
                       return;
                       }
     else if(input==2){
                       flipon = false;
                       printf("\nsuccessfully disabled effect\n");
                       printf("press any key to continue");
                       getch();
                       return;
                       }
     else{
          printf("\n");
          select2:
          printf("enter time between frames: ");
          scanf("%f",&fliptime);
          if(fliptime<0){printf("\ninvalid number!\n\n"); goto select2;}
          printf("\nsuccessfully set flip speed\n");
          printf("press any key to continue");
          getch();
          return;
          }
}

//opening move options
void openingoption(){
     int input;
     
     clrscr(); //clear screen
     printf("\n");
     printf("current setting:\n\n");
     if(not ParallelAllow) printf("allow diagonal and perpendicular opening");
     else printf("allow diagonal, perpendicular and parallel opening");
     printf("\n\n\n");
     printf("select options:\n");
     printf("---------------\n");
     printf("1.allow diagonal and perpendicular opening\n");
     printf("2.allow diagonal, perpendicular and parallel opening\n");
     printf("3.back\n\n");
     select:
     scanf("%d",&input);
     if(input<1 or input>3){printf("\ninvalid selection!\n\n"); goto select;}
     else if(input==3) return;
     else if(input==1){
                       ParallelAllow = false;
                       printf("\nsuccessfully set opening moves\n");
                       printf("press any key to continue");
                       getch();
                       return;
                       }
     else if(input==2){
                       ParallelAllow = true;
                       printf("\nsuccessfully set opening moves\n");
                       printf("press any key to continue");
                       getch();
                       return;
                       }
}

//human vs human
int human(int board[64],int no[2],int player){
    printf(""); //prevent bug
    int i,j,k; //for reflect
    int position;
    int mobilities; //value from function 'mobility'
    struct kirby flips; //value from function 'flip'
    char letter; //for one-letter syntax
    int currmove=-1;
    int lastmove=-1;
    //for undo
    int oldboard[64];
    int oldno[2];
    int oldplayer;
    int undo=0; //0=can't undo,1=can undo
    int oldlastmove=-1;

    loop2: //start here!
    clrscr(); //clear screen
    //display board
    display(board,player,no,lastmove);
    printf("\n\n");
    //diplay player
    if(player==1){
                  printf("         %s\n",black[0]);
                  printf("you are: %s [black]\n",black[1]);
                  printf("         %s",black[2]);
                  }
    else{
         printf("         %s\n",white[0]);
         printf("you are: %s [white]\n",white[1]);
         printf("         %s",white[2]);
         }
    //compute mobility
    indexformob(board);
    mobilities=mobility(player);
    if(mobilities!=0){
                      //if player is movable
                      printf("\n\nplayer %d's turn: input position or command\n\n",player);
                      loop1:
                      position=input(board,player,no); //get position
                      //-999 means save
                      if(position==-999){humansave(board,player); goto loop2;}
                      //-1 means undo
                      if(position==-1){
                                       if(undo==0){
                                                   printf("\ncannot undo!\n\n");
                                                   goto loop1;
                                                   }
                                       //use old values
                                       for(i=0;i<64;i++) board[i]=oldboard[i];
                                       no[0]=oldno[0];
                                       no[1]=oldno[1];
                                       player=oldplayer;
                                       lastmove=oldlastmove;
                                       undo=0; //now we can't undo
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
                      if(flipon) flipanimation(board,player,position,flips.board);
                      currmove=position;
                      //update old values
                      for(i=0;i<64;i++) oldboard[i]=board[i];
                      oldno[0]=no[0];
                      oldno[1]=no[1];
                      oldplayer=player;
                      oldlastmove=lastmove;
                      lastmove=currmove;
                      undo=1; //now we can undo :)
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
         printf("\n\nplayer %d's turn: no moves left",player);
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
     int i;
     printf("\nfile name (no extension): ");
     scanf("%s",filename);
     save=fopen(strcat(filename,".ohl"),"w");
     //print rands
     fprintf(save,"%d",rands);
     //print board
     for(i=0;i<64;i++) fprintf(save," %d",board[i]);
     //print player
     fprintf(save," %d",player);
     //print loadmode
     fprintf(save," 1");
     fclose(save);
     printf("\ngame saved in %s successfully!",filename);
     printf("\npress any key to continue");
     getch();
     return;
}

//human vs computer,computer vs human
//complayer=player that computer plays as
int comhuman(int board[64],int no[2],int player,int complayer,int mode,int depth,int depthperfect,float times){
    int i,j,k; //for reflect
    int position;
    int mobilities; //value from function 'mobility'
    struct kirby flips; //value from function 'flip'
    char letter; //for one-letter syntax
    int currmove=-1;
    int lastmove=-1;
    //for undo
    int oldboard[64];
    int oldno[2];
    int oldplayer;
    int undo=0; //0=can't undo,1=can undo
    int oldlastmove=-1;
     
    loop2: //start here!
    clrscr(); //clear screen
    //display board
    display(board,player,no,lastmove);
    printf("\n\n");
    //diplay player
    if(player==1){
                  if(player==complayer){
                                        printf("            %s\n",black[0]);
                                        printf("O'hello is: %s [black]\n",black[1]);
                                        printf("            %s",black[2]);
                                        }
                  else{
                       printf("         %s\n",black[0]);
                       printf("you are: %s [black]\n",black[1]);
                       printf("         %s",black[2]);
                       }
                  }
    else{
         if(player==complayer){
                               printf("            %s\n",white[0]);
                               printf("O'hello is: %s [white]\n",white[1]);
                               printf("            %s",white[2]);
                               }
         else{
              printf("         %s\n",white[0]);
              printf("you are: %s [white]\n",white[1]);
              printf("         %s",white[2]);
              }         
         }
    //compute mobility
    indexformob(board);
    mobilities=mobility(player);
    if(mobilities!=0){
                      //if player is movable
                      if(player==complayer){
                                            //for computer's turn
                                            printf("\n\nthinking");
											space(10);
                                            //get position
                                            if(mode==0) position=random(board,player,no,1);
                                            if(mode==1){
                                                        //for near-end evaluation, use depthperfect
                                                        if(no[0]+no[1]>=64-depthperfect) position=fsearch(board,depthperfect,player,no,1);
                                                        else position=fsearch(board,depth,player,no,1);
                                                        }
                                            if(mode==3) position=tsearch(board,times,player,no,1);
                                            printf("\n\n's' to save  'n' for new game  'm' for menu  'q' to quit");
                                            printf("\npress any other key to continue");
                                            letter=getch();
                                            if(letter=='s'){printf("\n"); comhumansave(board,player,complayer,mode,depth,depthperfect,times); goto loop2;}
                                            if(letter=='n') return EXIT;
                                            if(letter=='m') return 0;
                                            if(letter=='q') finish(0);
                                            currmove=position;
                                            lastmove=currmove;
                                            flips=flip(board,position,player); //flip!
                                            //show flip animation
                                            if(flipon) flipanimation(board,player,position,flips.board);
                                            //get board from function 'flip'
                                            for(i=0;i<64;i++) board[i]=flips.board[i];
                                            //update no[2]
                                            no[player-1]+=flips.num+1;
                                            no[2-player]-=flips.num;
                                            }
                      else{
                           //for human's turn
                           printf("\n\nyour turn: input position or command\n\n");
                           loop1:
                           position=input(board,player,no); //get position
                           //-999 means save
                           if(position==-999){comhumansave(board,player,complayer,mode,depth,depthperfect,times); goto loop2;}
                           //-1 means undo
                           if(position==-1){
                                            if(undo==0){
                                                        printf("\ncannot undo!\n\n");
                                                        goto loop1;
                                                        }
                                            //use old values
                                            for(i=0;i<64;i++) board[i]=oldboard[i];
                                            no[0]=oldno[0];
                                            no[1]=oldno[1];
                                            player=oldplayer;
                                            lastmove=oldlastmove;
                                            undo=0; //now we can't undo
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
                           if(flipon) flipanimation(board,player,position,flips.board);
                           currmove=position;
                           //update old values
                           for(i=0;i<64;i++) oldboard[i]=board[i];
                           oldno[0]=no[0];
                           oldno[1]=no[1];
                           oldplayer=player;
                           oldlastmove=lastmove;
                           lastmove=currmove;
                           undo=1; //now we can undo :)
                           //get board from function 'flip'
                           for(i=0;i<64;i++) board[i]=flips.board[i];
                           //update no[2]
                           no[player-1]+=flips.num+1;
                           no[2-player]-=flips.num;
                           }
                      player=3-player; //switch player
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
                               printf("\n\nO'hello passed its turn");
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
     int i;
     printf("\nfile name (no extension): ");
     scanf("%s",filename);
     save=fopen(strcat(filename,".ohl"),"w");
     //print rands
     fprintf(save,"%d",rands);
     //print board
     for(i=0;i<64;i++) fprintf(save," %d",board[i]);
     //print player
     fprintf(save," %d",player);
     //print loadmode
     fprintf(save," %d",4-complayer);
     //print mode
     fprintf(save," %d",mode);
     //print depth or times
     if(mode==1 or mode==2) fprintf(save," %d %d",depth,depthperfect);
     if(mode==3) fprintf(save," %f",times);
     fclose(save);
     printf("\ngame saved in %s successfully!",filename);
     printf("\n\npress any key to continue");
     getch();
     return;
}

//computer vs computer
int comcom(int board[64],int no[2],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]){
    printf(""); //prevent bug
    int i;
    int position;
    int mobilities; //value from function 'mobility'
    struct kirby flips; //value from function 'flip'
    char letter;
    int currmove=-1;
    int lastmove=-1;
    
    loop2: //start here!
    clrscr(); //clear screen
    //display board
    display(board,player,no,lastmove);
    printf("\n\n");
    //diplay player
    if(player==1){
                  printf("              %s\n",black[0]);
                  printf("O'hello 1 is: %s [black]\n",black[1]);
                  printf("              %s",black[2]);
                  }
    else{
         printf("              %s\n",white[0]);
         printf("O'hello 2 is: %s [white]\n",white[1]);
         printf("              %s",white[2]);
         }
    //compute mobility
    indexformob(board);
    mobilities=mobility(player);
    if(mobilities!=0){ //if player is movable
                      printf("\n\nthinking");
					  space(10);
                      //get position
                      if(doublemode[player-1]==0) position=random(board,player,no,1);
                      if(doublemode[player-1]==1){
                                                  //for near-end evaluation, use depthperfect
                                                  if(no[0]+no[1]>=64-doubledepthperfect[player-1]) position=fsearch(board,doubledepthperfect[player-1],player,no,1);
                                                  else position=fsearch(board,doubledepth[player-1],player,no,1);
                                                  }
                      if(doublemode[player-1]==3) position=tsearch(board,doubletimes[player-1],player,no,1);
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
                      if(flipon) flipanimation(board,player,position,flips.board);
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
         printf("\n\nO'hello %d passed its turn",player);
         printf("\n\npress any key to continue");
         getch();
         printf("\n");
         player=3-player; //switch player
         goto loop2;
         }
}

//save engine of computer vs computer
void comcomsave(int board[64],int player,int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2]){
     char filename[100];
     int i;
     printf("\nfile name (no extension): ");
     scanf("%s",filename);
     save=fopen(strcat(filename,".ohl"),"w");
     //print rands
     fprintf(save,"%d",rands);
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
                       if(doublemode[i]==1 or doublemode[i]==2) fprintf(save," %d %d",doubledepth[i],doubledepthperfect[i]);
                       if(doublemode[i]==3) fprintf(save," %f",doubletimes[i]);
                       }
     fclose(save);
     printf("\ngame saved in %s successfully!",filename);
     printf("\n\npress any key to continue");
     getch();
     return;
}

//set weight for weight test
void weightfortest(int doubleweight[2][100],int player){
     int i;
     for(i=0;i<weightnum;i++) weight[i]=doubleweight[player-1][i];
     nickname();
}

//weight test
void weighttest(){
     int doublemode[2];
     int doubledepth[2];
     int doubledepthperfect[2];
     float doubletimes[2];
     int doubleweight[2][100]; //weights for 2 players
     int i,j;
     int a;
     float b;
     struct triad result;
     float score1;
     float score2;
     
     int numgame; //number of games to play
     int numrand; //number of random first moves
     int twoway; //0 = computer 1 plays first only
                 //1 = computer 1,2 plays first

     printf("\ndefault weights are:\n\n");
     for(i=0;i<weightnum;i++) printf("%d ",dweight[i]);
     printf("\n");

     for(i=0;i<=1;i++){
                       set:
                       printf("\nenter computer %d parameters:\n\n",i+1);
                       //parameters = mode depth/time [weights]
                       scanf("%d",&doublemode[i]);
                       if(doublemode[i]<0 or doublemode[i]>3){printf("\nvalue 1 error!\n"); goto set;}
                       if(doublemode[i]!=0){
                                            if(doublemode[i]!=3){
                                                                 scanf("%d",&doubledepth[i]);
                                                                 if(doubledepth[i]<1){printf("\nvalue 2a error!\n"); goto set;}
                                                                 scanf("%d",&doubledepthperfect[i]);
                                                                 if(doubledepthperfect[i]<1){printf("\nvalue 2b error!\n"); goto set;}
                                                                 }
                                            else{
                                                 scanf("%f",&doubletimes[i]);
                                                 if(doubletimes[i]<0){printf("\nvalue 2 error!\n"); goto set;}
                                                 }
                                            for(j=0;j<weightnum;j++) scanf("%d",&doubleweight[i][j]);
                                            }
                       }
     settwo:
     printf("\nenter test parameters:\n\n");
     //parameters = numgame numrand twoway
     scanf("%d",&numgame);
     if(numgame<1){printf("\nvalue 1 error!\n"); goto settwo;}
     scanf("%d",&numrand);
     if(numrand<0 or numrand>60){printf("\n value 2 error!\n"); goto settwo;}
     scanf("%d",&twoway);
     if(twoway!=0 and twoway!=1){printf("\nvalue 3 error!\n"); goto settwo;}
     
     //computer 1 vs computer 2
     
     printf("\noutput : ");
     result=gamefortest(doublemode,doubledepth,doubledepthperfect,doubletimes,doubleweight,numgame,numrand);
     printf("%d %d %d",result.win,result.lose,result.draw);
     
     score1=result.win+(float)result.draw/2;
     score2=result.lose+(float)result.draw/2;
     
     //percentage
     if(twoway==0){
                   printf("\n\ncom 1 won %.1f/%d games = %.2f%c",score1,numgame,100*score1/numgame,37);
                   printf("\ncom 2 won %.1f/%d games = %.2f%c",score2,numgame,100*score2/numgame,37);
                   }

     //computer 2 vs computer 1 (only if twoway=1)
     
     if(twoway==1){
     
     printf("\noutput2: ");
     //switch parameters
     //doublmode
     a=doublemode[0];
     doublemode[0]=doublemode[1];
     doublemode[1]=a;
     //doubledepth
     a=doubledepth[0];
     doubledepth[0]=doubledepth[1];
     doubledepth[1]=a;
     //doubledepthperfect
     a=doubledepthperfect[0];
     doubledepthperfect[0]=doubledepthperfect[1];
     doubledepthperfect[1]=a;
     //doubletimes
     b=doubletimes[0];
     doubletimes[0]=doubletimes[1];
     doubletimes[1]=b;
     //weight
     for(i=0;i<weightnum;i++){
                              a=doubleweight[0][i];
                              doubleweight[0][i]=doubleweight[1][i];
                              doubleweight[1][i]=a;
                              }
     result=gamefortest(doublemode,doubledepth,doubledepthperfect,doubletimes,doubleweight,numgame,numrand);     
     printf("%d %d %d",result.lose,result.win,result.draw);
     //here we switched position of result.lose and result.win
     
     score1+=result.lose+(float)result.draw/2;
     score2+=result.win+(float)result.draw/2;
     
     //percentage
     printf("\n\ncom 1 won %.1f/%d games = %.2f%c",score1,2*numgame,50*score1/numgame,37);
     printf("\ncom 2 won %.1f/%d games = %.2f%c",score2,2*numgame,50*score2/numgame,37);
     }
     
     getch();
     return;
}

struct triad gamefortest(int doublemode[2],int doubledepth[2],int doubledepthperfect[2],float doubletimes[2],int doubleweight[2][100],int numgame,int numrand){
     struct triad result;
     result.win=0;
     result.lose=0;
     result.draw=0;
     int game=0;
     
     int i;
     int position;
     int mobilities;
     struct kirby flips;
     
     //loop each game
     do{
        //set board
        int board[64]={};
        board[27]=2;
        board[28]=1;
        board[35]=1;
        board[36]=2;
        int no[2]={2,2};
        int player=1;

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
                                  if(doublemode[player-1]==3) position=tsearch(board,doubletimes[player-1],player,no,0);
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
int input(int board[64],int player,int no[2]){
    char row;
    char column;
    char string[10];
    int i,j;
    int depth;
    float times;
    loop1:
    scanf("%s",string); //scan string
    if(strcmp(string,"undo")==0) return -1; //-1 means undo
    if(strcmp(string,"reflect")==0) return -2; //-2 means reflect
    if(strcmp(string,"new")==0) return -3; //-3 means new
    if(strcmp(string,"menu")==0) return -4; //-4 means menu
    if(strcmp(string,"save")==0) return -999; //-999 means save
    if(strcmp(string,"randon")==0){
                                   rands = true;
                                   printf("\nrandom feature on!\n\n");
                                   goto loop1;
                                   }
    if(strcmp(string,"randoff")==0){
                                    rands = false;
                                    printf("\nrandom feature off!\n\n");
                                    goto loop1;
                                    }
    //-5 means move on-off
    if(strcmp(string,"moveon")==0){smove = true; return -5;}
    if(strcmp(string,"moveoff")==0){smove = false; return -5;}
    if(strcmp(string,"spinon")==0){
                                   rotateon = true;
                                   printf("\nrotation effect on!\n\n");
                                   goto loop1;
                                   }
    if(strcmp(string,"spinoff")==0){
                                    rotateon = false;
                                    printf("\nrotation effect off!\n\n");
                                    goto loop1;
                                    }
    if(strcmp(string,"flipon")==0){
                                   flipon=true;
                                   printf("\nflip animation on!\n\n");
                                   goto loop1;
                                   }
    if(strcmp(string,"flipoff")==0){
                                    flipon=true;
                                    printf("\nflip animation off!\n\n");
                                    goto loop1;
                                    }
    if(strcmp(string,"quit")==0) finish(0);
    if(strcmp(string,"fsearch")==0){
                                    scanf("%d",&depth);
                                    if(depth<1){
                                                printf("\ninvalid depth!\n\n");
                                                goto loop1;
                                                }
                                    printf("\nthinking");
									space(10);
                                    fsearch(board,depth,player,no,2);
                                    printf("\n\n");
                                    goto loop1;
                                    }
    if(strcmp(string,"tsearch")==0){
                                    scanf("%f",&times);
                                    if(times<0){
                                         printf("\ninvalid time!\n\n");
                                         goto loop1;
                                         }
                                    printf("\nthinking ");
									space(10);
                                    tsearch(board,times,player,no,2);
                                    printf("\n\n");
                                    goto loop1;
                                    }
    if(strcmp(string,"syntax")==0){syntax(); goto loop1;}
    if(strlen(string)!=2){
                          printf("\nincorrect syntax!\n\n");
                          goto loop1;
                          }
    column=string[0];
    row=string[1];
    //column to 0-7
    switch(column){
                   case 'a':i=0; break;
                   case 'b':i=1; break;
                   case 'c':i=2; break;
                   case 'd':i=3; break;
                   case 'e':i=4; break;
                   case 'f':i=5; break;
                   case 'g':i=6; break;
                   case 'h':i=7; break;
                   default:printf("\nincorrect syntax!\n\n"); goto loop1;
                   }
    //row to 0-7
    switch(row){
                case '1':j=0; break;
                case '2':j=1; break;
                case '3':j=2; break;
                case '4':j=3; break;
                case '5':j=4; break;
                case '6':j=5; break;
                case '7':j=6; break;
                case '8':j=7; break;
                default:printf("\nincorrect syntax!\n\n"); goto loop1;
                }
    return i+8*j;
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
                       if(not ParallelAllow){
                                            //recompute moves.board
                                            if(board[19]!=0){moves.board[0]=18; moves.board[1]=34;}
                                            if(board[26]!=0){moves.board[0]=18; moves.board[1]=20;}
                                            if(board[37]!=0){moves.board[0]=43; moves.board[1]=45;}
                                            if(board[44]!=0){moves.board[0]=29; moves.board[1]=45;}
                                            if(board[20]!=0){moves.board[0]=21; moves.board[1]=37;}
                                            if(board[29]!=0){moves.board[0]=19; moves.board[1]=21;}
                                            if(board[34]!=0){moves.board[0]=42; moves.board[1]=44;}
                                            if(board[43]!=0){moves.board[0]=26; moves.board[1]=42;}
                                            
                                            if(rands) position = moves.board[rand(2)];
                                            else position=moves.board[0];
                                            }
                       //if allow parallel opening
                       else{
                            if(rands) position = moves.board[rand(3)];
                            else position=moves.board[0];
                            }
                       }
    else position=moves.board[rand(moves.num)]; //random move
    
    if(display!=0){
                   backspace(18); //clear "thinking "
                   printf("O'hello decided to place a disk at ");
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
    int bestscore; //best of scores[64]
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
    
    ftime(&time1); //get current time
    node=0; //reset node
    moves=move(board,player); //get move list

    //if only one legal move -- no need to search
    if(moves.num==1){position=moves.board[0]; depthwant=0; enginestate=1; goto finish;}

    //for starting position -- no need to search
    if(no[0]+no[1]==4){
                       //if random feature is on
                       if(rands) position = moves.board[rand(4)];
                       else position=moves.board[0];
                       depthwant=0;
                       enginestate=2;
                       goto finish;
                       }
    if(no[0]+no[1]==5){
                       //if no parallel opening
                       if(not ParallelAllow){
                                            //recompute moves.board
                                            if(board[19]!=0){moves.board[0]=18; moves.board[1]=34;}
                                            if(board[26]!=0){moves.board[0]=18; moves.board[1]=20;}
                                            if(board[37]!=0){moves.board[0]=43; moves.board[1]=45;}
                                            if(board[44]!=0){moves.board[0]=29; moves.board[1]=45;}
                                            if(board[20]!=0){moves.board[0]=21; moves.board[1]=37;}
                                            if(board[29]!=0){moves.board[0]=19; moves.board[1]=21;}
                                            if(board[34]!=0){moves.board[0]=42; moves.board[1]=44;}
                                            if(board[43]!=0){moves.board[0]=26; moves.board[1]=42;}
                                            
                                            if(rands) position = moves.board[rand(2)];
                                            else position=moves.board[0];
                                            }
                       //if allow parallel opening
                       else{
                            if(rands) position = moves.board[rand(3)];
                            else position=moves.board[0];
                            }
                       
                       depthwant=0;
                       enginestate=2;
                       goto finish;
                       }
    
    //if searches up to end-game (or 1 square left)
    if(depthwant>=63-no[0]-no[1]){
                                  depthwant=64-no[0]-no[1];
                                  depthshallow=fmax(depthwant/2-2,0);
                                  enginestate=0;
                                  perfect=1;
                                  }
    else{
         depthshallow=fmax(depthwant/2-1,0);
         enginestate=0;
         perfect=0;
         }
    
    //for the shallow search------------------------------------
    
    if(depthshallow>0){
                       //set extreme value
                       if(player==1) bestscore=-123456789;
                       else bestscore=123456789;   
                       //get score for all legal moves
                       for(k=0;k<moves.num;k++){
                                                position=moves.board[k];
                                                flips=flip(board,position,player); //flip!
                                                //update no[2] (as nonew[2])
                                                nonew[player-1]=no[player-1]+flips.num+1;
                                                nonew[2-player]=no[2-player]-flips.num;
                                                //get score
                                                scores[k]=score(flips.board,depthshallow-1,3-player,nonew,bestscore,display);
                                                if(player==1){ //if player 1: max mode
                                                               if(scores[k]>bestscore) bestscore=scores[k];
                                                               }
                                                else{ //if player 2: min mode
                                                      if(scores[k]<bestscore) bestscore=scores[k];
                                                      }
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
    if(player==1) bestscore=-123456789;
    else bestscore=123456789;          
    //get score for all legal moves
    for(k=0;k<moves.num;k++){
                             position=moves.board[k];
                             flips=flip(board,position,player); //flip!
                             //update no[2] (as nonew[2])
                             nonew[player-1]=no[player-1]+flips.num+1;
                             nonew[2-player]=no[2-player]-flips.num;
                             //get score
                             scores[k]=score(flips.board,depthwant-1,3-player,nonew,bestscore,display);
                             if(player==1){ //if player 1: max mode
                                           if(scores[k]>bestscore) bestscore=scores[k];
                                           }
                             else{ //if player 2: min mode
                                  if(scores[k]<bestscore) bestscore=scores[k];
                                  }
                             }
                             
    //---------------------------------------------------------
    
    //set extreme value
    if(player==1) bestscore=-123456789;
    else bestscore=123456789;
    //determine candidates
    for(k=0;k<moves.num;k++){
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
    if(rands) position = candidate[rand(numcan)]; //random from candidates 
    else position=candidate[0];

    finish:
    ftime(&time2); //get current time
    if(display != 0){
		if(display == 1){
			backspace(18); //clear "thinking "
			printf("O'hello decided to place a disk at ");
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
    int bestscore; //best of scores[64]
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
                       if(rands) position = moves.board[rand(4)];
                       else position=moves.board[0];
                       depth=1;
                       posreach=0;
                       enginestate=2;
                       goto finish;
                       }
    if(no[0]+no[1]==5){
                       //if no parallel opening
                       if(not ParallelAllow){
                                            //recompute moves.board
                                            if(board[19]!=0){moves.board[0]=18; moves.board[1]=34;}
                                            if(board[26]!=0){moves.board[0]=18; moves.board[1]=20;}
                                            if(board[37]!=0){moves.board[0]=43; moves.board[1]=45;}
                                            if(board[44]!=0){moves.board[0]=29; moves.board[1]=45;}
                                            if(board[20]!=0){moves.board[0]=21; moves.board[1]=37;}
                                            if(board[29]!=0){moves.board[0]=19; moves.board[1]=21;}
                                            if(board[34]!=0){moves.board[0]=42; moves.board[1]=44;}
                                            if(board[43]!=0){moves.board[0]=26; moves.board[1]=42;}
                                            
                                            if(rands) position = moves.board[rand(2)];
                                            else position=moves.board[0];
                                            }
                       //if allow parallel opening
                       else{
                            if(rands) position = moves.board[rand(3)];
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
                                               if(player==1) bestscore=-123456789;
                                               else bestscore=123456789;
                                               //get score for all legal moves
                                               for(k=0;k<moves.num;k++){
                                                                        position=moves.board[k];
                                                                        flips=flip(board,position,player); //flip!
                                                                        //update no[2] (as nonew[2])
                                                                        nonew[player-1]=no[player-1]+flips.num+1;
                                                                        nonew[2-player]=no[2-player]-flips.num;
                                                                        //get score
                                                                        scores[k]=score(flips.board,depth-1,3-player,nonew,bestscore,display);
                                                                        
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
                                                                        
                                                                        if(player==1){ //if player 1: max mode
                                                                                      if(scores[k]>bestscore) bestscore=scores[k];
                                                                                      }
                                                                        else{ //if player 2: min mode
                                                                             if(scores[k]<bestscore) bestscore=scores[k];
                                                                             }
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
    if(player==1) bestscore=-123456789;
    else bestscore=123456789;
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
    if(rands) position = candidate[rand(numcan)]; //random from candidates
    else position = candidate[0];

    finish:
    ftime(&time2); //get current time
    if(display!=0){
                   if(display==1){
                                  backspace(18); //clear "thinking "
                                  printf("O'hello decided to place a disk at ");
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
int score(int board[64],int depthleft,int player,int no[2],int cmpscore,int display){
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

     //if completely filled -- game ends
     if(no[0]+no[1]==64){
                         node++;
                         if(node%rotatetime==0) nodedisplay(display);
                         if(no[0]>no[1]) return wf*(64-2*no[1]);
                         else if(no[0]<no[1]) return wf*(2*no[0]-64);
                         else return 0;
                         }

     //terminal -- return evaluation function ----------------------------------

     if(depthleft==0){
                      node++;
                      if(node%rotatetime==0) nodedisplay(display);
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
     //check if searches up to end-game (or 1 square left)
     if(depthleft>=63-no[0]-no[1]) depthshallow=fmax(depthleft/2-2,0);
     else depthshallow=fmax(depthleft/2-1,0);
     
     //if no shallow search -- no record on position scores,move list, etc.
     //===================================================================
     if(depthshallow<=0){
     
     if(player==1){
                   //if player 1: max mode
                   scores=-123456789; //set extreme value
                   for(index=0;index<60;index++){
                                                 position=moveorder[index];
                                                 //check before flip
                                                 if(board[position]==0){
                                                                        flips=flip(board,position,player); //flip!
                                                                        //if move is legal
                                                                        if(flips.num!=0){
                                                                                         //update no[2] (as nonew[2])
                                                                                         nonew[player-1]=no[player-1]+flips.num+1;
                                                                                         nonew[2-player]=no[2-player]-flips.num;
                                                                                         //get score
                                                                                         //scores is used as cmpscore
                                                                                         candidate=score(flips.board,depthleft-1,3-player,nonew,scores,display);
                                                                                         if(candidate>cmpscore) return candidate; //alpha-beta pruning
                                                                                         if(candidate>scores) scores=candidate; //update score
                                                                                         }
                                                                        }
                                                 }
                   //if no legal move
                   if(scores==-123456789){
                                          //if game ends
                                          indexformob(board);
                                          if(mobility(3-player)==0){
                                                                          node++;
                                                                          if(node%rotatetime==0) nodedisplay(display);
                                                                          if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                          else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                          else return 0;
                                                                          }
                                          //if pass turn
                                          //use depthleft not depthleft-1 to preserve no. of disks
                                          return score(board,depthleft,3-player,no,scores,display);
                                          }
                   return scores;
                   }
     else{
          //if player 2: min mode
          scores=123456789; //set extreme value
          for(index=0;index<60;index++){
                                        position=moveorder[index];
                                        if(board[position]==0){
                                                               flips=flip(board,position,player); //flip!
                                                               //if move is legal
                                                               if(flips.num!=0){
                                                                                //update no[2] (as nonew[2])
                                                                                nonew[player-1]=no[player-1]+flips.num+1;
                                                                                nonew[2-player]=no[2-player]-flips.num;
                                                                                //get score
                                                                                //scores is used as cmpscore
                                                                                candidate=score(flips.board,depthleft-1,3-player,nonew,scores,display);
                                                                                if(candidate<cmpscore) return candidate; //alpha-beta pruning
                                                                                if(candidate<scores) scores=candidate; //update score
                                                                                }
                                                               }
                                        }
          //if no legal move
          if(scores==123456789){
                                //if game ends
                                indexformob(board);
                                if(mobility(3-player)==0){
                                                                node++;
                                                                if(node%rotatetime==0) nodedisplay(display);
                                                                if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                else return 0;
                                                                }
                                //if pass turn
                                //use depthleft not depthleft-1 to preserve no. of disks
                                return score(board,depthleft,3-player,no,scores,display);
                                }
          return scores;
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
                   scores=-123456789; //set extreme value
                   for(index=0;index<60;index++){
                                                 position=moveorder[index];
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
                                                                                         candidate=score(flips.board,depthshallow-1,3-player,nonew,scores,display);
                                                                                         scorearray[movenum-1]=candidate;
                                                                                         posarray[movenum-1]=position;
                                                                                         //[this line to return score is deleted in shallow search]
                                                                                         if(candidate>scores) scores=candidate; //update score
                                                                                         }
                                                                        }
                                                 }
                   //if no legal move (allow to return score)
                   if(movenum==0){
                                          //if game ends
                                          indexformob(board);
                                          if(mobility(3-player)==0){
                                                                          node++;
                                                                          if(node%rotatetime==0) nodedisplay(display);
                                                                          if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                          else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                          else return 0;
                                                                          }
                                          //if pass turn
                                          //use depthleft not depthleft-1 to preserve no. of disks
                                          return score(board,depthleft,3-player,no,scores,display);
                                          }
                   //[this line to return score is deleted in shallow search]
                   }
     else{
          //if player 2: min mode
          scores=123456789; //set extreme value
          for(index=0;index<60;index++){
                                        position=moveorder[index];
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
                                                                                candidate=score(flips.board,depthshallow-1,3-player,nonew,scores,display);
                                                                                scorearray[movenum-1]=candidate;
                                                                                posarray[movenum-1]=position;
                                                                                //[this line to return score is deleted in shallow search]
                                                                                if(candidate<scores) scores=candidate; //update score
                                                                                }
                                                               }
                                        }
          //if no legal move (allow to return score)
          if(movenum==0){
                                //if game ends
                                indexformob(board);
                                if(mobility(3-player)==0){
                                                                node++;
                                                                if(node%rotatetime==0) nodedisplay(display);
                                                                if(no[0]>no[1]) return wf*(64-2*no[1]);
                                                                else if(no[0]<no[1]) return wf*(2*no[0]-64);
                                                                else return 0;
                                                                }
                                //if pass turn
                                //use depthleft not depthleft-1 to preserve no. of disks
                                return score(board,depthleft,3-player,no,scores,display);
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
     
     //set extreme value
     if(player==1) scores=-123456789;
     else scores=123456789;          
     //get score for all legal moves
     for(k=0;k<movenum;k++){
                            position=posarray[k];
                            flips=flip(board,position,player); //flip!
                            //update no[2] (as nonew[2])
                            nonew[player-1]=no[player-1]+flips.num+1;
                            nonew[2-player]=no[2-player]-flips.num;
                            //get score
                            candidate=score(flips.board,depthleft-1,3-player,nonew,scores,display);
                            if(player==1){ //if player 1: max mode
                                           if(candidate>cmpscore) return candidate; //*add alpha-beta pruning here
                                           if(candidate>scores) scores=candidate;
                                           }
                            else{ //if player 2: min mode
                                  if(candidate<cmpscore) return candidate; //*add alpha-beta pruning here
                                  if(candidate<scores) scores=candidate;
                                  }
                            }
     return scores;
     
     }
}

//compute score when there is only 1 square left
//all unnecessary computations are removed to save time
int score63(int board[64],int player,int no[2],int display){
    node++; //this is one node exactly!
    if(node%rotatetime==0) nodedisplay(display);
    
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
     if(display!=0 and rotateon){
		 backspace(9);
                   switch((node/rotatetime)%4){
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
     return;
}

//display board string
//to be used in function 'display' and 'flipanimation'
//interpretation of board input: 0=empty,1=black,2=white,3=flipping
void boarddisplay(int board[64],int playertoshowmove){
     int i,j;
     char string[2000]={}; //board string
     
     //ascii for components
     char upleftcorner[2] = "+";
     char uprightcorner[2]= "+";
	 char downleftcorner[2] = "+";
     char downrightcorner[2] = "+";
     char fivehorizontal[6] = "-----";
     char horizontaldown[2] = "+";
     char horizontalup[2] = "+";
     char vertical[2] = "|";
     char verticalright[2] = "+";
     char verticalleft[2] = "+";
     char fourarm[2] = "+";
     char rownumber[8][2] = {"1","2","3","4","5","6","7","8"};
     
     //construct board string
     
     //header
     strcat(string,"      a     b     c     d     e     f     g     h");
     strcat(string,"\n   ");
     strcat(string,upleftcorner);
     for(j=1;j<=7;j++){
                       strcat(string,fivehorizontal);
                       strcat(string,horizontaldown);
                       }
     strcat(string,fivehorizontal);
     strcat(string,uprightcorner);
     
     //8 groups of rows
     for(i=0;i<=7;i++){
                       //row 1
                       strcat(string,"\n   ");
                       strcat(string,vertical);
                       for(j=8*i;j<8*i+8;j++){
                                              if(board[j]==1) strcat(string,black[0]);
                                              else if(board[j]==2) strcat(string,white[0]);
                                              else if(board[j]==3) strcat(string,fliplook[0]);
                                              else strcat(string,"     ");
                                              strcat(string,vertical);
                                              }
                       //row 2
                       strcat(string,"\n ");
                       strcat(string,rownumber[i]);
					   strcat(string," ");
                       strcat(string,vertical);
                       for(j=8*i;j<8*i+8;j++){
                                              if(board[j]==1) strcat(string,black[1]);
                                              else if(board[j]==2) strcat(string,white[1]);
                                              else if(board[j]==3) strcat(string,fliplook[1]);
                                              else{
                                                   //if move is to be shown (not in flipping process)
                                                   if(playertoshowmove!=0){
                                                                           //if move is legal and show move is on
                                                                           if(smove and flip(board,j,playertoshowmove).num!=0) strcat(string,"  .  ");
                                                                           else strcat(string,"     ");
                                                                           }
                                                   else strcat(string,"     ");
                                                   }
                                              strcat(string,vertical);
                                              }
                       //row 3
                       strcat(string,"\n   ");
                       strcat(string,vertical);
                       for(j=8*i;j<8*i+8;j++){
                                              if(board[j]==1) strcat(string,black[2]);
                                              else if(board[j]==2) strcat(string,white[2]);
                                              else if(board[j]==3) strcat(string,fliplook[2]);
                                              else strcat(string,"     ");
                                              strcat(string,vertical);
                                              }
                       //row 4
                       if(i!=7){
                                //not last row
                                strcat(string,"\n   ");
                                strcat(string,verticalright);
                                for(j=1;j<=7;j++){
                                                  strcat(string,fivehorizontal);
                                                  strcat(string,fourarm);
                                                  }
                                strcat(string,fivehorizontal);
                                strcat(string,verticalleft);
                                }
                       else{
                            //last row
                            strcat(string,"\n   ");
                            strcat(string,downleftcorner);
                            for(j=1;j<=7;j++){
                                              strcat(string,fivehorizontal);
                                              strcat(string,horizontalup);
                                              }
                            strcat(string,fivehorizontal);
                            strcat(string,downrightcorner);
                            }
                       }
     
     //print string (for fast display)
     printf("%s",string);
     
     return;
}

//display board and game information
void display(int board[64],int player,int no[2],int lastmove){
     int k;
     
     //display board
     boarddisplay(board,player);
     
     //display last move and no[2]
     //row 1
     printf("\n\n");
     for(k=1;k<=31;k++) printf(" ");
     printf("%s      %s",black[0],white[0]);
     //row 2
     printf("\nlast move: ");
     if(lastmove==-1) printf("- ");
     else{
          switch(lastmove%8){
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
          printf("%d",lastmove/8+1);
          }
     for(k=1;k<=18;k++) printf(" ");
     printf("%s x%2d  %s x%2d",black[1],no[0],white[1],no[1]);
     //row 3
     printf("\n");
     for(k=1;k<=31;k++) printf(" ");
     printf("%s      %s",black[2],white[2]);
     return;
}

//show flip animation!
void flipanimation(int board[64],int player,int position,int flipboard[64]){
     struct timeb time1,time2;
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
        }while(time2.time-time1.time+0.001*(time2.millitm-time1.millitm)<fliptime);
     
     //display flipping in progress
     for(i=0;i<64;i++){
                       if(dboard[i]!=flipboard[i]) dboard[i]=3; //determine position being flipped
                       }
     clrscr();
     boarddisplay(dboard,0);
     
     ftime(&time1);
     do{
        ftime(&time2);
        }while(time2.time-time1.time+0.001*(time2.millitm-time1.millitm)<fliptime);
     
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
struct kirby move(int board[64],int player){
    struct kirby move;
    move.num=0; //start at 0
    int index=-1; //for move order
    int position;
    int poscheck;
    
    prove:
    index++;
    if(index==60) return move;
    
    position=moveorder[index];
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
       if(board[position-9]==3-player){
       
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
       if(board[position-8]==3-player){
       
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
       if(board[position-7]==3-player){
       
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
       if(board[position-1]==3-player){
       
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
       if(board[position+1]==3-player){
       
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
       if(board[position+7]==3-player){
       
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
       if(board[position+8]==3-player){
       
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
       if(board[position+9]==3-player){
       
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
       
       if(board[position]) return 0; //if nonempty
       
       //search 8 directions away from position
       //first check conditions: position is movable and first sq is opponent
       //then look further in that direction
       //if reach player - flip!
       //if reach opponent and not border - look further
       //else - end process
       
       //upleft
       if(IsUpleftMovable[position]){
       if(board[position-9]==3-player){
       
       poscheck=position-18;
       i=1;
       loop1:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsUpleftBorder[poscheck]){poscheck-=9; i++; goto loop1;}
       
       }
       }
       
       //up
       if(IsUpMovable[position]){
       if(board[position-8]==3-player){
       
       poscheck=position-16;
       i=1;
       loop2:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsUpBorder[poscheck]){poscheck-=8; i++; goto loop2;}
       
       }
       }
       
       //upright
       if(IsUprightMovable[position]){
       if(board[position-7]==3-player){
       
       poscheck=position-14;
       i=1;
       loop3:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsUprightBorder[poscheck]){poscheck-=7; i++; goto loop3;}
       
       }
       }
       
       //left
       if(IsLeftMovable[position]){
       if(board[position-1]==3-player){
       
       poscheck=position-2;
       i=1;
       loop4:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsLeftBorder[poscheck]){poscheck-=1; i++; goto loop4;}
       
       }
       }
       
       //right
       if(IsRightMovable[position]){
       if(board[position+1]==3-player){
       
       poscheck=position+2;
       i=1;
       loop5:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsRightBorder[poscheck]){poscheck+=1; i++; goto loop5;}
       
       }
       }
       
       //downleft
       if(IsDownleftMovable[position]){
       if(board[position+7]==3-player){
       
       poscheck=position+14;
       i=1;
       loop6:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsDownleftBorder[poscheck]){poscheck+=7; i++; goto loop6;}
       
       }
       }
       
       //down
       if(IsDownMovable[position]){
       if(board[position+8]==3-player){
       
       poscheck=position+16;
       i=1;
       loop7:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsDownBorder[poscheck]){poscheck+=8; i++; goto loop7;}
       
       }
       }
       
       //downright
       if(IsDownrightMovable[position]){
       if(board[position+9]==3-player){
       
       poscheck=position+18;
       i=1;
       loop8:
       if(board[poscheck]==player) flipnum+=i;
       else if(board[poscheck]) if(!IsDownrightBorder[poscheck]){poscheck+=9; i++; goto loop8;}
       
       }
       }
       
       return flipnum;
}
