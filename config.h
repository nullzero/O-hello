//black disk appearance
char black[3][6] = {" X X ", 
                    "  X  ", 
					" X X "};

//white disk appearance
char white[3][6] = {"ooooo",
 					"ooooo",
					"ooooo"};

int rotatetime = 50000; //number of nodes between rotation effect displays
bool rotateon = true; //rotation effect on/off
float fliptime = 0.3; //time between each frame of flip animation
bool flipon = true; //flip animation on/off

//flip appearance
const char fliplook[3][6] = {"  |  ","  |  ","  |  "};

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

//weights
int weight[100];
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

//map weights to their nicknames
void nickname(){
     wd=weight[0];
     wm=weight[1];
     wp=weight[2];
     wc=weight[3];
     wxx=weight[4];
     wcc=weight[5];
     we=weight[6];
     ws=weight[7];
     wcut=weight[8];
     wd1=weight[9];
     wm1=weight[10];
     wp1=weight[11];
     wc1=weight[12];
     wxx1=weight[13];
     wcc1=weight[14];
     we1=weight[15];
     ws1=weight[16];
     wnew=weight[17];
     wf=weight[18];
}

//set weights(and their nicknames) as defaults
void weightdefault(){
     for(int i = 0; i < weightnum; i++) weight[i] = dweight[i];
     nickname();
}