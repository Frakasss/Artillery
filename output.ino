//##################################################################
//##################################################################
void outpt_selectMap(){
  byte x,y,z;
  
  if((((gamelevel)/3)*17)+20>48){
    z=(((gamelevel-3)/3))*17;
  }else{
    z=0;
    gb.display.drawRect(3, 7, 23, 14);
    gb.display.drawBitmap(4,9,options);
  }
  for (byte i=0; i<nbAvailableLevel; i++) {
    x = 4+(((i+1)%3)*27);
    y = 8+(((i+1)/3)*17)-z;
    gb.display.drawBitmap(x,y,levels[i]);
    gb.display.drawRect(x-1, y-1, 23, 14);
    if(i+1==gamelevel){gb.display.drawRect(x-2, y-2, 25, 16);}
  }

  gb.display.setColor(WHITE);
  gb.display.fillRect(0, 0, 84, 6);
  gb.display.setColor(BLACK);
    
  if(gamelevel==0){
    gb.display.cursorX = 27;
    gb.display.print("Settings");
    gb.display.drawRect(2, 6, 25, 16);
  }else{
    gb.display.cursorX = 28;
    gb.display.print("Level ");
    gb.display.print(gamelevel);
}
}


//##################################################################
//##################################################################
void outpt_options(){
  gb.display.cursorY = 0;
  gb.display.cursorX = 28;
  gb.display.print("Settings");
  gb.display.cursorY = 15;
  gb.display.cursorX = 16;
  gb.display.print("Player:  <");
  gb.display.print(nbTeam);
  gb.display.print(">");
  gb.display.cursorY = 25;
  gb.display.cursorX = 16;
  gb.display.print("Units:   <");
  gb.display.print(nbPlayer);
  gb.display.print(">");
  
  gb.display.cursorY = 35;
  gb.display.cursorX = 16;
  gb.display.print("Back");

  gb.display.drawBitmap(11,16+(setting*10),landscapetiles[30]);
}

//##################################################################
//##################################################################
void outpt_loading(){
  gb.display.print("Loading Map...");
}

//##################################################################
//##################################################################
void outpt_landscape() {
  for (byte y=0; y<12; y++) {
    for (byte x=0; x<21; x++) {
      if (landscape[x][y] != 0) {
        gb.display.drawBitmap(x*4,y*4,landscapetiles[landscape[x][y]-1]);
      }
    }
  }
}

//##################################################################
//##################################################################
void outpt_players() {
  for(byte i=0;i<nbPlayer*nbTeam;i++){    
    if(allPlayer[i].dead==0){
      gb.display.drawBitmap(allPlayer[i].x,allPlayer[i].y,landscapetiles[30+allPlayer[i].team]);
    }else{
      gb.display.drawFastVLine(allPlayer[i].x+1,allPlayer[i].y,4);
      switch(allPlayer[i].team){
        case 0:
          gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y+1,3);
          break;
        case 1:
          gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y,3);
          gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y+2,3);
          break;      
        case 2:
          gb.display.drawPixel(allPlayer[i].x,allPlayer[i].y);
          gb.display.drawPixel(allPlayer[i].x+2,allPlayer[i].y+1);
          break; 
        case 3:
          gb.display.drawPixel(allPlayer[i].x,allPlayer[i].y+1);
          gb.display.drawPixel(allPlayer[i].x+2,allPlayer[i].y);
          break; 
      }
    }
  }
}

//##################################################################
//##################################################################
void outpt_power(){
  if(power>0){
    gb.display.setColor(0);
    gb.display.fillRect(0, 0, 12, 4);
    gb.display.setColor(1);
    gb.display.drawPixel(0,0);
    gb.display.drawPixel(0,3);
    gb.display.drawPixel(11,0);
    gb.display.drawPixel(11,3);
    
    for(int pwr=1; pwr<power+1; pwr++){
      gb.display.drawPixel(pwr,1);
      gb.display.drawPixel(pwr,2);
    }
  }
}

//################################################################## 
//##################################################################
void outpt_life(){
  for(byte i=0;i<nbPlayer*nbTeam;i++){
    if(allPlayer[i].dead==0 && allPlayer[i].fall==0){
      if(i==currentPlayer && jumpStatus!=0){}
      else{
        gb.display.setColor(BLACK);
        switch(allPlayer[i].life){
          case 3:
               gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y-3,4); 
               break; 
          case 2:
               gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y-3,2); 
               break; 
        }
      }
    }
  }
}

//################################################################## 
//##################################################################
void outpt_team(){
  if(power==0){
    gb.display.print("P");
    gb.display.print(currentTeam+1);
  }
}

//################################################################## 
//##################################################################
void outpt_cursor(){
  
  byte dir;
  byte displayX;
  byte displayCenter;
  if(allPlayer[currentPlayer].dir==1){
    dir=1;
    displayX = 0;
    displayCenter = 2;
  }else{
    dir = -1;
    displayX = 3;
    displayCenter = 1;
  }
  gb.display.setColor(WHITE);
  //gb.display.drawPixel(allPlayer[currentPlayer].x+((trajParamX[angle-1]+2)*dir)+displayX+0  ,allPlayer[currentPlayer].y+(trajParamY[angle-1])+1);
  gb.display.drawPixel(allPlayer[currentPlayer].x+displayCenter                             ,allPlayer[currentPlayer].y+1);
  gb.display.setColor(BLACK);
  gb.display.drawLine (allPlayer[currentPlayer].x+((trajParamX[angle-1]+2)*dir)+displayX-1  ,allPlayer[currentPlayer].y+(trajParamY[angle-1])+1,
                       allPlayer[currentPlayer].x+((trajParamX[angle-1]+2)*dir)+displayX+0  ,allPlayer[currentPlayer].y+(trajParamY[angle-1])+0);
  gb.display.drawLine (allPlayer[currentPlayer].x+((trajParamX[angle-1]+2)*dir)+displayX+1  ,allPlayer[currentPlayer].y+(trajParamY[angle-1])+1,
                       allPlayer[currentPlayer].x+((trajParamX[angle-1]+2)*dir)+displayX+0  ,allPlayer[currentPlayer].y+(trajParamY[angle-1])+2);
                    
}

//##################################################################
//##################################################################
void outpt_projectile(){
  gb.display.drawPixel(projPositionX,projPositionY);
  gb.display.drawPixel(projPositionX+1,projPositionY);
  gb.display.drawPixel(projPositionX,projPositionY-1);
  gb.display.drawPixel(projPositionX+1,projPositionY-1);
}

//##################################################################
//##################################################################
void outpt_pause() {
  gb.display.cursorY = 0;
  gb.display.cursorX = 28;
  gb.display.print("Pause");
  gb.display.cursorY = 15;
  gb.display.cursorX = 16;
  gb.display.print("Back to Game");
  gb.display.cursorY = 25;
  gb.display.cursorX = 16;
  gb.display.print("Quit to New Map");
  gb.display.cursorY = 35;
  gb.display.cursorX = 16;
  gb.display.print("Quit to Homepage");

  gb.display.drawBitmap(11,16+(setting*10),landscapetiles[30]);
}

//##################################################################
//##################################################################
void outpt_boom() {
  switch(timer){
    case  0:
          gb.display.setColor(INVERT);
          gb.display.drawPixel(projPositionX-1,projPositionY-1);          
          break;
    
    case  1:
          gb.display.setColor(INVERT);
          gb.display.drawPixel(projPositionX-1,projPositionY-1);          
          break;    
    
    case  2:
          gb.display.setColor(WHITE);
          gb.display.drawPixel(projPositionX-1,projPositionY-1);
          gb.display.setColor(INVERT);
          gb.display.drawLine(projPositionX-2,projPositionY-1 ,projPositionX-1 ,projPositionY-2);
          gb.display.drawLine(projPositionX  ,projPositionY-1 ,projPositionX-1 ,projPositionY  );         
          break;
    
    case  3:
          gb.display.setColor(WHITE);
          gb.display.fillRect(projPositionX-1,projPositionY-1, 2, 2);
          gb.display.setColor(INVERT);
          gb.display.drawLine(projPositionX-1 ,projPositionY-2 ,projPositionX   ,projPositionY-2);
          gb.display.drawLine(projPositionX+1 ,projPositionY-1 ,projPositionX+1 ,projPositionY  );
          gb.display.drawLine(projPositionX-2 ,projPositionY-1 ,projPositionX-2 ,projPositionY  );
          gb.display.drawLine(projPositionX-1 ,projPositionY+1 ,projPositionX   ,projPositionY+1);
          break;
    
    case  5:
          gb.display.setColor(WHITE);
          gb.display.drawPixel(projPositionX,projPositionY);
          gb.display.setColor(INVERT);
          gb.display.drawLine(projPositionX-1,projPositionY ,projPositionX ,projPositionY-1);
          gb.display.drawLine(projPositionX  ,projPositionY+1 ,projPositionX+1 ,projPositionY  );
          break;
    
    case  6:
          gb.display.setColor(INVERT);
          gb.display.drawPixel(projPositionX,projPositionY);
          break;
    
  }
}


//##################################################################
//##################################################################
void outpt_gameOver(){
    gb.display.setColor(WHITE);
    gb.display.fillRect(4,5,76,24);
    gb.display.setColor(BLACK);
    gb.display.drawRect(5,6,74,22);
    gb.display.cursorY = 11;
    gb.display.cursorX = 17;
    gb.display.print("Player ");
    gb.display.print(currentTeam+1);
    gb.display.println(" win!");
    gb.display.cursorX = 7;
    gb.display.println("Others are losers!");
}

//##################################################################
//##################################################################
void outpt_title() {
  gb.titleScreen(gamelogo);
}

//##################################################################
//##################################################################
void outpt_soundfx(int fxno, int channel) {
  gb.sound.command(0,soundfx[fxno][6],0,channel); // set volume
  gb.sound.command(1,soundfx[fxno][0],0,channel); // set waveform
  gb.sound.command(2,soundfx[fxno][5],-soundfx[fxno][4],channel); // set volume slide
  gb.sound.command(3,soundfx[fxno][3],soundfx[fxno][2]-58,channel); // set pitch slide
  gb.sound.playNote(soundfx[fxno][1],soundfx[fxno][7],channel); // play note
}





