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
  byte team;
  byte fall;
  boolean isIA;
  byte life;
} Player;


typedef struct{
  byte nbAlive;
  byte lastPlayer;
} Team;

Player allPlayer[16];
Team teamInfo[4];

//Game
byte gamestatus; 
byte nbAvailableLevel;
byte nbAliveTeam;
byte gamelevel;
byte screen;
byte landscapeZip[21][12];
byte landscape[21][12];
byte setting;
//player
byte currentTeam;
byte currentPlayer;
byte jumpStatus;
byte nbPlayer;
byte nbTeam;

//projectile
float projPositionX;
float projPositionY;
float projTrajX;
float projTrajY;
float power;
byte angle;
byte timer;

//----------------------------------------------------------------------------    
// define images & sounds
//----------------------------------------------------------------------------    
extern const byte PROGMEM gamelogo[];
extern const byte PROGMEM landscapetiles[34][6];
extern const byte PROGMEM levels[11][38];
extern const byte PROGMEM options[];
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
  #define GAMEOVER 8
  #define OPTIONS 9
   
  gamestatus=SELECT_MAP;
  
  gamelevel=0;
  nbAvailableLevel=11;
  setting=0;

  nbTeam = 2; 
  nbPlayer = 3;
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
           
      case OPTIONS :
           outpt_options();
           fnctn_checkbuttons();
           break;
           
      case NEW_LEVEL :
           screen=gamelevel-1;
           fnctn_newlevel();
           nbAliveTeam = nbTeam;
           power = 0;
           angle = 8;
           timer = 10;
           jumpStatus=0;
           currentTeam=0;
           currentPlayer=0;
           for(byte tim = 0; tim < nbTeam; tim++){
             teamInfo[tim].nbAlive = nbPlayer;
             teamInfo[tim].lastPlayer = 0;
           }
           break;
           
      case LOADING :
           outpt_loading();
           fnctnt_loading();
           break;
           
      case PAUSE :
           outpt_pause();
           fnctn_checkbuttons();
           break;
           
      case RUNNING :
           outpt_landscape();
           outpt_players();
           outpt_power();
           outpt_team();
           if(allPlayer[currentPlayer].isIA==0){
             fnctn_checkbuttons();}
           else{
             //fnctn_ia();
           }      
           
           fnctn_checkJump();
           if(jumpStatus<3){
             fnctn_checkPlayerPos();
             if(allPlayer[currentPlayer].dead==1){
               fnctn_nextPlayer();
               check_gameOver();
             }
           }
           outpt_life();
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
           timer = timer + 1; 
           outpt_boom();
           switch(timer){
             case  4:
                   fnctn_rebuildMap();
                   break;
             case  7:
                   timer = 0;
                   fnctn_nextPlayer();
                   break;
           }
           fnctn_checkbuttons();
           break;
           
      case GAMEOVER :
           outpt_landscape();
           outpt_players();
           outpt_gameOver();
           fnctn_checkbuttons();
           break;
      
    } // end switch
  } // end update
} // end loop
