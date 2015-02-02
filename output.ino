//##################################################################
//##################################################################
void outpt_selectMap(){
  byte x,y,z;
  
  if((((gamelevel-1)/3)*17)+20>48){z=(((gamelevel-4)/3))*17;}
  else{z=0;}
  for (byte i=0; i<nbAvailableLevel; i++) {
    x = 4+((i%3)*27);
    y = 8+((i/3)*17)-z;
    gb.display.drawBitmap(x,y,levels[i]);
    gb.display.drawRect(x-1, y-1, 23, 14);
    if(i+1==gamelevel){gb.display.drawRect(x-2, y-2, 25, 16);}
  }
  
  gb.display.setColor(0);
  gb.display.fillRect(0, 0, 84, 6);
  gb.display.setColor(1);
  gb.display.cursorX = 28;
  gb.display.print("Level ");
  gb.display.print(gamelevel);
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
  for(int i=0;i<6;i++){    
    if(allPlayer[i].dead==0){
      if(allPlayer[i].dir==1){
        gb.display.drawBitmap(allPlayer[i].x,allPlayer[i].y,landscapetiles[30+allPlayer[i].team]);
      }else{
        gb.display.drawBitmap(allPlayer[i].x-1,allPlayer[i].y,landscapetiles[30+allPlayer[i].team],NOROT,FLIPH);
      }
    }else{
      gb.display.drawFastVLine(allPlayer[i].x+1,allPlayer[i].y,4);
      if(allPlayer[i].team==0){
        gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y+1,3);
      }else{
        gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y,3);
        gb.display.drawFastHLine(allPlayer[i].x,allPlayer[i].y+2,3);
      }
    }
  }
}

//##################################################################
//##################################################################
void outpt_power(){
  gb.display.setColor(0);
  gb.display.fillRect(0, 44, 12, 4);
  gb.display.setColor(1);
  gb.display.drawPixel(0,44);
  gb.display.drawPixel(0,47);
  gb.display.drawPixel(11,44);
  gb.display.drawPixel(11,47);
  
  for(int pwr=1; pwr<power+1; pwr++){
    gb.display.drawPixel(pwr,45);
    gb.display.drawPixel(pwr,46);
  }
}

//################################################################## 
//##################################################################
void outpt_cursor(){
  byte dir;
  byte displayX;
  byte displayCenter;
  if(allPlayer[currentPlayer-1].dir==1){
    dir=1;
    displayX = 0;
    displayCenter = 2;
  }else{
    dir = -1;
    displayX = 3;
    displayCenter = 1;
  }
  gb.display.setColor(WHITE);
  gb.display.drawPixel(allPlayer[currentPlayer-1].x+((trajParamX[angle-1]+2)*dir)+displayX+0  ,allPlayer[currentPlayer-1].y+(trajParamY[angle-1])+1);
  gb.display.drawPixel(allPlayer[currentPlayer-1].x+displayCenter                             ,allPlayer[currentPlayer-1].y+1);
  gb.display.setColor(BLACK);
  gb.display.drawLine (allPlayer[currentPlayer-1].x+((trajParamX[angle-1]+2)*dir)+displayX-1  ,allPlayer[currentPlayer-1].y+(trajParamY[angle-1])+1,
                       allPlayer[currentPlayer-1].x+((trajParamX[angle-1]+2)*dir)+displayX+0  ,allPlayer[currentPlayer-1].y+(trajParamY[angle-1])+0);
  gb.display.drawLine (allPlayer[currentPlayer-1].x+((trajParamX[angle-1]+2)*dir)+displayX+1  ,allPlayer[currentPlayer-1].y+(trajParamY[angle-1])+1,
                       allPlayer[currentPlayer-1].x+((trajParamX[angle-1]+2)*dir)+displayX+0  ,allPlayer[currentPlayer-1].y+(trajParamY[angle-1])+2);
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
  gb.display.cursorX = 33;
  gb.display.cursorY = 17;
  gb.display.print("Pause");
}

//##################################################################
//##################################################################
void outpt_boom() {
  
  if(timer<2){
    gb.display.setColor(INVERT);
    gb.display.drawPixel(projPositionX-1,projPositionY-1);
  }
  
  if(timer==2){
    gb.display.setColor(WHITE);
    gb.display.drawPixel(projPositionX-1,projPositionY-1);
    gb.display.setColor(INVERT);
    gb.display.drawLine(projPositionX-2,projPositionY-1 ,projPositionX-1 ,projPositionY-2);
    gb.display.drawLine(projPositionX  ,projPositionY-1 ,projPositionX-1 ,projPositionY  );
  }
  
  if(timer==3){
    gb.display.setColor(WHITE);
    gb.display.fillRect(projPositionX-1,projPositionY-1, 2, 2);
    gb.display.setColor(INVERT);
    gb.display.drawLine(projPositionX-1 ,projPositionY-2 ,projPositionX   ,projPositionY-2);
    gb.display.drawLine(projPositionX+1 ,projPositionY-1 ,projPositionX+1 ,projPositionY  );
    gb.display.drawLine(projPositionX-2 ,projPositionY-1 ,projPositionX-2 ,projPositionY  );
    gb.display.drawLine(projPositionX-1 ,projPositionY+1 ,projPositionX   ,projPositionY+1);
  }
  
  if(timer==4){
    fnctn_rebuildMap();
  }
  
  if(timer==5){
    gb.display.setColor(WHITE);
    gb.display.drawPixel(projPositionX,projPositionY);
    gb.display.setColor(INVERT);
    gb.display.drawLine(projPositionX-1,projPositionY ,projPositionX ,projPositionY-1);
    gb.display.drawLine(projPositionX  ,projPositionY+1 ,projPositionX+1 ,projPositionY  );
  }
  
  if(timer==6){
    gb.display.setColor(INVERT);
    gb.display.drawPixel(projPositionX,projPositionY);
  }
  
  timer = timer + 1;
  
  if(timer>=8){
    fnctn_nextPlayer();
  }
  
  
}

//##################################################################
//##################################################################
void outpt_missed() {
  gb.display.cursorX = 31;
  gb.display.cursorY = 17;
  gb.display.print("Missed");
}


//##################################################################
//##################################################################
void outpt_gameOver(){
  if(nbAlivePlayerTeam0<1){
    gb.display.setColor(WHITE);
    gb.display.fillRect(4,5,76,24);
    gb.display.setColor(BLACK);
    gb.display.drawRect(5,6,74,22);
    gb.display.cursorY = 11;
    gb.display.cursorX = 22;
    gb.display.println("Game Over");
    gb.display.cursorX = 7;
    gb.display.println("Alien erased all!");   
  }else{
    gb.display.setColor(WHITE);
    gb.display.fillRect(2,5,80,24);
    gb.display.setColor(BLACK);
    gb.display.drawRect(3,6,78,22);
    gb.display.cursorY = 8;
    gb.display.cursorX = 10;
    gb.display.println("Congratulations!");
    gb.display.cursorX = 14;
    gb.display.println("Human cleaned"); 
    gb.display.cursorX = 16;
    gb.display.println("this planet!");
  }
  
}




//##################################################################
//##################################################################
void outpt_title() {
  gb.titleScreen(gamelogo);
}

//######################
//######################
void outpt_soundfx(int fxno, int channel) {
  gb.sound.command(0,soundfx[fxno][6],0,channel); // set volume
  gb.sound.command(1,soundfx[fxno][0],0,channel); // set waveform
  gb.sound.command(2,soundfx[fxno][5],-soundfx[fxno][4],channel); // set volume slide
  gb.sound.command(3,soundfx[fxno][3],soundfx[fxno][2]-58,channel); // set pitch slide
  gb.sound.playNote(soundfx[fxno][1],soundfx[fxno][7],channel); // play note
}





