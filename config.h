//black disk appearance
char black[3][6] = {"XXXXX", 
                    "XXXXX", 
					"XXXXX"};

//white disk appearance
char white[3][6] = {"ooooo",
 					"o   o",
					"ooooo"};

//disk appearance choice
const char choice[][3][6]={
     {"XXXXX","XXXXX","XXXXX"},
     {"ooooo","o   o","ooooo"},
     {{219,219,219,219,219},{219,219,219,219,219},{219,219,219,219,219}}, //dense
     {{177,177,177,177,177},{177,177,177,177,177},{177,177,177,177,177}}, //not very dense
     {"XXXXX","X   X","XXXXX"},
     {"ooooo","ooooo","ooooo"},
     {{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}}, //not dense smile
     {{2,2,2,2,2},{2,2,2,2,2},{2,2,2,2,2}}, //dense smile
     {{1,1,1,1,1},{1,32,32,32,1},{1,1,1,1,1}}, //not dense smile with hole
     {{2,2,2,2,2},{2,32,32,32,2},{2,2,2,2,2}}, //dense smile with hole
     {{32,15,32,15,32},{15,32,15,32,15},{32,15,32,15,32}}, //flower
     {"     ","black","     "},
     {"     ","white","     "},
     {"     ","  .  ","     "}
};

const int numchoice = 14; //number of choices

int rotatetime = 20000; //number of nodes between rotation effect displays
bool rotateon = true; //rotation effect on/off
float fliptime = 0.3; //time between each frame of flip animation
bool flipon = true; //flip animation on/off
//flip appearance
const char fliplook[3][6] = {{32,32,179,32,32},{32,32,179,32,32},{32,32,179,32,32}};

//move order
//priority:
// 1 8 2 3
// . 9 7 6
// . . 4 5
// . . . .
const int moveorder[60]={
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

//variables for settings 0=off,1=on
bool rands = true; //random feature
bool smove = true; //move display
bool ParallelAllow = false; //allow parallel opening

int comsetchoice = 1; //choice for comsettings interface 1=new,2=old

//default weights
int dweight[]= {-1, 40, 100, 600, 800, 140, 250, 100, 45, 2, 60, 40, 200, 800, 100, 250, 100, 1, 1000000};
const int weightnum = sizeof(dweight) / sizeof(dweight[0]); //number of weights

//custom weights
const int weightchoice[][100]={
    {1,100,0,1000,0,0,0,0,55,1,0,0,1000,0,0,0,0,0,10000},
    {1,100,200,4000,1000,1000,0,0,55,1,100,50,250,50,50,0,0,0,50000},
    {-1,40,100,600,800,140,250,100,45,2,60,40,200,800,100,250,100,1,1000000}
};

//name of choices
const char weightchoicename[][100]={
     "1st generation",
     "2nd generation",
     "3rd generation (default)"
};

const int weightchoicenum = sizeof(weightchoice) / sizeof(weightchoice[0]); //number of weight choices
