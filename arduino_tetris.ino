#include <Sprite.h>    // library needed for Matrix.h http://wiring.org.co/reference/libraries/Sprite/index.html
#include <Matrix.h>    // library to easily send output to the led matrix http://wiring.org.co/reference/libraries/Matrix/index.html
#include <MsTimer2.h>  // library to control of the timer 2 interupt http://playground.arduino.cc/Main/MsTimer2
#include <TimerOne.h>  // library to control of the timer 1 interupt http://playground.arduino.cc/code/timer1

#define RED 1
#define GREEN 2
#define ORANGE 3

const int leftPin = 2;            // the pin of the left button
const int downPin = 3;            // the pin of the down button
const int rightPin = 4;           // the pin of the right button
const int rotateLeftPin = 5;      // the pin of the rotateleft button
const int rotateRightPin = 6;     // the pin of the rotateright button

const int numChips = 2;           // number of MAX7221 chips in each matrix
//                 DIN, CLK, LOAD, #chips
Matrix green = Matrix(9, 8, 7, numChips);    // the green 8 x 16 led matrix
Matrix red = Matrix(12, 11, 10, numChips);   // the red 8 x 16 led matrix

int backgroundBlocks [8][20];     //8 x 16 display + 4 x 8 blocks above for blocks to spawn in
int displayScreen [8][20];        //8 x 16 display + 4 x 8 blocks above for blocks to spawn in
int blockType ;                   //7 different block types
int state;                        //4 possible rotation states
const int activeBlocks [28][4][4] = {  
                           {{0,0,1,0},    //     blockType + 7(state)
                            {0,1,1,0},    // 0 =    0      + 7(0)
                            {0,1,0,0},
                            {0,0,0,0}}  ,
                         
                           {{0,2,0,0},    // 1 =    1      + 7(0)
                            {0,2,2,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,2,0,0},    // 2 =    2      + 7(0)
                            {0,2,2,0},
                            {0,0,2,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 3 =    3      + 7(0)
                            {0,3,3,0},
                            {0,3,3,0},
                            {0,0,0,0}} ,
                            
                           {{0,3,0,0},    // 4 =    4      + 7(0)
                            {0,3,0,0},
                            {0,3,3,0},
                            {0,0,0,0}} ,
                            
                           {{0,1,1,0},    // 5 =    5      + 7(0)
                            {0,1,0,0},
                            {0,1,0,0},
                            {0,0,0,0}} ,
                           
                           {{0,0,2,0},    // 6 =    6      + 7(0)
                            {0,0,2,0},
                            {0,0,2,0},
                            {0,0,2,0}} ,
                            
                            {{0,0,0,0},  // 7 =    0      + 7(1)
                            {1,1,0,0},    
                            {0,1,1,0},
                            {0,0,0,0}}  ,
                        
                           {{0,0,0,0},    // 8 =    1      + 7(1)
                            {2,2,2,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 9 =    2      + 7(1)
                            {0,2,2,0},
                            {2,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 10 =    3      + 7(1)
                            {0,3,3,0},
                            {0,3,3,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 11 =    4      + 7(1)
                            {3,3,3,0},
                            {3,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 12 =    5      + 7(1)
                            {1,1,1,0},
                            {0,0,1,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 13 =    6      + 7(1)
                            {0,0,0,0},
                            {2,2,2,2},
                            {0,0,0,0}} ,

                           {{0,1,0,0},    // 14 =    0      + 7(2)
                            {1,1,0,0},    
                            {1,0,0,0},
                            {0,0,0,0}}  ,
                            
                           {{0,2,0,0},    // 15 =    1      + 7(2)
                            {2,2,0,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{2,0,0,0},    // 16 =    2      + 7(2)
                            {2,2,0,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 17 =    3      + 7(2)
                            {0,3,3,0},
                            {0,3,3,0},
                            {0,0,0,0}} ,
                            
                           {{3,3,0,0},    // 18 =    4      + 7(2)
                            {0,3,0,0},
                            {0,3,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,1,0,0},    // 19 =    5      + 7(2)
                            {0,1,0,0},
                            {1,1,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,2,0,0},    // 20 =    6      + 7(2)
                            {0,2,0,0},
                            {0,2,0,0},
                            {0,2,0,0}} ,

                           {{1,1,0,0},    // 21 =    0      + 7(3)
                            {0,1,1,0},    
                            {0,0,0,0},
                            {0,0,0,0}}  ,
                            
                           {{0,2,0,0},    // 22 =    1      + 7(3)
                            {2,2,2,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,2,2,0},    // 23 =    2      + 7(3)
                            {2,2,0,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 24 =    3      + 7(3)
                            {0,3,3,0},
                            {0,3,3,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,3,0},    // 25 =    4      + 7(3)
                            {3,3,3,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{1,0,0,0},    // 26 =    5      + 7(3)
                            {1,1,1,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 27 =    6      + 7(3)
                            {2,2,2,2},
                            {0,0,0,0},
                            {0,0,0,0}}
                          };
                                                                                                    
int blockColumn;             //the x value of the left most row in the 4x4 active block placeholder
int blockRow;                //the y value of the bottom column in the 4x4 active block placeholder

int rowToBeDeleted;
bool inGame = false;
bool justLost = false;
int brightness = 8;
long fallRate;

void setup() 
{
  pinMode(leftPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(rotateLeftPin, INPUT);
  pinMode(rotateRightPin, INPUT);
  green.setBrightness(brightness);  // sets brightness of the led matrix values range from 0 to 15
  red.setBrightness(brightness);
  randomSeed(analogRead(1));
}

void newGame()
{
  green.clear();                        // clears the green display matrix
  red.clear();                          // clears the red display matrix
  resetArrays();
  newBlock();
  MsTimer2::set(10, updateDisplay);     // initialises timer2, timer2 calls updateDisplay() every 10ms
  MsTimer2::start();                    // starts timer2
  fallRate = 1000000;                   // set initial fall rate to 1000000 nanoseconds
  Timer1.initialize(fallRate);          // initialises timer1
  Timer1.attachInterrupt(fall);         // timer1 calls fall() 
}
  
void updateDisplay() // this function is called by a timer interupt every 10 milliseconds
{
  combineArrays();                      // adds the latest position of the active block to the displayScreen Array
  displayArray();                       // outputs the displayScreen Array
}

void combineArrays()  //updates the displayScreen array by combining the current positions of the backgroundBlocks and the activeBlocks
{
  //first make an exact copy of the backgroundBlocks
  for(byte y = 0; y < 16; y++)
    for(byte x = 0; x < 8; x++)
      displayScreen[x][y] = backgroundBlocks[x][y];
      
  //then add the non zero elements of the activeBlocks
  for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++)
      if(activeBlocks[blockType + (7 * state)][x][y] > 0)
        displayScreen[x + blockColumn][y + blockRow] = activeBlocks[blockType + (7 * state)][x][y];
}

void displayArray()  // cycles through each pixel in displayScreen and outputs corresponding colour
{
  for(byte y = 0; y < 16; y++)
  {
    for(byte x = 0; x < 8; x++)
    {
      byte pixel = displayScreen[x][y];
      if(pixel == RED)  
      {
        red.write(y, x, HIGH);
        green.write(y, x, LOW);
      }
      else if(pixel == GREEN)
      {
        red.write(y, x, LOW);
        green.write(y, x, HIGH);
      }
      else if(pixel == ORANGE)
      {
        red.write(y, x, HIGH);        // orange colour achieved by putting both green 
        green.write(y, x, HIGH);      // and red leds on at the same time
      }
      else
      {
        red.write(y, x, LOW);
        green.write(y, x, LOW);
      }
    }
  } 
}

void resetArrays()  // resets arrays to all zeros
{
  for(byte y = 0; y < 20; y++)
     for(byte x = 0; x < 8; x++)
        displayScreen[x][y] = backgroundBlocks[x][y] = 0;
}

void newBlock() // 
{
  blockColumn = 2;       
  blockRow = 14;          // new block spawns at 2,14
  blockType = random(7);  // blocktype is a random between 0 and 6 inclusive
  state = 0;              // reset state back to spawn block state
}

void fall()               // this function is called by a timer interupt every fallRate nanoseconds
{
  if(canMoveDown())
    blockRow--;
  else   //block cant move
  {
    moveActiveBlocksToBackground();      // adds the activeBlocks to the background display
    deleteFullRows();                    // deletes any full rows caused by now stationary block
    if(spaceForNewBlock())               // checks can a new block spawn
      newBlock();                  
    else  // new block cant spawn so game over
    {                           
      MsTimer2::stop();                  // stops timer2 interupt
      Timer1.detachInterrupt();          // stops timer1 interupt
      inGame = false;           
      justLost = true;
    }
  }
}

void tetrisText()      // displays the word 'TETRIS' to screen
{
  green.write(11, 1, HIGH); // letter T in orange
  green.write(12, 1, HIGH);
  green.write(13, 1, HIGH);
  green.write(14, 1, HIGH);
  green.write(15, 0, HIGH);
  green.write(15, 1, HIGH);
  green.write(15, 2, HIGH);
  red.write(11, 1, HIGH);
  red.write(12, 1, HIGH);
  red.write(13, 1, HIGH);
  red.write(14, 1, HIGH);
  red.write(15, 0, HIGH);
  red.write(15, 1, HIGH);
  red.write(15, 2, HIGH);
  
  green.write(11, 4, HIGH); // letter E in green
  green.write(11, 5, HIGH);
  green.write(11, 6, HIGH);
  green.write(12, 4, HIGH);
  green.write(13, 4, HIGH);
  green.write(13, 5, HIGH);
  green.write(13, 6, HIGH);
  green.write(14, 4, HIGH);
  green.write(15, 4, HIGH);
  green.write(15, 5, HIGH);
  green.write(15, 6, HIGH);
  
  red.write(6, 1, HIGH);  //letter T in red
  red.write(7, 1, HIGH);
  red.write(8, 1, HIGH);
  red.write(9, 0, HIGH);
  red.write(9, 1, HIGH);
  red.write(9, 2, HIGH);
  
  green.write(6, 4, HIGH);  // letter R in orange
  green.write(6, 6, HIGH);
  green.write(7, 4, HIGH);
  green.write(7, 5, HIGH);
  green.write(8, 4, HIGH);
  green.write(8, 6, HIGH);
  green.write(9, 4, HIGH);
  green.write(9, 5, HIGH);
  red.write(6, 4, HIGH);  
  red.write(6, 6, HIGH);
  red.write(7, 4, HIGH);
  red.write(7, 5, HIGH);
  red.write(8, 4, HIGH);
  red.write(8, 6, HIGH);
  red.write(9, 4, HIGH);
  red.write(9, 5, HIGH);
  
  green.write(0, 1, HIGH);  // letter I in green
  green.write(1, 1, HIGH);
  green.write(2, 1, HIGH);
  green.write(3, 1, HIGH);
  green.write(4, 1, HIGH);
  
  red.write(0, 4, HIGH);   //letter S in red
  red.write(0, 5, HIGH);
  red.write(0, 6, HIGH);
  red.write(1, 6, HIGH);
  red.write(2, 4, HIGH);
  red.write(2, 5, HIGH);
  red.write(2, 6, HIGH);
  red.write(3, 4, HIGH);
  red.write(4, 4, HIGH);
  red.write(4, 5, HIGH);
  red.write(4, 6, HIGH);
  
  delay(100);
}

void deleteFullRows()
{
   for(int i = 0; i < 4; i++) // checking 4 times if there are full rows to be deleted
   {
      if(rowIsFull())
      {
        deleteRow();
        speedUpFalling();
      }
   }
} 

void speedUpFalling()
{
  if(fallRate > 50000)   // to stop fallRate decreasing to zero
    fallRate -= 50000;   // blocks falling speeds up by 50000 nanoseconds every full row made

  Timer1.setPeriod(fallRate);  // updates the timer inetupt with the new fallRate
}

bool spaceForNewBlock()  // returns true if there is space for the new block to be spawned at the top of the matrix
{
  if(canBeMoved(2, 14))
    return true;
  else
    return false;
}

void fillScreen()       // fills pixels one at a time with green at the end of the game one at a time starting from the top
{
  for(int y = 15; y > -1; y--)
  {
    for(int x = 0; x < 8; x++)
    {
        displayScreen[x][y] = 2;    // turns the pixel green
        displayArray();             // outputs the pixel change to the screen
    }
  } 
}

bool canBeMoved(int newCol, int newRow)  // return true if the there is empty pixels for the block to move in to
{ 
  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 4; x++)
    {
      if(activeBlocks[blockType + (7 * state)][x][y] > 0)   // finds the positions of the non-blank blocks in the 4x4 matrix
      {
        int boardX = x + newCol;                            // calculates the corresponding x position on the display board
        int boardY = y + newRow;                            // calculates the corresponding y position on the display board
        if(boardX < 0 ||  boardX > 7 || boardY < 0)         // if the pixel is outside the boundaries of the board
          return false;
        if(backgroundBlocks[boardX][boardY] > 0)            // if the pixel is already full
          return false;
      }  
    }
  }
  return true;
}

bool canMoveDown()   // row - 1
{
  if(canBeMoved(blockColumn, blockRow -1))
    return true;
  else
    return false;
}

bool canMoveLeft()
{
  if(canBeMoved(blockColumn - 1, blockRow)) // column -1
    return true;
  else
    return false;
}

bool canMoveRight()
{
  if(canBeMoved(blockColumn + 1, blockRow)) // column + 1
    return true;
  else
    return false;
}

bool canBeRotated(int newState)
{
  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 4; x++)
    {
      if(activeBlocks[blockType + (7 * newState)][x][y] > 0) // finds the positions of the non-blank blocks in the 4x4 matrix
      {
        int boardX = x + blockColumn;                        // calculates the corresponding x position on the display board
        int boardY = y + blockRow;                           // calculates the corresponding y position on the display board
        if(boardX < 0 ||  boardX > 7 || boardY < 0)          // if the pixel is outside the boundaries of the board
          return false;
        
        if(backgroundBlocks[boardX][boardY] > 0)             // if the pixel is already full
          return false; 
      }  
    }
  }
  return true;
}

bool canRotateLeft()
{
  int nextState;
  if(state == 0)      // state to the left of 0 is 3
    nextState = 3;
  else
    nextState = state - 1;  
  
  if(canBeRotated(nextState))
    return true;
  else
    return false;
}

bool canRotateRight()
{
  int nextState;
  if(state == 3)      // state to the right of 3 is 0
    nextState = 0;
  else
    nextState = state + 1;

  if(canBeRotated(nextState))
    return true;
  else
    return false;
}

void moveActiveBlocksToBackground()
{
  combineArrays(); //updated the display array
  
  for(byte y = 0; y < 16; y++)
    for(byte x = 0; x < 8; x++)
      backgroundBlocks[x][y] = displayScreen[x][y];    // copy the displayScreen to the backgroundBlocks
}

bool rowIsFull()                // goes through all 16 rows and checks if they are full
{
  for(byte y = 0; y < 16; y++)
  {
    if((backgroundBlocks[0][y] > 0) && (backgroundBlocks[1][y] > 0) && (backgroundBlocks[2][y] > 0) && (backgroundBlocks[3][y] > 0) &&
        (backgroundBlocks[4][y] > 0) && (backgroundBlocks[5][y] > 0) && (backgroundBlocks[6][y] > 0) && (backgroundBlocks[7][y] > 0))
    {
       rowToBeDeleted = y;      // the row is returned as a global variable
       return true;
    }
  }
  return false; 
}

void deleteRow()   
{
  for(byte y = rowToBeDeleted; y < 16; y++)  
    for(byte x = 0; x < 8; x++)
      backgroundBlocks[x][y] = backgroundBlocks[x][y + 1];  // replaces row with row above 
}

void loop()
{
  delay(10);
  int rotateRightState = digitalRead(rotateRightPin);
  int leftState = digitalRead(leftPin);
  int rightState = digitalRead(rightPin);
  
  if(inGame) // if ingame
  {
    int downState = digitalRead(downPin);
    int rotateLeftState = digitalRead(rotateLeftPin);

    if(leftState == 1)      
      if(canMoveLeft())
        blockColumn--;

    if(downState == 1)    
      if(canMoveDown())
        blockRow--;
        
    if(rightState == 1)
      if(canMoveRight())
        blockColumn++;

    if(rotateRightState == 1)
    {
      if(canRotateRight())
      {
        if(state == 3)
          state = 0;  // state to the right of 3 is 0
        else
          state++;
      }
    }
    if(rotateLeftState == 1)
    {
      if(canRotateLeft())
      {
        if(state == 0)
          state = 3;   // state to the left of 0 is 3
        else
          state--;
      }
    }
  }
  else
  {
    if(justLost) // enters this loop only once after the game is lost
    {
      fillScreen();      
      green.clear();     // clears display
      red.clear();       // clears display
      justLost = false;  
    }
    tetrisText();        // prints tetris text splashscreen
    
    if(rotateRightState == 1)   // pressing rotateright starts a new game
    {
      inGame = true;
      newGame();
    }
    
    if((leftState == 1) && (brightness > 0))  // reduces brightness of both led matrices
    {
      brightness--;
      green.setBrightness(brightness);  
      red.setBrightness(brightness);
    }
      
    if((rightState == 1) && (brightness < 15)) // increases brightness of both led matrices
    {
      brightness++;
      green.setBrightness(brightness);  
      red.setBrightness(brightness);
    }
  }
}

