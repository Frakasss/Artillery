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

Player allPlayer[6];

//Game
String gamestatus;
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
extern const int soundfx[5][8];

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
  gamestatus="selectMap";
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
    // select map
    if (gamestatus=="selectMap") { 
      fnctn_checkbuttons();
      outpt_selectMap(); 
    }

    // create level
    if (gamestatus=="newLevel") { 
      screen=gamelevel-1;
      fnctn_nextPlayer();
      fnctn_newlevel();
    }
    
    // loading screen (needed to avoid having unexpected action)
    if(gamestatus=="loading" || gamestatus=="loadingPause"){
      fnctn_checkbuttons();
    }
    
    // pause / weapon select?
    if (gamestatus=="pause") {
      fnctn_checkbuttons();
      outpt_pause();
    }
    
    // game running
    if (gamestatus=="running") {
      outpt_landscape();
      outpt_players();
      outpt_power();
      
      if(allPlayer[currentPlayer-1].team==0){fnctn_checkbuttons();}
      else{fnctn_ia();}      
      
      fnctn_checkJump();
      if(jumpStatus<3){fnctn_checkPlayerPos();}
      if(allPlayer[currentPlayer-1].dead==1){
        fnctn_checkDead();
        fnctn_nextPlayer();
      }
      
      if(power==0 && jumpStatus==0){outpt_cursor();}
    }
    
    
    if(gamestatus=="animFire"){
      outpt_soundfx(0,0);
      outpt_landscape();
      outpt_players();
      fn_nextProjPosition();
      fn_checkCollision();
      outpt_projectile();
    }
    
    if(gamestatus=="boom"){
      outpt_soundfx(1,1);
      outpt_landscape();
      outpt_players();
      outpt_power();
      outpt_boom();
      fnctn_checkbuttons();
      if(timer>=7){
        fnctn_nextPlayer();
      }
    }
    
    if(gamestatus=="missed"){
      outpt_landscape();
      outpt_players();
      outpt_power();
      outpt_missed();
      fnctn_checkbuttons();
    }
    
    if(gamestatus=="gameover"){
      outpt_landscape();
      outpt_players();
      outpt_gameOver();
      fnctn_checkbuttons();
    }
    
  } // end of update
 } // end of loop
