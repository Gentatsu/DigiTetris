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
<<<<<<< HEAD
int blockHistory[8][8];
=======
boolean blockHistory[8][8] = {false};
>>>>>>> 92371c664da77720bb47b164676c4961fa1cd751
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

void playerCheck()
{
  playerSpeed --;
  if (playerSpeed == 0)
  {
    int smallestX = currentBlock[0][0];
    int biggestX = currentBlock[0][0];
    for (int i=1; i < 4; i++)
    {
      if (currentBlock[i][0] < smallestX)
      {
        smallestX = currentBlock[i][0];
      }
      if (currentBlock[i][0] > biggestX)
      {
        biggestX = currentBlock[i][0];
      } 
    }
    if (digiPixel.buttonLeftPressed )
    {
      if (blockX + smallestX > 0) 
      {
        blockX --;
      }
    }
    else if (digiPixel.buttonRightPressed )
    {
      if (blockX + biggestX < 7)
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
    int smallestY = currentBlock[0][1];
    for (int i=1; i < 4; i++)
    {
      if (currentBlock[i][1] < smallestY)
      {
        smallestY = currentBlock[i][1];
      }
    }
    blockSpeed = blockSpeedSave;
<<<<<<< HEAD
    if (blockY + smallestY > 0)
=======
    int rightX = 0;
    int leftX = 8;
    for (int i=0; i<8; i++)
    {
      int biggestY = 0; 
      for (int j=0; j<8; j++)
      { 
        if (blockHistory[i][j]) 
        {
          if (j > biggestY)
          {
            biggestY = j;
          }
          if (i > rightX)
          {
            rightX = i;
          }
          if (i < leftX)
          {
            leftX = i;
          }
        }
      }  
    }
    if (blockY > 0) 
>>>>>>> 92371c664da77720bb47b164676c4961fa1cd751
    {
      blockY--;
    }
    else
    {
      for (int i=0; i<4; i++)
      {
        int x = currentBlock[i][0];
        int y = currentblock[i][1];
        blockHistory[x][y] = true;  
      }
      getNewBlock();
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
     while (currentBlock[i][0] + blockX < 0)
     {
       blockX++;
     }
     while (currentBlock[i][0] + blockX > 7)
     {
       blockX--;
     }
     while (currentBlock[i][1] + blockY < 0)
     {
       blockY++;
     }
     
  }
}

void saveGraphics(){
  digiPixel.clearScreen();
  for (int i=0; i<4; i++)
  {
    digiPixel.setPixel(blockX+currentBlock[i][0], blockY+currentBlock[i][1], 1);
  }
  for (int i=0; i<8;i++)
  {
    for (int j=0; j<8; j++)
    {
      if (blockHistory[i][j])
      {
        digiPixel.setPixel(i, j, 2);
      }
    }
  }
}



