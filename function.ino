//##################################################################
//##################################################################
void fnctn_checkbuttons() {
  switch(gamestatus)
  {
     case SELECT_MAP :
          if(gb.buttons.pressed(BTN_DOWN)  && gamelevel+3<=nbAvailableLevel){gamelevel = gamelevel + 3;}
          if(gb.buttons.pressed(BTN_UP)    && gamelevel-3>=0)               {gamelevel = gamelevel - 3;}
          if(gb.buttons.pressed(BTN_RIGHT) && gamelevel+1<=nbAvailableLevel){gamelevel = gamelevel + 1;}
          if(gb.buttons.pressed(BTN_LEFT)  && gamelevel-1>=0)               {gamelevel = gamelevel - 1;}
          if(gb.buttons.pressed(BTN_A)){
            if(gamelevel==0){
              gamestatus=OPTIONS;
              setting=0;
            }else{
              gamestatus=NEW_LEVEL;
            }
          }
          break;
     
     case OPTIONS :
          if(gb.buttons.pressed(BTN_DOWN) && setting<2){setting=setting+1;}
          if(gb.buttons.pressed(BTN_UP) && setting>0){setting=setting-1;}
          if(gb.buttons.pressed(BTN_RIGHT)){
            if(setting==0 && nbTeam<4){nbTeam=nbTeam+1;}
            if(setting==1 && nbPlayer<4){nbPlayer=nbPlayer+1;}
          }
          if(gb.buttons.pressed(BTN_LEFT)){
            if(setting==0 && nbTeam>2){nbTeam=nbTeam-1;}
            if(setting==1 && nbPlayer>1){nbPlayer=nbPlayer-1;}
          }
          if(gb.buttons.pressed(BTN_A) && setting==2){gamestatus=SELECT_MAP;}
          break;
               
     case LOADING :
          //no button to check: timer
          break;
          
     case PAUSE :
          if(gb.buttons.pressed(BTN_DOWN) && setting<2){setting=setting+1;}
          if(gb.buttons.pressed(BTN_UP) && setting>0){setting=setting-1;}
          if(gb.buttons.pressed(BTN_A)){
            switch(setting){
              case 0:
                   gamestatus=RUNNING;
                   break;
                   
              case 1:
                   gamestatus=SELECT_MAP;
                   break;
                   
              case 2:
                   gb.begin();
                   break;
            } 
          }
          break;
           
     case GAMEOVER :
          if(gb.buttons.pressed(BTN_A)){
            
            gamestatus=SELECT_MAP;
          }
          break;
     
     case RUNNING :
          if(gb.buttons.repeat(BTN_A,1) && power<10){
            power = power+1;
          }else{
            if(power>0 ){
              gamestatus=ANIMFIRE;
              float dir;
              if(allPlayer[currentPlayer].dir==1){
                dir = 1;
                projPositionX = (allPlayer[currentPlayer].x+4);
              }else{
                dir = -1;
                projPositionX = (allPlayer[currentPlayer].x-1);      
              }
              projPositionY = allPlayer[currentPlayer].y-1;
              projTrajX = (((trajParamX[angle-1])/5)*(power/3))*dir;
              projTrajY = ((trajParamY[angle-1])/5)*(power/3);
            }else{
              if(gb.buttons.repeat(BTN_UP,1))   {if(angle>1) {angle = angle-1;}}
              if(gb.buttons.repeat(BTN_DOWN,1)) {if(angle<15){angle = angle+1;}}
              
              if(gb.buttons.pressed(BTN_RIGHT)){
                allPlayer[currentPlayer].dir = 1;
              }else{
                if(gb.buttons.repeat(BTN_RIGHT,1)){
                  if(gb.display.getPixel(allPlayer[currentPlayer].x+4,allPlayer[currentPlayer].y+3)==0){
                    allPlayer[currentPlayer].x = allPlayer[currentPlayer].x + 1;
              } } }
              
              if(gb.buttons.pressed(BTN_LEFT)){
                allPlayer[currentPlayer].dir = 0;
              }else{
                if(gb.buttons.repeat(BTN_LEFT,1)){
                  if(gb.display.getPixel(allPlayer[currentPlayer].x-1,allPlayer[currentPlayer].y+3)==0){
                    allPlayer[currentPlayer].x = allPlayer[currentPlayer].x - 1;
              } } }
              
              if(gb.buttons.pressed(BTN_B)){
                if(jumpStatus==0 && power==0 && gb.display.getPixel(allPlayer[currentPlayer].x,allPlayer[currentPlayer].y-3)==0 && gb.display.getPixel(allPlayer[currentPlayer].x+3,allPlayer[currentPlayer].y-3)==0)
                { 
                  outpt_soundfx(5,0);
                  jumpStatus=6;
                }
              }
              if(gb.buttons.pressed(BTN_C)){
                setting=0;
                gamestatus=PAUSE;
              }
            }
          }
          break;
  } // end switch
}



// ##################################################################################################################################
// ################## PLAYER RELATED FUNCTIONS ######################################################################################
// ##################################################################################################################################


//##################################################################
//##### DEFINE PLAYERS POSITION ####################################
void fnctn_definePlayer(){
  byte tmpY;
  byte team;
  byte check;
  byte tmp1;
  byte tmp2; 
  byte randm[21];
  
  //create a table containing 'random' number between 1 and 21, and mix them.
  for(byte rdm=0;rdm<21;rdm++){randm[rdm] = rdm;}
  for(byte rdm=0;rdm<21;rdm++){
    tmp1 = random(0,21);
    tmp2 = randm[rdm];
    randm[rdm] = randm[tmp1];
    randm[tmp1] = tmp2;
  }
  for(byte i=0;i<nbPlayer*nbTeam;i++){
    allPlayer[i].dead = 0;
    allPlayer[i].life = 3;
    allPlayer[i].team = i / nbPlayer;
    allPlayer[i].fall=0;
    allPlayer[i].isIA = 0; //All player are human
 
    
    //Define position
    allPlayer[i].x = randm[i]*4;
    
    //Find Y available position
    check=0;
    switch(randm[i]%2){
      case 1:
           for(byte Y=11; Y>0; Y--){
             if(gb.display.getPixel(randm[i],Y)==0 && check==0){
               gb.display.drawPixel(randm[i],Y);
               allPlayer[i].y=Y*4;
               check=1;
             }
           }
           if(check==0){
             landscapeZip[randm[i]][5]=0;
             allPlayer[i].y=20;
           }
           break;
       
       case 0:
            tmpY=0;
            for(byte Y = 0; Y <11; Y++){
              if(gb.display.getPixel(randm[i],Y)==0 && check==0){
                tmpY=Y;
              }else{
                if(tmpY!=0 && check==0){
                  gb.display.drawPixel(randm[i],tmpY);
                  allPlayer[i].y=tmpY*4;
                  check=1;
                }
              }
            }
            if(check==0){
              landscapeZip[randm[i]][5]=0;
              allPlayer[i].y=20;
            }
            break;
    }
    
    //initial direction (1:right, -1:left)
    if(randm[i]>10){allPlayer[i].dir = -1;}
    else{allPlayer[i].dir = 1;}
  }
}

//##################################################################
//##################################################################
void fnctn_checkJump(){
  if(jumpStatus==6){allPlayer[currentPlayer].y=allPlayer[currentPlayer].y-3;}
  if(jumpStatus==5){allPlayer[currentPlayer].y=allPlayer[currentPlayer].y-2;}
  if(jumpStatus==4){allPlayer[currentPlayer].y=allPlayer[currentPlayer].y-1;}
  
  if(jumpStatus>0){jumpStatus=jumpStatus-1;}
}

//##################################################################
//##################################################################
void fnctn_checkPlayerPos(){
  //check all players
  for(byte pl=0;pl<nbPlayer*nbTeam;pl++){
    
    if(gb.display.getPixel(allPlayer[pl].x+2,allPlayer[pl].y+4)==0 && gb.display.getPixel(allPlayer[pl].x+1,allPlayer[pl].y+4)==0){
      for(byte calcFall=0; calcFall<allPlayer[pl].fall+1;calcFall++){
        if(gb.display.getPixel(allPlayer[pl].x+2,allPlayer[pl].y+4)==0 && gb.display.getPixel(allPlayer[pl].x+1,allPlayer[pl].y+4)==0){
          if(allPlayer[pl].y>48 && allPlayer[pl].y<200)
          {
            //player is dying. increment team death counter
            if(allPlayer[pl].dead==0){
              teamInfo[allPlayer[pl].team].nbAlive = teamInfo[allPlayer[pl].team].nbAlive-1;
              allPlayer[pl].life=0;
              allPlayer[pl].dead=1;
              check_gameOver();
            }
          }else{
            allPlayer[pl].y=allPlayer[pl].y+1;
          }
        }
      }
      allPlayer[pl].fall=allPlayer[pl].fall + 1;
    }else{
      allPlayer[pl].fall=0;
    }
    if(gb.display.getPixel(allPlayer[pl].x+2,allPlayer[pl].y+3)==1 || gb.display.getPixel(allPlayer[pl].x+1,allPlayer[pl].y+3)==1 && allPlayer[pl].dead==0){
      allPlayer[pl].y=allPlayer[pl].y-1;
    }
  }
}

//##################################################################
//##################################################################
void fnctn_nextPlayer(){
  //byte tmpcrntTeam = currentTeam;
  do{
    currentTeam = (currentTeam + 1)%nbTeam;
  }while(teamInfo[currentTeam].nbAlive==0);
  
  //if(tmpcrntTeam==currentTeam){gamestatus = GAMEOVER;}
  //else{
    do{
      currentPlayer = (currentTeam*nbPlayer)+(teamInfo[currentTeam].lastPlayer+1)%nbPlayer;
      teamInfo[currentTeam].lastPlayer = (teamInfo[currentTeam].lastPlayer+1)%nbPlayer;
    }while(allPlayer[currentPlayer].dead==1);
    power = 0;
    angle = 8;
    gamestatus = RUNNING;
  //}
}

//##################################################################
//##################################################################
void fnctn_checkDead(){
  //check all players
  for(byte i=0;i<nbPlayer*nbTeam;i++){
    //if player is still alive
    if(allPlayer[i].dead==0){
      //if player is hitten by bullet
      if(allPlayer[i].x <= projPositionX && allPlayer[i].x+4 >= projPositionX && allPlayer[i].y <= projPositionY && allPlayer[i].y+4 >= projPositionY ){
        allPlayer[i].life = allPlayer[i].life-1;
        if(allPlayer[i].life<=0){
          teamInfo[allPlayer[i].team].nbAlive = teamInfo[allPlayer[i].team].nbAlive-1;
          allPlayer[i].dead=1;
          check_gameOver();
        }
      } 
      
      //if player is out of map
      if((allPlayer[i].y>48) || (allPlayer[i].x<-3) || (allPlayer[i].x>83)){
        teamInfo[allPlayer[i].team].nbAlive = teamInfo[allPlayer[i].team].nbAlive-1;
        allPlayer[i].life=0;
        allPlayer[i].dead=1;
        check_gameOver();
      }
    }    
  }
}

//##################################################################
//##################################################################
void check_gameOver(){
  byte nbAliveTeam = 0;
  gamestatus=GAMEOVER;
  for(byte i=0;i<nbTeam;i++){
    if(teamInfo[i].nbAlive>0){nbAliveTeam=nbAliveTeam+1;}
  }
  if(nbAliveTeam>1){gamestatus = RUNNING;}
}




// ##################################################################################################################################
// ################## BULLET RELATED FUNCTIONS ######################################################################################
// ##################################################################################################################################

//##################################################################
//##################################################################
void fn_nextProjPosition(){
  projPositionX = projPositionX + projTrajX;
  projPositionY = projPositionY + projTrajY;
  projTrajX = projTrajX + 0; //Wind
  projTrajY = projTrajY + (0.23); //Gravity
}

//##################################################################
//##################################################################
void fn_checkCollision(){
  byte check=0;
  float evalProjPosX = projPositionX; 
  float evalProjPosY = projPositionY; 
  float evalProjTrajX;
  float evalProjTrajY;
  float dir;
  
  if(allPlayer[currentPlayer].dir==1){dir = 1;}
  else{dir = -1;}
  
  evalProjPosX = projPositionX;
  

  if(projPositionX>84){gamestatus=BOOM;}
  if(projPositionX<0){gamestatus=BOOM;}
  if(projPositionY>48){gamestatus=BOOM;}
    
  evalProjTrajX = projTrajX/30;
  evalProjTrajY = projTrajY/30;
  
  
  while(check==0){
    if(evalProjPosX>projPositionX+projTrajX){check=1;}
    if(evalProjPosY>projPositionY+projTrajY){check=1;}
    if(gb.display.getPixel(evalProjPosX,evalProjPosY)==1){
      projPositionX = evalProjPosX;
      projPositionY = evalProjPosY;
      check=1;
      gamestatus=BOOM;
      fnctn_checkDead();
    }
    evalProjPosX = evalProjPosX + evalProjTrajX;
    evalProjPosY = evalProjPosY + evalProjTrajY;

  }
}



// ##################################################################################################################################
// ################# LANDSCAPE RELATED FUNCTIONS ####################################################################################
// ##################################################################################################################################

//##################################################################
//##################################################################
void fnctnt_loading(){
  timer = timer - 1;
  if(timer == 0){gamestatus=RUNNING;}
}

//##################################################################
//##################################################################
void fnctn_newlevel() {
  // create landscape array from level bitmap    
  gb.display.setColor(WHITE);
  gb.display.fillRect(0,0,21,12);
  gb.display.setColor(BLACK);
  gb.display.drawBitmap(0,0,levels[screen]);

  fnctn_buildLandscape();
  fnctn_definePlayer();
      
  gb.display.setColor(WHITE);
  gb.display.fillRect(0,0,21,12);
  gb.display.setColor(BLACK);
  gamestatus=LOADING;
}


//##################################################################
//##################################################################
void fnctn_buildLandscape(){
  for (int y=0; y<12; y++) {
    for (int x=0; x<21; x++) {
      int L0 = gb.display.getPixel(x,y);
      int L1=1;
      int L2=1;
      int L3=1;
      int L4=1;
      int L5=1;
      int L6=1;
      int L7=1;
      int L8=1;
      
      if (x>0 and y>0)   { L1 = gb.display.getPixel(x-1,y-1); }
      if (y>0)           { L2 = gb.display.getPixel(x,y-1); }
      if (x<20 and y>0)  { L3 = gb.display.getPixel(x+1,y-1); }
      if (x>0)           { L4 = gb.display.getPixel(x-1,y); }
      if (x<20)          { L5 = gb.display.getPixel(x+1,y); }
      if (x>0 and y<11)  { L6 = gb.display.getPixel(x-1,y+1); }
      if (y<11)          { L7 = gb.display.getPixel(x,y+1); }
      if (x<20 and y<11) { L8 = gb.display.getPixel(x+1,y+1); }
      
      fnctn_unzip(x, y, L0, L1, L2, L3, L4, L5, L6, L7, L8);   
    }
  }
}

//##################################################################
//##################################################################
void fnctn_unzip(int x, int y, int L0, int L1, int L2, int L3, int L4, int L5, int L6, int L7, int L8){
  int L=0;
  int Lzip=0;
  
  if (L0==1) {
    L=1;
    Lzip=1;
    if(L1==0 && L2==0 && L4==0 && L5==1 && L7==1 && L8==1){L=2;}
    if(L2==0 && L3==0 && L4==1 && L5==0 && L6==1 && L7==1){L=3;}
    if(L1==1 && L2==1 && L4==1 && L5==0 && L7==0 && L8==0){L=4;}
    if(L2==1 && L3==1 && L4==0 && L5==1 && L6==0 && L7==0){L=5;}
    if(L1==0 && L2==0 && L3==0 && L4==0 && L5==0 && L7==1){L=6;}
    if(L1==0 && L2==0 && L4==0 && L5==1 && L6==0 && L7==0){L=7;}
    if(L2==1 && L4==0 && L5==0 && L6==0 && L7==0 && L8==0){L=8;}
    if(L2==0 && L3==0 && L4==1 && L5==0 && L7==0 && L8==0){L=9;}
    if(L1==0 && L2==0 && L3==1 && L4==0 && L5==0 && L6==1 && L7==0 && L8==0){L=10;}
    if(L1==1 && L2==0 && L3==0 && L4==0 && L5==0 && L6==0 && L7==0 && L8==1){L=11;}
    if(L1==0 && L2==0 && L3==0 && L4==0 && L5==0 && L6==0 && L7==0 && L8==1){L=12;}
    if(L1==0 && L2==0 && L3==0 && L4==0 && L5==0 && L6==1 && L7==0 && L8==0){L=13;}
    if(L1==1 && L2==0 && L3==0 && L4==0 && L5==0 && L6==0 && L7==0 && L8==0){L=14;}
    if(L1==0 && L2==0 && L3==1 && L4==0 && L5==0 && L6==0 && L7==0 && L8==0){L=15;}
  }else{
    L=16;
    if(L2==1 && L4==1 && L5==1 && L7==0){L=21;}
    if(L2==1 && L4==1 && L5==0 && L7==1){L=22;}
    if(L2==0 && L4==1 && L5==1 && L7==1){L=23;}
    if(L2==1 && L4==0 && L5==1 && L7==1){L=24;}        
    if(L2==1 && L4==1 && L5==0 && L7==0){L=17;}
    if(L2==1 && L4==0 && L5==1 && L7==0){L=18;}
    if(L2==0 && L4==0 && L5==1 && L7==1){L=19;}
    if(L2==0 && L4==1 && L5==0 && L7==1){L=20;}
    if(L1==1 && L2==1 && L3==0 && L4==1 && L5==1 && L6==0 && L7==1 && L8==1){L=25;}
    if(L1==0 && L2==1 && L3==1 && L4==1 && L5==1 && L6==1 && L7==1 && L8==0){L=26;}
    if(L1==1 && L2==1 && L3==1 && L4==1 && L5==1 && L6==1 && L7==1 && L8==0){L=27;}
    if(L1==1 && L2==1 && L3==1 && L4==1 && L5==1 && L6==0 && L7==1 && L8==1){L=28;}
    if(L1==0 && L2==1 && L3==1 && L4==1 && L5==1 && L6==1 && L7==1 && L8==1){L=29;}
    if(L1==1 && L2==1 && L3==0 && L4==1 && L5==1 && L6==1 && L7==1 && L8==1){L=30;}
  }
  landscapeZip[x][y]=Lzip;
  landscape[x][y]=L;
}

//##################################################################
//##################################################################
void fnctn_rebuildMap(){
  int mapx = projPositionX/4;
  int mapy = projPositionY/4;
  landscapeZip[mapx][mapy]=0;
  
  for (int y=0; y<12; y++) {
    for (int x=0; x<21; x++) {
      int L0 = landscapeZip[x][y];
      int L1=1;
      int L2=1;
      int L3=1;
      int L4=1;
      int L5=1;
      int L6=1;
      int L7=1;
      int L8=1;
      
      if (x>0 and y>0)   { L1 = landscapeZip[x-1][y-1]; }
      if (y>0)           { L2 = landscapeZip[x][y-1]; }
      if (x<20 and y>0)  { L3 = landscapeZip[x+1][y-1]; }
      if (x>0)           { L4 = landscapeZip[x-1][y]; }
      if (x<20)          { L5 = landscapeZip[x+1][y]; }
      if (x>0 and y<11)  { L6 = landscapeZip[x-1][y+1]; }
      if (y<11)          { L7 = landscapeZip[x][y+1]; }
      if (x<20 and y<11) { L8 = landscapeZip[x+1][y+1]; }
      
      fnctn_unzip(x, y, L0, L1, L2, L3, L4, L5, L6, L7, L8);
      
      if(L0==2){
        landscape[x][y]=31;
        landscapeZip[x][y]=2;
      }
      if(L0==3){
        landscape[x][y]=32;
        landscapeZip[x][y]=3;
      }
    }
  }
}




//##################################################################
//##################################################################
void fnctn_ia(){
  /*
  
  
  if(ia_power==0){

    byte target = cpuMem[currentPlayer/2].target;
    if(allPlayer[target].dead==1){
      byte check = 0;
      byte checkedPlayer = 0;
      while (check==0){
        if(allPlayer[checkedPlayer].dead==0 && allPlayer[checkedPlayer].team==0){
          target=checkedPlayer;
          cpuMem[currentPlayer/2].target=checkedPlayer;
          check=0;
        }else{
          checkedPlayer=checkedPlayer+1;
        }
      }
    }      
    /*     
    if(cpuMem[currentPlayer/2].collx1>allPlayer[target].x){//shoot should be shorter than shoot1
      if(cpuMem[currentPlayer/2].collx2>allPlayer[target].x){//shoot should be shorter than shoot2
        if(cpuMem[currentPlayer/2].collx1>cpuMem[currentPlayer/2].collx2){
          ia_power = cpuMem[currentPlayer/2].power2-1;
          ia_angle = cpuMem[currentPlayer/2].angle2;
        }else{
          ia_power = cpuMem[currentPlayer/2].power1-1;
          ia_angle = cpuMem[currentPlayer/2].angle1;
        }
      }else{//shoot should be longer than shoot2
      
      }
    }else{//shoot should be longer than shoot1
      if(cpuMem[currentPlayer/2].collx2>allPlayer[target].x){//shoot should be shorter than shoot2
        
      }else{//shoot should be longer than shoot2
      
      }
    }* /
    ia_power = 5;
    ia_angle = 4;
    timer = 10;
  }
  
  if(timer>0){
    timer = timer -1;
  }else{
    if(angle>ia_angle){
      angle = angle - 1;
      timer = 1;
    }else{
      if(power<ia_power){
        power = power + 1;
      }else{
        float dir;
        if(allPlayer[currentPlayer-1].dir==1){dir = 1;}
        else{dir = -1;}
        projPositionX = (allPlayer[currentPlayer-1].x+(trajParamX[angle-1]*dir));
        projPositionY = allPlayer[currentPlayer-1].y+trajParamY[angle-1];
        projTrajX = (((trajParamX[angle-1])/5)*(power/3))*dir;
        projTrajY = ((trajParamY[angle-1])/5)*(power/3);
        gamestatus=ANIMFIRE;
      }
    }
  }
  */
}

