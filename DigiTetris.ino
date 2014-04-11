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
byte blocks[7][4][2] = {{{0,0},{1,0},{2,0},{3,0}}, {{0,0},{0,1},{1,0},{2,0}}, {{0,0},{1,0},{2,0},{2,1}}, {{0,0},{0,1},{1,0},{1,1}}, {{0,0},{1,0},{1,1},{2,1}}, {{0,0},{1,0},{2,0},{1,1}}, {{0,1},{1,1},{1,0},{2,0}} };
byte currentBlock[4][2];
byte filledScreen[64];
boolean gameOver = false;
byte clearedRows = 0;
byte blockX;
byte blockY;

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
  //digiPixel.setPixel(playerX, 0,1);
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
      //rotate(-90);
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
    if (blockY < 0)
    {
      blockY--;
    }
  }
}
void getNewBlock()
{
  int k = random(0,6);
  memcpy(blocks[k], currentBlock, sizeof(currentBlock));
  blockX = random(0,5);
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

void rotate(float angle)
{
  for (int i; i<4; i++)
  {
     byte x = currentBlock[i][0];
     byte y = currentBlock[i][1];
     currentBlock[i][0] = cos(angle)*x - sin(angle)*y;
     currentBlock[i][1] = sin(angle)*x + cos(angle)*y;
  }
}



