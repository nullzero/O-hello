struct Conf{
	char black[3][6];
	char white[3][6];
	
	int rotateTime; //number of nodes between rotation effect displays
	bool rotateOn; //rotation effect on/off

	float flipTime; //time between each frame of flip animation
	bool flipOn; //flip animation on/off
	char flipLook[3][6];
		
	bool moveOn;
	char moveLook[3][6];

	bool randOn;
	bool parallelAllow;
	int rawSet;
	
	int weight[19];
};

Conf myConf;
	
//default config
const Conf DConf = {
	{" X X ", 
	 "  X  ", 
	 " X X "}, // black
	{"ooooo",
	 "ooooo",
	 "ooooo"}, // white
	50000, // rotateTime
	true, // rotateOn
	0.3, // flipTime
	true, // flipOn
	{"  |  ",
	 "  |  ",
	 "  |  "}, // flipLook
	true, // moveOn
	{"     ",
	 "  .  ",
	 "     "}, // moveLook
	true, // randOn
	false, // parallelAllow
	false, // rawSet
	{-1, 40, 100, 600, 800, 140, 250, 100, 45, 2, 60, 40, 200, 800, 100, 250, 100, 1, 1000000}, // weight
};
						   
const int weightNum = sizeof(DConf.weight) / sizeof(DConf.weight[0]); //number of weights

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
const int weightChoiceNum = sizeof(weightChoice) / sizeof(weightChoice[0]); //number of weight choices

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

int weightRaw[100];

int diskWidth = (sizeof(DConf.black[0]) / sizeof(DConf.black[0][0])) - 1;
int diskHeight = sizeof(DConf.black) / sizeof(DConf.black[0]);

//defaultation functions

//map weights to their nicknames
void nickname(){
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
void weightInitialize(){
     for(int i = 0; i < weightNum; i++) weightRaw[i] = DConf.weight[i];
     nickname();
}
