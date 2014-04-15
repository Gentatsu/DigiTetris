//Author : Naval Bhandari 
//Email  : navs92@hotmail.com
//Left and right to move blocks, "A" button to rotate.
#include <DigiPixel.h>
#include <avr/pgmspace.h>

// leave the following line uncommented for use with a Digispark
//DigiPixel digiPixel(3,0,5,2,1);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

// leave the following line uncommented for use with an Arduino
DigiPixel digiPixel(5,2,6,4,3);  // LED Latch/Button Shift !load pin, LED/Button clock pin, LED Data Pin, LED Output Enable pin, Buttons data pin)

//Make the game harder/easier
#define playerSpeedSave 10; //Lower is faster
#define blockSpeedSave 150;

byte playerSpeed = playerSpeedSave; 
byte blockSpeed = blockSpeedSave;
//To display score at end of game
byte numberTable[30]PROGMEM = {
  0b01111110, 0b01000010, 0b01111110, 0b00100010, 0b01111110, 0b00000010, 0b01001110, 0b01001010, 0b01111010, 0b01000010, 0b01001010, 0b01111110, 0b01111000, 0b00001000, 0b01111110, 0b01111010, 0b01001010, 0b01001110, 0b01111110, 0b01001010, 0b01001110, 0b01000000, 0b01000000, 0b01111110, 0b01111110, 0b01001010, 0b01111110, 0b01111010, 0b01001010, 0b01111110};
int blocks[7][4][2] = {{{0,0},{1,0},{2,0},{3,0}}, {{0,0},{0,1},{1,0},{2,0}}, {{0,0},{1,0},{2,0},{2,1}}, {{0,0},{0,1},{1,0},{1,1}}, {{0,0},{1,0},{1,1},{2,1}}, {{0,0},{1,0},{2,0},{1,1}}, {{0,1},{1,1},{1,0},{2,0}} };
int currentBlock[4][2];
int blockHistory[64][2];
boolean gameOver = false;
byte clearedRows = 0;
byte blockX = 0;
byte blockY = 7;

void initGame()
{
  //Initiliases the positions and shiz of the invaders and resets some stuff. 
  gameOver = false;
  getNewBlock();
}
void setup()
{
  initGame();
}

void loop()
{
  check();
  saveGraphics();
  digiPixel.saveButtonStates(); 
  digiPixel.drawScreen(); 
}

void check()
{
  playerCheck();
  blockCheck();
  if (gameOver)
  {
    //Reset invader
    showDeath();
    initGame();
    clearedRows = 0;
  }
}

void saveGraphics(){
  digiPixel.clearScreen();
  for (int i=0; i<4; i++)
  {
    digiPixel.setPixel(blockX+currentBlock[i][0], blockY+currentBlock[i][1], 1);
  }
}

void playerCheck()
{
  playerSpeed --;
  if (playerSpeed == 0)
  {
    if (digiPixel.buttonLeftPressed )
    {
      if (blockX > 0) 
      {
        blockX --;
      }
    }
    else if (digiPixel.buttonRightPressed )
    {
      if (blockX < 5)
      {
        blockX++;
      } 
    }
    else if (digiPixel.buttonAPressed )
    {
      rotate();
    }
    playerSpeed = playerSpeedSave;
  }
}

void blockCheck()
{
  blockSpeed --;
  if (blockSpeed == 0)
  {
    blockSpeed = blockSpeedSave;
    if (blockY > 0)
    {
      blockY--;
    }
    else
    {
      initGame();
    }
  }
}
void getNewBlock()
{
  int k = random(0,6);
  memcpy(currentBlock, blocks[k], sizeof(blocks[k]));
  blockX = random(0,4);
  blockY = 7;
}


void showDeath()
{
  //Nicked from SnakePixel
  digiPixel.clearScreen();
  digiPixel.drawScreen();

  int scoreTens = clearedRows / 10;
  int scoreOnes = clearedRows - (scoreTens * 10);

  for (int index = 0; index < 3; index++)
  {
    digiPixel.bufferBlue[index] = pgm_read_dword(&numberTable[index + (3 * scoreTens)]);
    digiPixel.bufferBlue[index + 4] = pgm_read_dword(&numberTable[index + (3 * scoreOnes)]);
  }

  for (int j = 0; j < 100; ++j)
  {
    digiPixel.drawScreen();
  }
  delay(1000);
}

void rotate()
{
  int originX = (currentBlock[0][0] + currentBlock[1][0] + currentBlock[2][0] + currentBlock[3][0]) / 4;
  int originY = (currentBlock[0][1] + currentBlock[1][1] + currentBlock[2][1] + currentBlock[3][1]) / 4;
  for (int i=0; i<4; i++)
  {
     int dx = originX - currentBlock[i][0];
     int dy = originY - currentBlock[i][1];
     currentBlock[i][0] = originX + dy;
     currentBlock[i][1] = originY + -dx;
  }
}



