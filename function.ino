//##################################################################
//##################################################################
void fnctn_checkbuttons() {
  if(gamestatus=="selectMap"){
    if(gb.buttons.pressed(BTN_DOWN) && gamelevel+3<=nbAvailableLevel){gamelevel = gamelevel + 3;}
    if(gb.buttons.pressed(BTN_UP)   && gamelevel-3>=1){gamelevel = gamelevel - 3;}
    if(gb.buttons.pressed(BTN_RIGHT) && gamelevel+1<=nbAvailableLevel){gamelevel = gamelevel + 1;}
    if(gb.buttons.pressed(BTN_LEFT) && gamelevel-1>=1){gamelevel = gamelevel - 1;}
    if(gb.buttons.pressed(BTN_A)){gamestatus="newLevel";}
  }
  
  if(gamestatus=="loading"){
    if(gb.buttons.released(BTN_A)){gamestatus="running";}
  }
  
  if(gamestatus=="loadingPause"){
    if(gb.buttons.released(BTN_A)){gamestatus="pause";}
  }
  
  if(gamestatus=="running"){
    if(gb.buttons.repeat(BTN_A,1) && power<10){
      power = power+1;
    }else{
      if(power>0 ){
        gamestatus="animFire";
        float dir;
        if(allPlayer[currentPlayer-1].dir==1){
          dir = 1;
          projPositionX = (allPlayer[currentPlayer-1].x+4);
        }else{
          dir = -1;
          projPositionX = (allPlayer[currentPlayer-1].x-1);      
        }
        projPositionY = allPlayer[currentPlayer-1].y-1;
        projTrajX = (((trajParamX[angle-1])/5)*(power/3))*dir;
        projTrajY = ((trajParamY[angle-1])/5)*(power/3);
      }else{
        if(gb.buttons.repeat(BTN_UP,1))   {if(angle>1) {angle = angle-1;}}
        if(gb.buttons.repeat(BTN_DOWN,1)) {if(angle<15){angle = angle+1;}}
        
        if(gb.buttons.pressed(BTN_RIGHT)){
          outpt_soundfx(3,1);
          allPlayer[currentPlayer-1].dir = 1;
        }else{
          if(gb.buttons.repeat(BTN_RIGHT,1)){
            outpt_soundfx(3,1);
            if(gb.display.getPixel(allPlayer[currentPlayer-1].x+4,allPlayer[currentPlayer-1].y+1)==0){
              allPlayer[currentPlayer-1].x = allPlayer[currentPlayer-1].x + 1;
        } } }
        
        if(gb.buttons.pressed(BTN_LEFT)){
          outpt_soundfx(3,1);
          allPlayer[currentPlayer-1].dir = 0;
        }else{
          if(gb.buttons.repeat(BTN_LEFT,1)){
            outpt_soundfx(3,1);
            if(gb.display.getPixel(allPlayer[currentPlayer-1].x-1,allPlayer[currentPlayer-1].y+1)==0){
              allPlayer[currentPlayer-1].x = allPlayer[currentPlayer-1].x - 1;
        } } }
        
        if(gb.buttons.pressed(BTN_B) && jumpStatus==0 && power==0){jumpStatus=6;}
        if(gb.buttons.pressed(BTN_C))     {gamestatus="selectMap";}
      }
    }
  }
  
  if(gamestatus=="pause"){
    if(gb.buttons.pressed(BTN_A)){gamestatus="loading";}
  }
  
  if(gamestatus=="missed"){
    if(gb.buttons.pressed(BTN_A)){
      fnctn_nextPlayer();
      gamestatus="loading";
    }
  }
  
  
  if(gamestatus=="gameover"){
    if(gb.buttons.pressed(BTN_A)){
      gamestatus="selectMap";
    }
  } 
}

//##################################################################
//##################################################################
void fnctn_newlevel() {
  // create landscape array from level bitmap    
  gb.display.setColor(0);
  gb.display.fillRect(0,0,21,12);
  gb.display.setColor(1);
  gb.display.drawBitmap(0,0,levels[screen]);

  fnctn_buildLandscape();
  fnctn_definePlayer();
  
    
  gb.display.setColor(0);
  gb.display.fillRect(0,0,21,12);
  gb.display.setColor(1);
  gamestatus="loading";
}

//##################################################################
//##################################################################
void fnctn_definePlayer(){
  int canonX;
  int canonY;
  int check;
  int tmp1;
  int tmp2; 
  int randm[21];
  

  for(int rdm=0;rdm<21;rdm++){randm[rdm] = rdm+1;}
  for(int rdm=0;rdm<21;rdm++){
    tmp1 = random(0,21);
    tmp2 = randm[rdm];
    randm[rdm] = randm[tmp1];
    randm[tmp1] = tmp2;
  }
  for(int i=0;i<6;i++){
    //IsDead
    allPlayer[i].dead = 0;
    
    //Team
    allPlayer[i].team = 0;
    if(i!=0){allPlayer[i].team = allPlayer[i-1].team+1;}
    if(allPlayer[i].team>1){allPlayer[i].team=0;}
    
    canonX = randm[i+((random(0,3))*6)];
       
    check=0;
    for(int Y = 11; Y > 0; Y--){
      if(gb.display.getPixel(canonX,Y)==0 && check==0){
      gb.display.drawPixel(canonX,Y);
      canonY=Y;
      check=1;
      }
    }
    allPlayer[i].x = canonX*4;
    allPlayer[i].y = canonY*4;
    allPlayer[i].dir = 1;
    if(canonX>10){allPlayer[i].dir = -1;}
    allPlayer[i].fall=0;
  }
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
void fn_nextProjPosition(){
  projPositionX = projPositionX + projTrajX;
  projPositionY = projPositionY + projTrajY;
  projTrajX = projTrajX + 0; //Wind
  projTrajY = projTrajY + (0.3); //Gravity
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
  
  if(allPlayer[currentPlayer-1].dir==1){dir = 1;}
  else{dir = -1;}
  
  evalProjPosX = projPositionX;
  

  if(projPositionX>84){gamestatus="boom";}
  if(projPositionX<0){gamestatus="boom";}
  if(projPositionY>48){gamestatus="boom";}
    
  evalProjTrajX = projTrajX/30;
  evalProjTrajY = projTrajY/30;
  
  
  while(check==0){
    if(evalProjPosX>projPositionX+projTrajX){check=1;}
    if(evalProjPosY>projPositionY+projTrajY){check=1;}
    if(gb.display.getPixel(evalProjPosX,evalProjPosY)==1){
      projPositionX = evalProjPosX;
      projPositionY = evalProjPosY;
      check=1;
      gamestatus="boom";
      fnctn_checkDead();
    }
    evalProjPosX = evalProjPosX + evalProjTrajX;
    evalProjPosY = evalProjPosY + evalProjTrajY;

  }
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
void fnctn_nextPlayer(){
  byte checkDead=1;
  byte checkTeam=1;
  if(currentPlayer-1>=0){checkTeam=allPlayer[currentPlayer-1].team;}
  
  if(checkTeam==0){
    previousPlayerTeam0 = currentPlayer;
    currentPlayer = previousPlayerTeam1;
  }else{
    previousPlayerTeam1 = currentPlayer;
    currentPlayer = previousPlayerTeam0;
  }
  
  while(checkDead==1){
    currentPlayer = currentPlayer + 1;
    if(currentPlayer>6){currentPlayer=1;}
    
    if(allPlayer[currentPlayer-1].dead==0 && allPlayer[currentPlayer-1].team!=checkTeam){
      checkDead=0; 
    }
  }  
  power = 0;
  angle = 8;
  timer = 0;
  gamestatus="running";
}

//##################################################################
//##################################################################
void fnctn_checkDead(){
  for(byte i=0;i<6;i++){
    if((allPlayer[i].x <= projPositionX && allPlayer[i].x+4 >= projPositionX && allPlayer[i].y <= projPositionY && allPlayer[i].y+4 >= projPositionY) || (allPlayer[i].y>50) || (allPlayer[i].x<-3) || (allPlayer[i].y>84)){
      if(allPlayer[i].dead==0){
        if(allPlayer[i].team==0){nbAlivePlayerTeam0 = nbAlivePlayerTeam0 - 1;}
        else{nbAlivePlayerTeam1 = nbAlivePlayerTeam1-1;}
      }
      allPlayer[i].dead=1;
    }
    
  }
  if(nbAlivePlayerTeam0==0 || nbAlivePlayerTeam1 == 0){
    outpt_soundfx(2,1);
    gamestatus="gameover";
    currentPlayer = 0;
    previousPlayerTeam0 = 0;
    previousPlayerTeam1 = 1;
    nbAlivePlayerTeam0=3;
    nbAlivePlayerTeam1=3;
    jumpStatus=0;
  }
}

//##################################################################
//##################################################################
void fnctn_checkPlayerPos(){
  for(byte pl=0;pl<6;pl++){
    if(gb.display.getPixel(allPlayer[pl].x+2,allPlayer[pl].y+4)==0 && gb.display.getPixel(allPlayer[pl].x+1,allPlayer[pl].y+4)==0){
      byte tmpFall = allPlayer[pl].fall+1;
      for(byte calcFall=0; calcFall<tmpFall;calcFall++){
        if(gb.display.getPixel(allPlayer[pl].x+2,allPlayer[pl].y+4)==0 && gb.display.getPixel(allPlayer[pl].x+1,allPlayer[pl].y+4)==0 && allPlayer[pl].y<50){
          allPlayer[pl].y=allPlayer[pl].y+1;
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
void fnctn_checkJump(){
  if(jumpStatus==6){allPlayer[currentPlayer-1].y=allPlayer[currentPlayer-1].y-3;}
  if(jumpStatus==5){allPlayer[currentPlayer-1].y=allPlayer[currentPlayer-1].y-2;}
  if(jumpStatus==4){allPlayer[currentPlayer-1].y=allPlayer[currentPlayer-1].y-1;}
  
  if(jumpStatus>0){jumpStatus=jumpStatus-1;}
}


//##################################################################
//##################################################################
void fnctn_ia(){
  if(ia_power==0){
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
        gamestatus="animFire";
      }
    }
  }
}

