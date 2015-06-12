#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//----------------------------------------------------------------------------    
// define variables and constants
//----------------------------------------------------------------------------    
typedef struct {
  byte x;
  byte y;
  boolean dir;
  boolean dead;
  boolean team;
  byte fall;
} Player;

typedef struct{
  byte alien;
  byte target;
  byte power1;
  byte angle1;
  boolean dir1;
  byte collx1;
  byte colly1;
  byte power2;
  byte angle2;
  boolean dir2;
  byte collx2;
  byte colly2; 
} CpuMemory;

Player allPlayer[6];
CpuMemory cpuMem[12];

//Game
byte gamestatus; 
byte nbAvailableLevel;
byte gamelevel;
byte screen;
byte landscapeZip[21][12];
byte landscape[21][12];

//player
byte currentPlayer;
byte jumpStatus;
byte previousPlayerTeam0;
byte previousPlayerTeam1;
byte nbAlivePlayerTeam0;
byte nbAlivePlayerTeam1;

//projectile
float projPositionX;
float projPositionY;
float projTrajX;
float projTrajY;
float power;
byte ia_power;
byte angle;
byte ia_angle;
byte timer;

//----------------------------------------------------------------------------    
// define images & sounds
//----------------------------------------------------------------------------    
extern const byte PROGMEM gamelogo[];
extern const byte PROGMEM landscapetiles[32][6];
extern const byte PROGMEM levels[11][38];
extern const float trajParamX[];
extern const float trajParamY[];
extern const int soundfx[6][8];

//----------------------------------------------------------------------------    
// setup
//----------------------------------------------------------------------------    
void setup(){
  gb.begin();
  main_initGame();
}

//----------------------------------------------------------------------------    
// init game
//----------------------------------------------------------------------------  
void main_initGame(){
  gb.titleScreen(gamelogo);
  gb.battery.show=false;
  #define SELECT_MAP 1
  #define NEW_LEVEL 2
  #define LOADING 3  
  #define PAUSE 4
  #define RUNNING 5
  #define ANIMFIRE 6
  #define BOOM 7
  #define MISSED 8
  #define GAMEOVER 9
   
  gamestatus=SELECT_MAP;
  
  gamelevel=1;
  nbAvailableLevel=11;
  currentPlayer = 0;
  previousPlayerTeam0 = 0;
  previousPlayerTeam1 = 1;
  nbAlivePlayerTeam0=3;
  nbAlivePlayerTeam1=3;
  jumpStatus=0;
}

//----------------------------------------------------------------------------    
// loop
//----------------------------------------------------------------------------    
void loop(){
  if(gb.update()){    
    switch(gamestatus)
    {
      case SELECT_MAP :
           fnctn_checkbuttons();
           outpt_selectMap(); 
           break;
           
      case NEW_LEVEL :
           screen=gamelevel-1;
           fnctn_nextPlayer();
           fnctn_newlevel();
           break;
           
      case LOADING :
           fnctn_checkbuttons();
           break;
           
      case PAUSE :
           fnctn_checkbuttons();
           outpt_pause();
           break;
           
      case RUNNING :
           outpt_landscape();
           outpt_players();
           outpt_power();
          
           if(allPlayer[currentPlayer-1].team==0){
             fnctn_checkbuttons();}
           else{
             fnctn_ia();
             gb.display.print(cpuMem[currentPlayer/2].target);
             gb.display.print(allPlayer[cpuMem[currentPlayer/2].target].dead);
           }      
          
           fnctn_checkJump();
           if(jumpStatus<3){fnctn_checkPlayerPos();}
           if(allPlayer[currentPlayer-1].dead==1){
             fnctn_checkDead();
             fnctn_nextPlayer();
           }
          
           if(power==0 && jumpStatus==0){outpt_cursor();}
           break;
           
      case ANIMFIRE :
           outpt_soundfx(0,0);
           outpt_landscape();
           outpt_players();
           fn_nextProjPosition();
           fn_checkCollision();
           outpt_projectile();
           break;
           
      case BOOM :
           outpt_soundfx(1,0);
           outpt_landscape();
           outpt_players();
           outpt_power();
           outpt_boom();
           fnctn_checkbuttons();
           if(timer>=7){
             fnctn_nextPlayer();
           }
           break;
           
      case MISSED :
           outpt_landscape();
           outpt_players();
           outpt_power();
           outpt_missed();
           fnctn_checkbuttons();
           break;
           
      case GAMEOVER :
           outpt_landscape();
           outpt_players();
           outpt_gameOver();
           fnctn_checkbuttons();
           break;
      
    } // end of switch
  } // end of update
} // end of loop
