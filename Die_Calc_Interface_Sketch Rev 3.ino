/******************************************************************
  This is an example for the Adafruit RA8875 Driver board for TFT displays
  ---------------> http://www.adafruit.com/products/1590
  It is tested to work with displays in the Adafruit shop. Other displays
  may need timing adjustments and are not guanteed to work.

  Adafruit invests time and resources providing this open
  source code, please support Adafruit and open-source hardware
  by purchasing products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, check license.txt for more information.
  All text above must be included in any redistribution.
 ******************************************************************/


#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include <SD.h>
#include <Wire.h>
#include <Adafruit_STMPE610.h>


// tube control pins
// Tube ONES place
const int PIN_T1A = 2;         //PIN A
const int PIN_T1B = 3;         //PIN B
const int PIN_T1C = 4;         //PIN C
const int PIN_T1D = 5;         //PIN D

// Tube TENS Place
const int PIN_T2A = 6;         //PIN A
const int PIN_T2B = 7;         //PIN B
const int PIN_T2C = 8;         //PIN C
const int PIN_T2D = 9;         //PIN D

// Tube HUND Place
const int PIN_T3A = 10;         //PIN A
const int PIN_T3B = 11;         //PIN B
const int PIN_T3C = 12;         //PIN C
const int PIN_T3D = 13;         //PIN D

// For the Adafruit 5.0" TFT Screen, these are the default pins on a MEGA2560.
// VN = +5vdc
// GND = Ground
// CLK = D52
// MISO = D50
// MOSI = D51
#define RA8875_INT 22
#define RA8875_CS 25
#define RA8875_RESET 9

//  Touchscreen map (actual values to 1024 scale)
//                    ACTUAL  ACTUAL  IDEAL   IDEAL
//                         MIN   MAX     MIN   MAX
#define X_CORRECTION_MAP    60,   980,  0,    1024
#define Y_CORRECTION_MAP    100,  940,  0,    1024

//*************
// Starting positions for the dieCursor used in eachDieRoll, void setup, etc...
#define CURSOR_START_X 110
#define CURSOR_START_Y 150

// Ending positions for the dieCursor
// Indicates how far we can write horizontally before
//  inserting new line (in pixels)
#define CURSOR_END_X 675
#define CURSOR_END_Y 380

// Incremental sizes for the characters
//  Cursor will move this many pixels for each character printed
#define CHARACTER_X_SIZE 10
#define CHARACTER_Y_SIZE 30

// pixel buffer forBMP Draw
#define BUFFPIXEL 20

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

/*
   adding stage identifiers to try and fix sub menu problem.
   mainStage = 0
   saveStage = 1
*/
int stageNumber = 0;



// define placeholder columns
int VAR_Grand_Total_Ones;        // Grand total Ones column
int VAR_Grand_Total_Tens;        // Grand Total Tens Column
int VAR_Grand_Total_Hund;        // Grand total Hundreds column
/*
  // character setup bit for run once.
  boolean charIsSetup = false;
*/

// Global button sizing variables
int TRyStart = 10; // y start position
int DxSize = 75; // w size in pixels
int DySize = 75; // y size in pixels
int DRad = 15; // corner radius

// Top & side Rows Button Map positioning definitions
int MODxStart = 15; // X starting position Left vetical row
int TopiconSpace = 25; // X spacing between icons
int P1xStart = 15; // X starting position of bottom row
int P5xStart = ((P1xStart + DxSize) + TopiconSpace); // ****************************************
int M1xStart = ((P5xStart + DxSize) + TopiconSpace); //*****************************************
int M5xStart = ((M1xStart + DxSize) + TopiconSpace); // Next buton position based on first button in row, and previously defined variables.
int CLRxStart = ((M5xStart + DxSize) + TopiconSpace); //*****************************************
int TTLxStart = ((CLRxStart + DxSize) + TopiconSpace); //*****************************************
int ATK1xStart = ((TTLxStart + DxSize) + TopiconSpace); //*****************************************
int ATK2xStart = ((ATK1xStart + DxSize) + TopiconSpace); //*****************************************
int ATK3xStart = ((ATK1xStart + DxSize) + TopiconSpace); //Begin right vertical row of buttons
int ATK4xStart = ((ATK1xStart + DxSize) + TopiconSpace); //*****************************************
int ADVxStart = ((ATK1xStart + DxSize) + TopiconSpace); //*****************************************
int DISxStart = ((ATK1xStart + DxSize) + TopiconSpace); //*****************************************



int txtStartx = 125;
int txtStarty = 301;


//Bottom Row Button Maps positioning definitions
int BTMiconSpace = 25; // X spacing between icons
int BRyStart = 395; // y start position
int D4xStart = 15; // X starting position of bottom row
int D6xStart = ((D4xStart + DxSize) + BTMiconSpace); // ****************************************
int D8xStart = ((D6xStart + DxSize) + BTMiconSpace); // ****************************************
int D10xStart = ((D8xStart + DxSize) + BTMiconSpace); // Next buton position based on first button in roa, and previously defined variables.
int D12xStart = ((D10xStart + DxSize) + BTMiconSpace); // ****************************************
int D20xStart = ((D12xStart + DxSize) + BTMiconSpace); // ****************************************
int D100xStart = ((D20xStart + DxSize) + BTMiconSpace); // ****************************************
int INITxStart = ((D100xStart + DxSize) + BTMiconSpace); // ****************************************


// text display block definition for button labels
int TXTxStart = 100;
int TXTyStart = 100;
int TXTxWidth = 600;
int TXTyWidth = 280;

// setup file for character
File myFile;
// Saving Throw Stats (to be defined by user on startup)
int initiative = 0;
int armorClass = 0;
int STR = 0;
int strSave = 0;
int athletics = 0;
int DEX = 0;
int dexSave = 0;
int acrobatics = 0;
int slightOfHand = 0;
int stealth = 0;
int CON = 0;
int conSave = 0;
int INT = 0;
int intSave = 0;
int arcana = 0;
int history = 0;
int investigation = 0;
int nature = 0;
int religion = 0;
int WIS = 0;
int wisSave = 0;
int animalHandling = 0;
int insight = 0;
int medicine = 0;
int perception = 0;
int survival = 0;
int CHA = 0;
int charSave = 0;
int deception = 0;
int intimidation = 0;
int performance = 0;
int persuasion = 0;
int hitPoints = 0;
int atk1 = 0;
int atk2 = 0;
int atk3 = 0;
int atk4 = 0;
// end player stats




/*
  char charName = "PLAYER";



  // spell hit modifier (to be defined by user on startup)
  int VAR_S_Mod = 0;
  // melee hit modifier (to be defined by user on startup)????????????? TBD ????????????????
  int VAR_M_Mod = 0;
*/

// Variable modifier to be added to roll if needed.
int VAR_Mod = 0;

// Die sub totals
int VAR_4_Sum;                   // Total of all d4 rolled
int VAR_6_Sum;                   // Total of all d6 rolled
int VAR_8_Sum;                   // Total of all d8 rolled
int VAR_10_Sum;                  // Total of all d10 rolled
int VAR_12_Sum;                  // Total of all d12 rolled
int VAR_20_Sum;                  // Total of all d20 rolled
int VAR_100_Sum;                 // Total of all d100 rolled
int VAR_attackOneSum;
int VAR_attackTwoSum;
int VAR_attackThreeSum;
// total of all dice and modifiers for applicable action
int VAR_Grand_Total;



// temporary place holder for sum of a singular die type (all D4 dice rolled, or all D5 dice rolled.
int DieRollSum = 0;
// number of dice in the dice bag
int buttonPushCounter = 0;


float xScale = 1024.0F / tft.width();
float yScale = 1024.0F / tft.height();

// begin Cody Tappan's  code
// Type used to track the position of the cursor
typedef struct {
  int xPos; // Horizontal position
  int yPos; // Vertical positon
} cursor;

// Instance of cursor,
// Used to track the cursor for printing die results
cursor dieCursor;
// end Cody Tappan's  code

// labels for stat blocks on mainstage
void statsLabel(int xstart, int ystart, String label)
{
  tft.textMode();
  tft.textEnlarge(1); // temporary increase to font size
  tft.textSetCursor(xstart, ystart); // defines starting text position for this code block
  tft.textColor(RA8875_CYAN, RA8875_BLACK);
  tft.print(label);
  // tft.textSetCursor(xstart + 40, ystart + 40); // definnes the start position of the save number
  // tft.print(StatN);
  tft.graphicsMode();
}

// prints stat values when called eiter mainstage or savestage
void statsPrint (int xstart, int ystart, int StatN)
{
  tft.textMode();
  tft.textEnlarge(1); // temporary increase to font size
  // tft.textSetCursor(xstart, ystart); // defines starting text position for this code block
  tft.textColor(RA8875_CYAN, RA8875_BLACK);
  // tft.print(label);
  tft.textSetCursor(xstart, ystart); // definnes the start position of the save number
  tft.print(StatN);
  tft.graphicsMode();
}

// clears the print area of all previously drawn text
// redraws text area bounding box
// redraws character save boxes, and labels
// rests cursor position to top left of text box
void resetCenterMainStage()
{
  // starting variables for drawing text on stage
  int txtStartx = 125;
  int txtStarty = 301;

  // Draw text boundry box on stage (Gobal variables defined at top of code structure)
  tft.fillRect (TXTxStart, TXTyStart, TXTxWidth, TXTyWidth, RA8875_BLACK);
  // Draw black background for text area
  tft.drawRect (TXTxStart, TXTyStart, TXTxWidth, TXTyWidth, RA8875_CYAN);

  // Draw horizontal dividers (local variables defined above)
  //           xStart/ystart/xEnd/yEnd
  tft.drawLine (100, 300, 700, 300, RA8875_CYAN); // top line of save label block
  tft.drawLine (100, 335, 700, 335, RA8875_CYAN); // middle line of save label block

  // Draw vertical Lines to define character save stats
  //           xStart/ystart/xEnd/yEnd
  tft.drawLine (200, 300, 200, 380, RA8875_CYAN);
  tft.drawLine (300, 300, 300, 380, RA8875_CYAN);
  tft.drawLine (400, 300, 400, 380, RA8875_CYAN);
  tft.drawLine (500, 300, 500, 380, RA8875_CYAN);
  tft.drawLine (600, 300, 600, 380, RA8875_CYAN);


  // Draw saving throw text identifiers
  statsLabel(txtStartx, txtStarty, "STR");
  statsLabel(txtStartx + 100, txtStarty, "DEX");
  statsLabel(txtStartx + 200, txtStarty, "CON");
  statsLabel(txtStartx + 300, txtStarty, "INT");
  statsLabel(txtStartx + 400, txtStarty, "WIS");
  statsLabel(txtStartx + 500, txtStarty, "CHA");

  // Draw saving throw values
  //          x / y / variable
  statsPrint (140, 340, STR);
  statsPrint (240, 340, DEX);
  statsPrint (340, 340, CON);
  statsPrint (440, 340, INT);
  statsPrint (540, 340, WIS);
  statsPrint (640, 340, CHA);

  // Cody Tappan's  code
  dieCursor.xPos = CURSOR_START_X;
  dieCursor.yPos = CURSOR_START_Y;
  // Cody Tappan's  code
}


// clears the print area of all previously drawn text
// redraws text area bounding box
// redraws character save boxes, and labels
// rests cursor position to top left of text box
// draws lines only no text or values
void drawSaveStage()
{
  // starting variables for drawing text on stage
  int txtStartx = 125;
  int txtStarty = 301;
  int TXTxStart = 100;
  int TXTyStart = 100;
  int TXTxWidth = 600;
  int TXTyWidth = 280;
  // tft.fillRect(0,0,800,480,RA8875_CYAN);
  // tft.fillRoundRect(90,90,620,810, 15,RA8875_BLACK);
  // Home button for all saving throw pages
  tft.fillRoundRect(250, 400, 300, 75, DRad, RA8875_WHITE);
  tft.fillRoundRect(250 + 3 , 400 + 3, 300 - 6 , 75 - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(250 + 12 , 400 + 12, 300 - 24 , 75 - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("Home", 338 , 400);

  // Draw text boundry box on stage
  tft.drawRect (TXTxStart, TXTyStart, TXTxWidth, TXTyWidth, RA8875_CYAN);

  // Draw horizontal dividers (local variables defined above)
  //           xStart/ystart/xEnd/yEnd
  tft.drawLine (100, 300, 700, 300, RA8875_CYAN); // top line of save label block
  tft.drawLine (100, 335, 700, 335, RA8875_CYAN); // middle line of save label block

  // Draw vertical Lines to define character save stats
  //           xStart/ystart/xEnd/yEnd
  tft.drawLine (200, 300, 200, 380, RA8875_CYAN);
  tft.drawLine (300, 300, 300, 380, RA8875_CYAN);
  tft.drawLine (400, 300, 400, 380, RA8875_CYAN);
  tft.drawLine (500, 300, 500, 380, RA8875_CYAN);
  tft.drawLine (600, 300, 600, 380, RA8875_CYAN);
}

void drawHealthStage()
{
  // starting variables for drawing text on stage
  int txtStartx = 125;
  int txtStarty = 301;
  int TXTxStart = 100;
  int TXTyStart = 100;
  int TXTxWidth = 600;
  int TXTyWidth = 280;
  // tft.fillRect(0,0,800,480,RA8875_CYAN);
  // tft.fillRoundRect(90,90,620,810, 15,RA8875_BLACK);
  // Home button for all saving throw pages
  tft.fillRoundRect(250, 400, 300, 75, DRad, RA8875_WHITE);
  tft.fillRoundRect(250 + 3 , 400 + 3, 300 - 6 , 75 - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(250 + 12 , 400 + 12, 300 - 24 , 75 - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("Home", 338 , 400);

  // Draw text boundry box on stage
  tft.drawRect (TXTxStart, TXTyStart, TXTxWidth, TXTyWidth, RA8875_CYAN);

  // Draw horizontal dividers (local variables defined above)
  //           xStart/ystart/xEnd/yEnd
  tft.drawLine (100, 300, 700, 300, RA8875_CYAN); // top line of save label block
  tft.drawLine (100, 335, 700, 335, RA8875_CYAN); // middle line of save label block

  // Draw vertical Lines to define character save stats
  //           xStart/ystart/xEnd/yEnd
  tft.drawLine (200, 300, 200, 380, RA8875_CYAN);
  tft.drawLine (300, 300, 300, 380, RA8875_CYAN);
  tft.drawLine (400, 300, 400, 380, RA8875_CYAN);
  tft.drawLine (500, 300, 500, 380, RA8875_CYAN);
  tft.drawLine (600, 300, 600, 380, RA8875_CYAN);
}


// Begin column breakdown. Following code creates a variable for each column place holder in the VAR_Grand_Total
// function Light_Em_Up defines placeholders, AND which cathode to ignite
void Light_Em_Up(int VAR_Grand_Total)
{
  int VAR_Grand_Total_Ones = VAR_Grand_Total % 10;
  Serial.print("Ones - ");
  Serial.println(VAR_Grand_Total_Ones);

  int VAR_Grand_Total_TensP = VAR_Grand_Total / 10;
  int VAR_Grand_Total_Tens = VAR_Grand_Total_TensP % 10;
  Serial.print("Tens - ");
  Serial.println(VAR_Grand_Total_Tens);

  int VAR_Grand_Total_Hund = VAR_Grand_Total / 100;
  Serial.print("Hundreds - ");
  Serial.println(VAR_Grand_Total_Hund );

  // End Column Breakdown
  // Begin lighting sequence. Following code defines which cathode to power for respective number in the respctive placeholder column.
  if (VAR_Grand_Total_Ones == 0)
  {
    // LIGHT 0
    digitalWrite (PIN_T1A, LOW);
    digitalWrite (PIN_T1B, LOW);
    digitalWrite (PIN_T1C, LOW);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 1)
  {
    // LIGHT 1
    digitalWrite (PIN_T1A, HIGH);
    digitalWrite (PIN_T1B, LOW);
    digitalWrite (PIN_T1C, LOW);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 2)
  {
    // LIGHT 2
    digitalWrite (PIN_T1A, LOW);
    digitalWrite (PIN_T1B, HIGH);
    digitalWrite (PIN_T1C, LOW);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 3)
  {
    // LIGHT 3
    digitalWrite (PIN_T1A, HIGH);
    digitalWrite (PIN_T1B, HIGH);
    digitalWrite (PIN_T1C, LOW);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 4)
  {
    // LIGHT 4
    digitalWrite (PIN_T1A, LOW);
    digitalWrite (PIN_T1B, LOW);
    digitalWrite (PIN_T1C, HIGH);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 5)
  {
    // LIGHT 5
    digitalWrite (PIN_T1A, HIGH);
    digitalWrite (PIN_T1B, LOW);
    digitalWrite (PIN_T1C, HIGH);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 6)
  {
    // LIGHT 6
    digitalWrite (PIN_T1A, LOW);
    digitalWrite (PIN_T1B, HIGH);
    digitalWrite (PIN_T1C, HIGH);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 7)
  {
    // LIGHT 7
    digitalWrite (PIN_T1A, HIGH);
    digitalWrite (PIN_T1B, HIGH);
    digitalWrite (PIN_T1C, HIGH);
    digitalWrite (PIN_T1D, LOW);
  }
  else if (VAR_Grand_Total_Ones == 8)
  {
    // LIGHT 8
    digitalWrite (PIN_T1A, LOW);
    digitalWrite (PIN_T1B, LOW);
    digitalWrite (PIN_T1C, LOW);
    digitalWrite (PIN_T1D, HIGH);
  }
  else if (VAR_Grand_Total_Ones == 9)
  {
    // LIGHT 9
    digitalWrite (PIN_T1A, HIGH);
    digitalWrite (PIN_T1B, LOW);
    digitalWrite (PIN_T1C, LOW);
    digitalWrite (PIN_T1D, HIGH);
  }
  if (VAR_Grand_Total_Tens == 0)
  {
    // LIGHT 0
    digitalWrite (PIN_T2A, LOW);
    digitalWrite (PIN_T2B, LOW);
    digitalWrite (PIN_T2C, LOW);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 1)
  {
    // LIGHT 1
    digitalWrite (PIN_T2A, HIGH);
    digitalWrite (PIN_T2B, LOW);
    digitalWrite (PIN_T2C, LOW);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 2)
  {
    // LIGHT 2
    digitalWrite (PIN_T2A, LOW);
    digitalWrite (PIN_T2B, HIGH);
    digitalWrite (PIN_T2C, LOW);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 3)
  {
    // LIGHT 3
    digitalWrite (PIN_T2A, HIGH);
    digitalWrite (PIN_T2B, HIGH);
    digitalWrite (PIN_T2C, LOW);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 4)
  {
    // LIGHT 4
    digitalWrite (PIN_T2A, LOW);
    digitalWrite (PIN_T2B, LOW);
    digitalWrite (PIN_T2C, HIGH);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 5)
  {
    // LIGHT 5
    digitalWrite (PIN_T2A, HIGH);
    digitalWrite (PIN_T2B, LOW);
    digitalWrite (PIN_T2C, HIGH);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 6)
  {
    // LIGHT 6
    digitalWrite (PIN_T2A, LOW);
    digitalWrite (PIN_T2B, HIGH);
    digitalWrite (PIN_T2C, HIGH);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 7)
  {
    // LIGHT 7
    digitalWrite (PIN_T2A, HIGH);
    digitalWrite (PIN_T2B, HIGH);
    digitalWrite (PIN_T2C, HIGH);
    digitalWrite (PIN_T2D, LOW);
  }
  else if (VAR_Grand_Total_Tens == 8)
  {
    // LIGHT 8
    digitalWrite (PIN_T2A, LOW);
    digitalWrite (PIN_T2B, LOW);
    digitalWrite (PIN_T2C, LOW);
    digitalWrite (PIN_T2D, HIGH);
  }
  else if (VAR_Grand_Total_Tens == 9)
  {
    // LIGHT 9
    digitalWrite (PIN_T2A, HIGH);
    digitalWrite (PIN_T2B, LOW);
    digitalWrite (PIN_T2C, LOW);
    digitalWrite (PIN_T2D, HIGH);
  }
  if (VAR_Grand_Total_Hund == 0)
  {
    // LIGHT 0
    digitalWrite (PIN_T3A, LOW);
    digitalWrite (PIN_T3B, LOW);
    digitalWrite (PIN_T3C, LOW);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 1)
  {
    // LIGHT 1
    digitalWrite (PIN_T3A, HIGH);
    digitalWrite (PIN_T3B, LOW);
    digitalWrite (PIN_T3C, LOW);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 2)
  {
    // LIGHT 2
    digitalWrite (PIN_T3A, LOW);
    digitalWrite (PIN_T3B, HIGH);
    digitalWrite (PIN_T3C, LOW);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 3)
  {
    // LIGHT 3
    digitalWrite (PIN_T3A, HIGH);
    digitalWrite (PIN_T3B, HIGH);
    digitalWrite (PIN_T3C, LOW);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 4)
  {
    // LIGHT 4
    digitalWrite (PIN_T3A, LOW);
    digitalWrite (PIN_T3B, LOW);
    digitalWrite (PIN_T3C, HIGH);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 5)
  {
    // LIGHT 5
    digitalWrite (PIN_T3A, HIGH);
    digitalWrite (PIN_T3B, LOW);
    digitalWrite (PIN_T3C, HIGH);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 6)
  {
    // LIGHT 6
    digitalWrite (PIN_T3A, LOW);
    digitalWrite (PIN_T3B, HIGH);
    digitalWrite (PIN_T3C, HIGH);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 7)
  {
    // LIGHT 7
    digitalWrite (PIN_T3A, HIGH);
    digitalWrite (PIN_T3B, HIGH);
    digitalWrite (PIN_T3C, HIGH);
    digitalWrite (PIN_T3D, LOW);
  }
  else if (VAR_Grand_Total_Hund == 8)
  {
    // LIGHT 8
    digitalWrite (PIN_T3A, LOW);
    digitalWrite (PIN_T3B, LOW);
    digitalWrite (PIN_T3C, LOW);
    digitalWrite (PIN_T3D, HIGH);
  }
  else if (VAR_Grand_Total_Hund == 9)
  {
    // LIGHT 9
    digitalWrite (PIN_T3A, HIGH);
    digitalWrite (PIN_T3B, LOW);
    digitalWrite (PIN_T3C, LOW);
    digitalWrite (PIN_T3D, HIGH);
  }
}




// print the number of dice in the bag on the centermainstage
void dieBagPrint()
{
  Serial.println (buttonPushCounter);
  resetCenterMainStage(); // clears any previously draw data
  tft.textMode(); // sets to text mode
  tft.textEnlarge(2); // temporary increase to font size
  tft.textSetCursor(110, 190 ); // defines starting text position for this code block
  tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
  tft.print("# of dice in the bag: ");// prints data to the screen
  tft.print(buttonPushCounter); // prints the number of dice in the bag
  tft.graphicsMode(); // returns to graphics mode

}

// print the value of the modifer to add to a roll on the centermainstage
void dieModPrint()
{
  Serial.println (buttonPushCounter);
  resetCenterMainStage(); // clears any previously draw data
  tft.textMode(); // sets to text mode
  tft.textEnlarge(2); // temporary increase to font size
  tft.textSetCursor(110, 190 ); // defines starting text position for this code block
  tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
  tft.print("Modifier to add to the roll: ");// prints data to the screen
  tft.print(buttonPushCounter); // prints the number to be added to the roll.
  tft.graphicsMode(); // returns to graphics mode
}



// prints individual die rolls to the the serial monitor
// for debugging can be disabled later if desired.
void dieSubTotalPrint(int dieType, int RollN, int i)
{
  Serial.print("D");
  Serial.print(dieType);
  Serial.print(" #");
  Serial.print(i);
  Serial.print(" - ");
  Serial.print(RollN);
  Serial.print(". Touch: ");
  Serial.print(tx);
  Serial.print(", ");
  Serial.println(ty);
}

// button text label function to be called later when printing button lables.
void buttonlabel(String label, int xStart, int yStart)
{
  tft.textMode(); // sets to text mode
  tft.textEnlarge(1); // temporary increase to font size
  tft.textSetCursor(xStart + 15, yStart + 15); // defines starting text position for this code block
  tft.textTransparent(RA8875_BLACK); // Black text transparent bacjground
  tft.print(label); // print variable "LABEL"
  tft.graphicsMode(); // return to graphics mode
}

// prints each die roll to the screen and to the serial monitor.
// Int i = number of dice to roll
// int dieType defines the die type 4, 6, 8, 10, 12, sided etc
// boolean moreDice is true of false difines the last die
// Cody Tappan's portion of this code tracks cursor positition,
// and advnaces to correct x/y location when needed.
void eachdieroll(int i, int dieType, int DieRoll, boolean moreDice)
{
  // Convert die roll to a string
  String dieString = String(DieRoll);

  // Prepare tft for printing
  tft.textMode();
  tft.textEnlarge(1);
  tft.setTextColor(RA8875_WHITE, RA8875_BLACK);
  tft.textSetCursor(110, 120); // 80 + (i * 30)); // defines starting text position for this code block
  tft.print(buttonPushCounter);
  tft.print(" - D"); // prints to the the TFT Screen
  tft.print(dieType); // prints to the the TFT Screen
  tft.print(" Rolls:"); // prints to the the TFT Screen
  // tft.print(i); // prints to the the TFT Screen
  // tft.print(" = "); // prints to the the TFT Screen
  // prints to the the TFT Screen

  // Check if print would overflow text box boundry
  if (dieCursor.xPos + ((dieString.length() + 3) * CHARACTER_X_SIZE) >= CURSOR_END_X)
  {
    // Increment y pos of cursor to next line
    // TODO :: Check for vertical (Y) overflow here
    dieCursor.yPos += CHARACTER_Y_SIZE;
    // Reset cursor to starting x position
    dieCursor.xPos = CURSOR_START_X;
  }

  // If so, next line
  tft.textSetCursor(dieCursor.xPos, dieCursor.yPos);

  tft.print(dieString);

  // If there are more dice to roll, add the separator
  if ( moreDice )
  {
    tft.print("/");
  }

  // Iterate the x position of the cursor forward,
  // adding enough space (+3 characters) to cover separator
  dieCursor.xPos += (dieString.length() + 3) * CHARACTER_X_SIZE;

  if ( !moreDice )
  {
    // Increment y pos of cursor to next line
    // TODO :: Check for vertical overflow here
    dieCursor.yPos += CHARACTER_Y_SIZE;
    // Reset cursor to starting x position
    dieCursor.xPos = CURSOR_START_X;

    // If so, next line
    tft.textSetCursor(dieCursor.xPos, dieCursor.yPos);
    tft.textEnlarge(2);
    Serial.print("Sub Total: ");
    Serial.println(DieRollSum);
    tft.print("Sub Total: ");
    tft.print(DieRollSum);
  }

  tft.graphicsMode();
}

void slots() // Slot machine random loop, generates random scrolling numbers for effect
{
  int i;
  for (i = 0; i < 50; i++)
  {
    int VAR_Grand_Total = random(0, 1000);
    Light_Em_Up(VAR_Grand_Total);
  }
}


// rolls selected die based on touchmaps as many times as buttonpushcounter states
void rolldice(int dieType)
{
  for (int i = 1; i <= buttonPushCounter; i++) {
    int DieRoll = random (1, dieType + 1);
    DieRollSum += DieRoll; // sums the total of the dice rolled
    dieSubTotalPrint(dieType, DieRoll, i); // prints the subtotal to the TFT
    eachdieroll(i, dieType, DieRoll , i != buttonPushCounter); // prints each die roll individually to the TFT
  }
  buttonPushCounter = 0; // rests teh buttonpushcounter to zero (no more dice in the bag)

  // determines the type of die rolled
  // stores the sum for that die type in a unique
  // variable for that die type, untill cleared by either
  // the CLEAR button, or another die roll of the same type.
  // Includes D4/D6/D8/D10/D12 dice only.
  if (dieType == 4) {
    VAR_4_Sum = DieRollSum ;
    DieRollSum = 0;

  }
  else if (dieType == 6) {
    VAR_6_Sum = DieRollSum ;
    DieRollSum = 0;
  }
  else if (dieType == 8) {
    VAR_8_Sum = DieRollSum ;
    DieRollSum = 0;
  }
  else if (dieType == 10) {
    VAR_10_Sum = DieRollSum ;
    DieRollSum = 0;
  }
  else if (dieType == 12) {
    VAR_12_Sum = DieRollSum ;
    DieRollSum = 0;
  }
}


// sets all subtotal variables to zero and clears the print area.
void clearall()
{
  VAR_4_Sum = 0;                   // Clear Total of all d4 rolled
  VAR_6_Sum = 0;                   // Clear Total of all d6 rolled
  VAR_8_Sum = 0;                   // Clear Total of all d8 rolled
  VAR_10_Sum = 0;                  // Clear Total of all d10 rolled
  VAR_12_Sum = 0;                  // Clear Total of all d12 rolled
  VAR_20_Sum = 0;                  // Clear Total of all d20 rolled
  VAR_100_Sum = 0;                 // Clear Total of all d100 rolled
  VAR_Mod = 0;                     // Clear Var Mod variable
  VAR_attackOneSum = 0;
  VAR_attackTwoSum = 0;
  VAR_attackThreeSum = 0;
  VAR_Grand_Total = 0;
  DieRollSum = 0;

  buttonPushCounter = 0;


  // LIGHT NIXIE ones to 0
  digitalWrite (PIN_T1A, LOW);
  digitalWrite (PIN_T1B, LOW);
  digitalWrite (PIN_T1C, LOW);
  digitalWrite (PIN_T1D, LOW);
  // LIGHT NIXIE tens to 0
  digitalWrite (PIN_T2A, LOW);
  digitalWrite (PIN_T2B, LOW);
  digitalWrite (PIN_T2C, LOW);
  digitalWrite (PIN_T2D, LOW);
  // LIGHT NIXIE hundreds to 0
  digitalWrite (PIN_T3A, LOW);
  digitalWrite (PIN_T3B, LOW);
  digitalWrite (PIN_T3C, LOW);
  digitalWrite (PIN_T3D, LOW);
}

void readSDSettings() {
  char character;
  String settingName;
  String settingValue;
  // variable must be bracketed by [], example [acrobatics=10] , note there are no spaces!
  myFile = SD.open("settings.txt");
  if (myFile) {
    while (myFile.available()) {
      character = myFile.read();
      while ((myFile.available()) && (character != '[')) {
        character = myFile.read();
      }
      character = myFile.read();
      while ((myFile.available()) && (character != '=')) {
        settingName = settingName + character;
        character = myFile.read();
      }
      character = myFile.read();
      while ((myFile.available()) && (character != ']')) {
        settingValue = settingValue + character;
        character = myFile.read();
      }
      if (character == ']') {

        /*
                //Debuuging Printing
                Serial.print("Name:");
                Serial.println(settingName);
                Serial.print("Value :");
                Serial.println(settingValue);
        */

        // Apply the value to the parameter
        applySetting(settingName, settingValue);
        // Reset Strings
        settingName = "";
        settingValue = "";
      }
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening settings.txt");
  }
}

/* Apply the value to the parameter by searching for the parameter name
  Using String.toInt(); for Integers
  toFloat(string); for Float
  toBoolean(string); for Boolean
  toLong(string); for Long
*/
void applySetting(String settingName, String settingValue) {
  if (settingName == "STR") {
    STR = settingValue.toInt();
  }
  if (settingName == "strSave") {
    strSave = settingValue.toInt();
  }
  if (settingName == "DEX") {
    DEX = settingValue.toInt();
  }
  if (settingName == "dexSave") {
    dexSave = settingValue.toInt();
  }
  if (settingName == "acrobatics") {
    acrobatics = settingValue.toInt();
  }
  if (settingName == "slightOfHand") {
    slightOfHand = settingValue.toInt();
  }
  if (settingName == "stealth") {
    stealth = settingValue.toInt();
  }
  if (settingName == "CON") {
    CON = settingValue.toInt();
  }
  if (settingName == "conSave") {
    conSave = settingValue.toInt();
  }
  if (settingName == "INT") {
    INT = settingValue.toInt();
  }
  if (settingName == "intSave") {
    intSave = settingValue.toInt();
  }
  if (settingName == "arcana") {
    arcana = settingValue.toInt();
  }
  if (settingName == "history") {
    history = settingValue.toInt();
  }
  if (settingName == "investigation") {
    investigation = settingValue.toInt();
  }
  if (settingName == "nature") {
    nature = settingValue.toInt();
  }
  if (settingName == "religion") {
    religion = settingValue.toInt();
  }
  if (settingName == "WIS") {
    WIS = settingValue.toInt();
  }
  if (settingName == "wisSave") {
    wisSave = settingValue.toInt();
  }
  if (settingName == "animalHandling") {
    animalHandling = settingValue.toInt();
  }
  if (settingName == "insight") {
    insight = settingValue.toInt();
  }
  if (settingName == "medicine") {
    medicine = settingValue.toInt();
  }
  if (settingName == "perception") {
    perception = settingValue.toInt();
  }
  if (settingName == "survival") {
    survival = settingValue.toInt();
  }

  if (settingName == "CHA") {
    CHA = settingValue.toInt();
  }
  if (settingName == "charSave") {
    charSave = settingValue.toInt();
  }
  if (settingName == "deception") {
    deception = settingValue.toInt();
  }
  if (settingName == "intimidation") {
    intimidation = settingValue.toInt();
  }
  if (settingName == "performance") {
    performance = settingValue.toInt();
  }
  if (settingName == "persuasion") {
    persuasion = settingValue.toInt();
  }
  if (settingName == "initiative") {
    initiative = settingValue.toInt();
  }

  if (settingName == "atk1") {
    atk1 = settingValue.toInt();
  }

  if (settingName == "atk2") {
    atk2 = settingValue.toInt();
  }

  if (settingName == "atk3") {
    atk3 = settingValue.toInt();
  }

  if (settingName == "atk4") {
    atk4 = settingValue.toInt();
  }

  if (settingName == "hitPoints") {
    hitPoints = settingValue.toInt();
  }

  if (settingName == "armorClass") {
    armorClass = settingValue.toInt();
  }
}

// run once
void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /*
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }
    Serial.print("Initializing SD card...");
  */
  pinMode(10, OUTPUT);
  SD.begin(53);
  readSDSettings();

  // initialize the digital pins for the tubes as outputs
  pinMode(PIN_T1A, OUTPUT);
  pinMode(PIN_T1B, OUTPUT);
  pinMode(PIN_T1C, OUTPUT);
  pinMode(PIN_T1D, OUTPUT);

  pinMode(PIN_T2A, OUTPUT);
  pinMode(PIN_T2B, OUTPUT);
  pinMode(PIN_T2C, OUTPUT);
  pinMode(PIN_T2D, OUTPUT);

  pinMode(PIN_T3A, OUTPUT);
  pinMode(PIN_T3B, OUTPUT);
  pinMode(PIN_T3C, OUTPUT);
  pinMode(PIN_T3D, OUTPUT);

  randomSeed(analogRead(A0)); // sets random seed on setup.


  // ******************************************//
  // ******************************************//
  // *****Touch screen and display setup*******//
  // ******************************************//
  // ******************************************//

  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();


  Serial.begin(9600); // init serial comms
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("RA8875 start");
  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }
  Serial.println("Found RA8875");
  tft.displayOn(true); // turns on display
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255); // backlight brightness 0-255
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  tft.touchEnable(true); // enable touch screen
  Serial.print("Status: "); Serial.println(tft.readStatus(), HEX);
  Serial.println("Waiting for touch events ...");
  //   drawSetupStage();
  //   drawMainStage();
  delay (1000);
}

// ******************************************//
// ******************************************//
// ****END Touch screen and display setup****//
// ******************************************//
// ******************************************//


// defines the touch maps for the bottom row of buttons
void dietouchread(int dieType, int DxStart, int DyStart, int DxSize, int DySize, float xScale, float yScale)
{
  if ((tx > (DxStart * xScale) && (tx <= (DxStart + DxSize) * xScale))) {
    if ((ty > (DyStart * yScale) && (ty <= (DyStart + DySize) * yScale))) {
      // Preps print area for new text
      resetCenterMainStage();
      // rolls dice, and prints to TFT
      rolldice(dieType);
    }
  }
}





void savetouchread(String saveName, int xStart, int yStart, int xSize, int ySize, float xScale, float yScale)
{
  if ((tx > (xStart * xScale) && (tx <= (xStart + xSize) * xScale))) {
    if ((ty > (yStart * yScale) && (ty <= (yStart + ySize) * yScale))) {

      if (saveName == "STR") {
        Serial.println (saveName);
        stageNumber = 1;
        Serial.println (stageNumber);
      }
      if (saveName == "DEX") {
        Serial.println (saveName);
        stageNumber = 2;
        Serial.println (stageNumber);
      }
      if (saveName == "CON") {
        Serial.println (saveName);
        stageNumber = 3;
        Serial.println (stageNumber);
      }
      if (saveName == "INT") {
        Serial.println (saveName);
        stageNumber = 4;
        Serial.println (stageNumber);
      }
      if (saveName == "WIS") {
        Serial.println (saveName);
        stageNumber = 5;
        Serial.println (stageNumber);
      }
      if (saveName == "CHA") {
        Serial.println (saveName);
        stageNumber = 6;
        Serial.println (stageNumber);
      }
      if (saveName == "HEALTH") {
        Serial.println (saveName);
        stageNumber = 7;
        Serial.println (stageNumber);
      }
    }
  }
}

void readSaveSubMenu(String saveName, int saveValue, int xStart, int yStart, int xSize, int ySize, float xScale, float yScale)
{
  if ((tx > (xStart * xScale) && (tx <= (xStart + xSize) * xScale))) {
    if ((ty > (yStart * yScale) && (ty <= (yStart + ySize) * yScale))) {
      savingThrowRoll(saveName, saveValue);
    }
  }
}

void readHomeButton(int xStart, int yStart, int xSize, int ySize, float xScale, float yScale)
{
  if ((tx > (xStart * xScale) && (tx <= (xStart + xSize) * xScale))) {
    if ((ty > (yStart * yScale) && (ty <= (yStart + ySize) * yScale))) {
      tft.fillRect(0, 0, 800, 480, RA8875_BLACK);
      stageNumber = 0;
    }
  }
}

void readHealthSubMenu(String buttonType, int xStart, int yStart, int xSize, int ySize, float xScale, float yScale)
{
  if ((tx > (xStart * xScale) && (tx <= (xStart + xSize) * xScale))) {
    if ((ty > (yStart * yScale) && (ty <= (yStart + ySize) * yScale))) {
      if (buttonType == "HL+1") {
        hitPoints++;
        tft.textMode(); // sets to text mode
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
        tft.print("HP Now equals ");
        tft.print(hitPoints);
        tft.graphicsMode();
      }
      if (buttonType == "HL+5") {
        hitPoints = hitPoints + 5;
        tft.textMode(); // sets to text mode
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
        tft.print("HP Now equals ");
        tft.print(hitPoints);
        tft.graphicsMode();
      }
      if (buttonType == "HL-1") {
        hitPoints--;
        tft.textMode(); // sets to text mode
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
        tft.print("HP Now equals ");
        tft.print(hitPoints);
        tft.graphicsMode();
      }
      if (buttonType == "HL-5") {
        hitPoints = hitPoints - 5;
        tft.textMode(); // sets to text mode
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
        tft.print("HP Now equals ");
        tft.print(hitPoints);
        tft.graphicsMode();
      }
      if (buttonType == "AC+1") {
        armorClass++;
        tft.textMode(); // sets to text mode
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
        tft.print("AC Now equals ");
        tft.print(armorClass);
        tft.graphicsMode();
      }
      if (buttonType == "AC-1") {
        armorClass--;
        tft.textMode(); // sets to text mode
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
        tft.print("AC Now equals ");
        tft.print(armorClass);
        tft.graphicsMode();
      }
    }
  }
}
/*
  void readSaveSubMenu(String saveName, int saveValue, int xStart, int yStart, int xSize, int ySize, float xScale, float yScale)
  {
  if ((tx > (xStart * xScale) && (tx <= (xStart + xSize) * xScale))) {
    if ((ty > (yStart * yScale) && (ty <= (yStart + ySize) * yScale))) {
      savingThrowRoll(saveName, saveValue);
    }
  }
  }
*/


// prints saving thhrow rolls
void savingThrowRoll(String saveName, int saveValue)
{
  // Preps print area for new text
  tft.fillRect(101, 101, 598, 194, RA8875_BLACK);
  drawSaveStage(); // clears any previously draw data
  tft.textMode(); // sets to text mode
  tft.textEnlarge(1); // temporary increase to font size
  tft.textSetCursor(170, 180 ); // defines starting text position for this code block
  tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
  int saveRoll = random (1, 21); // randon D20 roll for save
  int saveRollSum = (saveRoll + saveValue); // sums the random roll and the saving throw variable
  tft.print("Rolling Dice");
  slots(); // random numbers on tubes emulates a slot machine

  Light_Em_Up(saveRollSum); // displays outcome on tubes
  tft.textSetCursor(170, 180 ); // defines starting text position for this code block
  tft.print("D20=" );
  tft.print(saveRoll);
  tft.print(" + ");
  tft.print(saveName);
  tft.print(" +");
  tft.print(saveValue);
  tft.textEnlarge(1); // temporary increase to font size
  tft.textSetCursor(170, 220 ); // defines starting text position for this code block
  tft.print("Saving throw: ");// prints data to the screen
  tft.print(saveRollSum); // prints the number of dice in the bag
  tft.graphicsMode(); // returns to graphics mode
}

// prints modifiers to stage
void printMod(String modName, int modValue)
{
  tft.textSetCursor(170, 180 ); // defines starting text position for this code block
  tft.textEnlarge(1); // temporary increase to font size
  tft.print(modName);
  tft.print(": ");
  tft.print(modValue);
  tft.print("will be added to your next roll");
  tft.textSetCursor(170, 220 ); // defines starting text position for this code block
  tft.graphicsMode(); // returns to graphics mode
}

/*
   draws main stage, and defines all hit boxes for main stage.
*/
void drawMainStage()
{
  // draw left side vertical buttons  MODxStart
  tft.fillRoundRect(MODxStart , TRyStart + TopiconSpace + DySize - 5, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(MODxStart + 3 , TRyStart + TopiconSpace + DySize - 2, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(MODxStart + 6 , TRyStart + TopiconSpace + DySize + 6, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("MOD", MODxStart , TRyStart + TopiconSpace + DySize);


  tft.fillRect(35 , TRyStart + TopiconSpace + TopiconSpace + DySize + DySize - 5, 35 , DySize, RA8875_WHITE);
  tft.fillRect(15 , TRyStart + 20 + TopiconSpace + TopiconSpace + DySize + DySize - 5, DxSize , 35, RA8875_WHITE);
  tft.fillRect(15 + 3 , TRyStart + 20 + TopiconSpace + TopiconSpace + DySize + DySize - 2, DxSize - 6 , 35 - 6, RA8875_RED);
  tft.fillRect(35 + 3 , TRyStart + TopiconSpace + TopiconSpace + DySize + DySize - 2, 35 - 6 , DySize - 6, RA8875_RED);
  tft.textMode(); // sets to text mode
  tft.textEnlarge(1); // temporary increase to font size
  tft.textSetCursor(35, 224); // defines starting text position for this code block
  tft.textTransparent(RA8875_WHITE); // WHITE text transparent bacjground
  tft.print(hitPoints); // print variable "LABEL"
  tft.graphicsMode(); // return to graphics mode

  // Draw shield for AC
  tft.fillCurve(50, 300, 38, 75, 4, RA8875_RED);
  tft.fillCurve(50, 300, 38, 75, 3, RA8875_RED);
  tft.fillCurve(50, 300, 34, 71, 4, RA8875_WHITE);
  tft.fillCurve(50, 300, 34, 71, 3, RA8875_WHITE);
  tft.fillEllipse(30, 300, 18, 8, RA8875_RED);
  tft.fillEllipse(70, 300, 18, 8, RA8875_RED);
  tft.fillEllipse(30, 295, 18, 8, RA8875_BLACK);
  tft.fillEllipse(70, 295, 18, 8, RA8875_BLACK);
  // draw AC Value in shield
  tft.textMode(); // sets to text mode
  tft.textEnlarge(1); // temporary increase to font size
  tft.textSetCursor(35, 315); // defines starting text position for this code block
  tft.textTransparent(RA8875_RED); // WHITE text transparent bacjground
  tft.print(armorClass); // print variable "LABEL"
  tft.graphicsMode(); // return to graphics mode


  // Draw top row buttons
  tft.fillRoundRect(P1xStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(P1xStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(P1xStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("+1", P1xStart , TRyStart);

  tft.fillRoundRect(P5xStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(P5xStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(P5xStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("+5", P5xStart , TRyStart);

  tft.fillRoundRect(M1xStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(M1xStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(M1xStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("-1", M1xStart , TRyStart);

  tft.fillRoundRect(M5xStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(M5xStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(M5xStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("-5", M5xStart , TRyStart);

  tft.fillRoundRect(CLRxStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(CLRxStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(CLRxStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("CL", CLRxStart + 5 , TRyStart);

  tft.fillRoundRect(TTLxStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(TTLxStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(TTLxStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("TL", TTLxStart + 5 , TRyStart);

  tft.fillRoundRect(ATK1xStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(ATK1xStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(ATK1xStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("AT1", ATK1xStart , TRyStart);

  tft.fillRoundRect(ATK2xStart , TRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(ATK2xStart + 3 , TRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(ATK2xStart + 6 , TRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("AT2", ATK2xStart , TRyStart);

  // draw right side vertical buttons
  tft.fillRoundRect(ATK3xStart , TRyStart + TopiconSpace + DySize - 5, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(ATK3xStart + 3 , TRyStart + TopiconSpace + DySize - 2, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(ATK3xStart + 6 , TRyStart + TopiconSpace + DySize + 6, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("AT3", ATK3xStart , TRyStart + TopiconSpace + DySize);

  tft.fillRoundRect(ADVxStart , TRyStart + TopiconSpace + TopiconSpace + DySize + DySize - 5, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(ADVxStart + 3 , TRyStart + TopiconSpace + TopiconSpace + DySize + DySize - 2, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(ADVxStart + 6 , TRyStart + TopiconSpace + TopiconSpace + DySize + DySize + 6, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("ADV", ADVxStart , TRyStart + TopiconSpace + TopiconSpace + DySize + DySize);

  tft.fillRoundRect(DISxStart , TRyStart + TopiconSpace + TopiconSpace + TopiconSpace + DySize + DySize + DySize - 5, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(DISxStart + 3 , TRyStart + TopiconSpace + TopiconSpace + TopiconSpace + DySize + DySize + DySize - 2, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(DISxStart + 6 , TRyStart + TopiconSpace + TopiconSpace + TopiconSpace + DySize + DySize + DySize + 6, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("DIS", DISxStart , TRyStart + TopiconSpace + TopiconSpace + TopiconSpace + DySize + DySize + DySize);


  // Draw bottom row buttons
  tft.fillRoundRect(D4xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D4xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D4xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("4", D4xStart + 15 , BRyStart);

  tft.fillRoundRect(D6xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D6xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D6xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("6", D6xStart + 15, BRyStart);

  tft.fillRoundRect(D8xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D8xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D8xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("8", D8xStart + 15, BRyStart);

  tft.fillRoundRect(D10xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D10xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D10xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("10", D10xStart + 3, BRyStart);

  tft.fillRoundRect(D12xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D12xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D12xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("12", D12xStart + 3, BRyStart);

  tft.fillRoundRect(D20xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D20xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D20xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("20", D20xStart + 8, BRyStart);


  tft.fillRoundRect(D100xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D100xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D100xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("100", D100xStart , BRyStart);

  tft.fillRoundRect(INITxStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(INITxStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(INITxStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("INIT", INITxStart - 10 , BRyStart);


  resetCenterMainStage();
}
void readMainStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {

    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }

    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);

    Serial.print("x: ");
    Serial.print(String(tx));

    Serial.print("\ty: ");
    Serial.print(String(ty));

    Serial.print("\n");
    Serial.print("+1 x start: ");
    Serial.print(String(P1xStart * xScale));
    Serial.print("\n");
    Serial.print("+1 x end: ");
    Serial.print(String((P1xStart + DxSize) * xScale));
    Serial.print("\n");
    Serial.print("+1 y start: ");
    Serial.print(String(TRyStart * yScale));
    Serial.print("\n");
    Serial.print("+1 y end: ");
    Serial.print(String((TRyStart + DySize) * yScale));
    Serial.print("\n");


    // look for touched dice on the bottom row
    dietouchread(4, D4xStart, BRyStart, DxSize, DySize, xScale, yScale);
    dietouchread(6, D6xStart, BRyStart, DxSize, DySize, xScale, yScale);
    dietouchread(8, D8xStart, BRyStart, DxSize, DySize, xScale, yScale);
    dietouchread(12, D12xStart, BRyStart, DxSize, DySize, xScale, yScale);
    // dietouchread(20, D20xStart, BRyStart, DxSize, DySize, xScale, yScale);
    // dietouchread(100, D100xStart, BRyStart, DxSize, DySize, xScale, yScale);
    // dietouchread(initiative, INITxStart, BRyStart, DxSize, DySize, xScale, yScale);

    // look for saving throw touches will roll saves and add modifer from SD card
    savetouchread("STR", 101, 300, 98, 80, xScale, yScale);
    savetouchread("DEX", 201, 300, 98, 80, xScale, yScale);
    savetouchread("CON", 301, 300, 98, 80, xScale, yScale);
    savetouchread("INT", 401, 300, 98, 80, xScale, yScale);
    savetouchread("WIS", 501, 300, 98, 80, xScale, yScale);
    savetouchread("CHA", 601, 300, 98, 80, xScale, yScale);
    savetouchread("HEALTH", 15, 205, DxSize , 175 , xScale, yScale);

    // Begin Total Button instructions block
    if ((tx > (TTLxStart * xScale) && (tx <= (TTLxStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        VAR_Grand_Total = (VAR_4_Sum + VAR_6_Sum + VAR_8_Sum + VAR_10_Sum + VAR_12_Sum + VAR_Mod + VAR_attackOneSum + VAR_attackTwoSum + VAR_attackThreeSum);
        Serial.println (VAR_Grand_Total);
        resetCenterMainStage();
        slots();
        Light_Em_Up(VAR_Grand_Total);
        tft.textMode();
        tft.textEnlarge(4); // temporary increase to font size
        tft.textSetCursor(240, 200 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("Total: ");
        tft.print(VAR_Grand_Total); // Displays the grand total
        tft.graphicsMode();
        clearall();
      }
    }
    // Begin Attack one instructions block
    if ((tx > (ATK1xStart * xScale) && (tx <= (ATK1xStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        VAR_attackOneSum = atk1;
        Serial.println (VAR_attackOneSum);
        resetCenterMainStage();
        tft.textMode();
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(102, 160 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("Attack one modifier will be added");
        tft.textSetCursor(102, 200 ); // defines starting text position for this code block
        tft.print("to the next attack roll: ");
        tft.print(VAR_attackOneSum); // Displays the grand total
        tft.graphicsMode();
      }
    }
    // Begin Attack two instructions block
    if ((tx > (ATK2xStart * xScale) && (tx <= (ATK2xStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        VAR_attackTwoSum = atk2;
        Serial.println (VAR_attackTwoSum);
        resetCenterMainStage();
        tft.textMode();
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(102, 160 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("Attack Two modifier will be added");
        tft.textSetCursor(102, 200 ); // defines starting text position for this code block
        tft.print("to the next attack roll: ");
        tft.print(VAR_attackTwoSum); // Displays the grand total
        tft.graphicsMode();
      }
    }
    // Begin Attack three instructions block
    if ((tx > (ATK3xStart * xScale) && (tx <= (ATK3xStart + DxSize) * xScale))) {
      if ((ty > (105 * yScale) && (ty <= (105 + DySize) * yScale))) {
        VAR_attackThreeSum = atk3;
        Serial.println (VAR_attackThreeSum);
        resetCenterMainStage();
        tft.textMode();
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(102, 160 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("Attack three modifier will be added");
        tft.textSetCursor(102, 200 ); // defines starting text position for this code block
        tft.print("to the next attack roll: ");
        tft.print(VAR_attackThreeSum); // Displays the grand total
        tft.graphicsMode();
      }
    }
    // Begin Modifier instructions block
    if ((tx > (MODxStart * xScale) && (tx <= (MODxStart + DxSize) * xScale))) {
      if ((ty > (105 * yScale) && (ty <= (105 + DySize) * yScale))) {
        VAR_Mod = 0;
        VAR_Mod = buttonPushCounter;
        // Serial.println (VAR_attackThreeSum);
        resetCenterMainStage();
        tft.textMode();
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(102, 160 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("A modifier of ");
        tft.print(VAR_Mod);
        tft.print(" will be");
        tft.textSetCursor(102, 200 ); // defines starting text position for this code block
        tft.print("added to the next roll. ");
        tft.graphicsMode();
        buttonPushCounter = 0;
      }
    }

    // Begin Advantage instructions block
    if ((tx > (ADVxStart * xScale) && (tx <= (ADVxStart + DxSize) * xScale))) {
      if ((ty > (205 * yScale) && (ty <= (205 + DySize) * yScale))) {
        slots();
        int VAR_Grand_Total = (VAR_4_Sum + VAR_6_Sum + VAR_8_Sum + VAR_10_Sum + VAR_12_Sum + VAR_20_Sum + VAR_Mod + VAR_attackOneSum + VAR_attackTwoSum + VAR_attackThreeSum);
        int VAR_D20A1 = 0;
        int VAR_D20A2 = 0;
        VAR_D20A1 = random(1, 21);
        VAR_D20A2 = random(1, 21);
        resetCenterMainStage();
        tft.textMode();
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(102, 140 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("D20 Roll #1: ");
        tft.print(VAR_D20A1);
        tft.textSetCursor(102, 180 ); // defines starting text position for this code block
        tft.print("D20 Roll #2: ");
        tft.print(VAR_D20A2);
        if (VAR_D20A1 >= VAR_D20A2) {
          Light_Em_Up(VAR_Grand_Total + VAR_D20A1);
          tft.textSetCursor(102, 220);
          tft.print("D20 @ Advantage:  "); // prints to the the TFT Screen
          tft.print(VAR_Grand_Total + VAR_D20A1); // prints to the the TFT Screen
        }
        else {
          Light_Em_Up(VAR_Grand_Total + VAR_D20A2);
          tft.textSetCursor(102, 220);
          tft.print("D20 @ Advantage:  "); // prints to the the TFT Screen
          tft.print(VAR_Grand_Total + VAR_D20A2); // prints to the the TFT Screen
        }
        tft.graphicsMode();
        clearall();
        Serial.print("All Cleared");
        delay(2000);
      }
    }


    // Begin disadvantage instructions block
    if ((tx > (DISxStart * xScale) && (tx <= (DISxStart + DxSize) * xScale))) {
      if ((ty > (305 * yScale) && (ty <= (305 + DySize) * yScale))) {
        slots();
        int VAR_Grand_Total = (VAR_4_Sum + VAR_6_Sum + VAR_8_Sum + VAR_10_Sum + VAR_12_Sum + VAR_20_Sum + VAR_Mod + VAR_attackOneSum + VAR_attackTwoSum + VAR_attackThreeSum);
        int VAR_D20D1 = 0;
        int VAR_D20D2 = 0;
        VAR_D20D1 = random(1, 21);
        VAR_D20D2 = random(1, 21);
        resetCenterMainStage();
        tft.textMode();
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(102, 140 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("D20 Roll #1: ");
        tft.print(VAR_D20D1);
        tft.textSetCursor(102, 180 ); // defines starting text position for this code block
        tft.print("D20 Roll #2: ");
        tft.print(VAR_D20D2);
        if (VAR_D20D1 <= VAR_D20D2) {
          Light_Em_Up(VAR_Grand_Total + VAR_D20D1);
          tft.textSetCursor(102, 220);
          tft.print("D20 @ Disadvantage:  "); // prints to the the TFT Screen
          tft.print(VAR_Grand_Total + VAR_D20D1); // prints to the the TFT Screen
        }
        else {
          Light_Em_Up(VAR_Grand_Total + VAR_D20D2);
          tft.textSetCursor(102, 220);
          tft.print("D20 @ Disadvantage:  "); // prints to the the TFT Screen
          tft.print(VAR_Grand_Total + VAR_D20D2); // prints to the the TFT Screen
        }
        tft.graphicsMode();
        clearall();
        Serial.print("All Cleared");
        delay(2000);
      }
    }

    // Begin D20 Button instructions block
    if ((tx > (D20xStart * xScale) && (tx <= (D20xStart + DxSize) * xScale))) {
      if ((ty > (BRyStart * yScale) && (ty <= (BRyStart + DySize) * yScale))) {
        VAR_Grand_Total = (random(1, 21));
        Serial.println (VAR_Grand_Total);
        resetCenterMainStage();
        slots();
        Light_Em_Up(VAR_Grand_Total);
        tft.textMode();
        tft.textEnlarge(4); // temporary increase to font size
        tft.textSetCursor(240, 200 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("Total: ");
        tft.print(VAR_Grand_Total); // Displays the grand total
        tft.graphicsMode();

      }
    }

    // Begin D100 Button instructions block
    if ((tx > (D100xStart * xScale) && (tx <= (D100xStart + DxSize) * xScale))) {
      if ((ty > (BRyStart * yScale) && (ty <= (BRyStart + DySize) * yScale))) {
        VAR_Grand_Total = (random(1, 101));
        Serial.println (VAR_Grand_Total);
        resetCenterMainStage();
        slots();
        Light_Em_Up(VAR_Grand_Total);
        tft.textMode();
        tft.textEnlarge(4); // temporary increase to font size
        tft.textSetCursor(240, 200 ); // defines starting text position for this code block
        tft.textColor(RA8875_CYAN, RA8875_BLACK);
        tft.print("Total: ");
        tft.print(VAR_Grand_Total); // Displays the grand total
        tft.graphicsMode();

      }
    }
    // Begin INITIATIVE Button instructions block
    if ((tx > (INITxStart * xScale) && (tx <= (INITxStart + DxSize) * xScale))) {
      if ((ty > (BRyStart * yScale) && (ty <= (BRyStart + DySize) * yScale))) {
        int VAR_Sub_Total = (random(1, 21));
        VAR_Grand_Total = (VAR_Sub_Total + initiative);
        Serial.println (VAR_Grand_Total);
        Serial.print(initiative);
        delay(2000);
        resetCenterMainStage();
        tft.textMode();
        //        tft.textEnlarge(3); // temporary increase to font size
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.print("Rolling Dice");
        slots();
        Light_Em_Up(VAR_Grand_Total);
        tft.textSetCursor(170, 180 ); // defines starting text position for this code block
        tft.print("D20=" );
        tft.print(VAR_Sub_Total);
        tft.print(" + ");
        tft.print("Initiative");
        tft.print(" +");
        tft.print(initiative);
        tft.textEnlarge(1); // temporary increase to font size
        tft.textSetCursor(170, 220 ); // defines starting text position for this code block
        tft.print("Initiative: ");// prints data to the screen
        tft.print(VAR_Grand_Total); // prints the number of dice in the bag
        tft.graphicsMode(); // returns to graphics mode

      }
    }



    // Begin ADD ONE die Button instructions block
    if ((tx > (P1xStart * xScale) && (tx <= (P1xStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        buttonPushCounter++;
        dieBagPrint();
      }
    }
    // Begin ADD FIVE dice Button instructions block
    if ((tx > (P5xStart * xScale) && (tx <= (P5xStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        buttonPushCounter = buttonPushCounter + 5;
        dieBagPrint();
      }
    }
    // Begin SUBTRACT one die Button instructions block
    if ((tx > (M1xStart * xScale) && (tx <= (M1xStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        buttonPushCounter--;
        dieBagPrint();
      }
    }
    // Begin SUBTRACT FIVE dice Button instructions block
    if ((tx > (M5xStart * xScale) && (tx <= (M5xStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        buttonPushCounter = buttonPushCounter - 5;
        dieBagPrint();
      }
    }
    // Begin CLEAR Button instructions block
    if ((tx > (CLRxStart * xScale) && (tx <= (CLRxStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        clearall();
        resetCenterMainStage();
      }
    }
  }
}

void readSTRSaveStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    // look for saving throw touches will roll saves and add modifer from SD card
    readSaveSubMenu("STR Save", strSave, 101, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Athletics", athletics, 201, 300, 98, 80, xScale, yScale);
    readHomeButton(200, 400, 400, 75, xScale, yScale);
    //    readSaveSubMenu("CON", CON, 301, 300, 98, 80, xScale, yScale);
    //    readSaveSubMenu("INT", INT, 401, 300, 98, 80, xScale, yScale);
    //    readSaveSubMenu("WIS", WIS, 501, 300, 98, 80, xScale, yScale);
    //    readSaveSubMenu("CHA", CHA, 601, 300, 98, 80, xScale, yScale);
  }
}


void readDEXSaveStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    // look for saving throw touches will roll saves and add modifer from SD card
    readSaveSubMenu("DEX Save", dexSave, 101, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Acrobatics", acrobatics, 201, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Slieght of Hand", slightOfHand , 301, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Stealth", stealth, 401, 300, 98, 80, xScale, yScale);
    readHomeButton(200, 400, 400, 75, xScale, yScale);
  }
}


void readCONSaveStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    // look for saving throw touches will roll saves and add modifer from SD card
    readSaveSubMenu("CON Save", conSave, 101, 300, 98, 80, xScale, yScale);
    readHomeButton(200, 400, 400, 75, xScale, yScale);
  }
}

void readINTSaveStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    // look for saving throw touches will roll saves and add modifer from SD card
    readSaveSubMenu("Intelligence Save", intSave, 101, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Arcana", arcana, 201, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("History", history, 301, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Investigation", investigation, 401, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Nature", nature, 501, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Religion", religion, 601, 300, 98, 80, xScale, yScale);
    readHomeButton(200, 400, 400, 75, xScale, yScale);
  }
}


void readWISSaveStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    // look for saving throw touches will roll saves and add modifer from SD card
    readSaveSubMenu("Wisdom Save", wisSave, 101, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Animal  handling", animalHandling, 201, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Insight", insight, 301, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Medicine", medicine, 401, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Perception", perception, 501, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Survival", survival, 601, 300, 98, 80, xScale, yScale);
    readHomeButton(200, 400, 400, 75, xScale, yScale);
  }
}

void readCHASaveStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    // look for saving throw touches will roll saves and add modifer from SD card
    readSaveSubMenu("Charisma Save", charSave, 101, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Deception", deception, 201, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Intimidation", intimidation, 301, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Performance", performance, 401, 300, 98, 80, xScale, yScale);
    readSaveSubMenu("Persuasion", persuasion, 501, 300, 98, 80, xScale, yScale);
    readHomeButton(200, 400, 400, 75, xScale, yScale);
  }
}


void readHealthStage()
{
  float xScale = 1024.0F / tft.width();
  float yScale = 1024.0F / tft.height();
  // Wait around for touch events
  if (! digitalRead(RA8875_INT)) {
    // begin Cody Tappan's debug touch code & touch mapping code
    // begin Cody Tappan's debug touch code & touch mapping code
    Serial.println("TOUCHED");
    while (tft.touched()) {
      tft.touchRead(&tx, &ty);
      delay(50);
    }
    // Update the map values using the maps we defined in the define section of the code
    tx = map(tx, X_CORRECTION_MAP);
    ty = map(ty, Y_CORRECTION_MAP);
    readHealthSubMenu("HL+1", 101, 300, 98, 80, xScale, yScale);
    readHealthSubMenu("HL+5", 201, 300, 98, 80, xScale, yScale);
    readHealthSubMenu("HL-1", 301, 300, 98, 80, xScale, yScale);
    readHealthSubMenu("HL-5", 401, 300, 98, 80, xScale, yScale);
    readHealthSubMenu("AC+1", 501, 300, 98, 80, xScale, yScale);
    readHealthSubMenu("AC-1", 601, 300, 98, 80, xScale, yScale);
    /*
        readSaveSubMenu("Wisdom Save", wisSave, 101, 300, 98, 80, xScale, yScale);
        readSaveSubMenu("Animal  handling", animalHandling, 201, 300, 98, 80, xScale, yScale);
        readSaveSubMenu("Insight", insight, 301, 300, 98, 80, xScale, yScale);
        readSaveSubMenu("Medicine", medicine, 401, 300, 98, 80, xScale, yScale);
        readSaveSubMenu("Perception", perception, 501, 300, 98, 80, xScale, yScale);
        readSaveSubMenu("Survival", survival, 601, 300, 98, 80, xScale, yScale);
    */
    readHomeButton(200, 400, 400, 75, xScale, yScale);

  }
}


void loop()
{
  if (stageNumber == 0) {
    drawMainStage();
    while (drawMainStage) {
      readMainStage();
      if (stageNumber > 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
  if (stageNumber == 1) {
    tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
    drawSaveStage();
    while (drawSaveStage) {
      Serial.println(stageNumber);
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "STR");
      statsLabel(txtStartx + 100, txtStarty, "ATH");
      // Draw saving throw values
      //          x / y / variable
      statsPrint (140, 340, strSave);
      Serial.println(strSave);
      Serial.println(athletics);
      statsPrint (240, 340, athletics);
      readSTRSaveStage();
      if (stageNumber == 0) {
        return;
      }
    }
  }
  if (stageNumber == 2) {
    drawSaveStage();
    while (drawSaveStage) {
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "DEX");
      statsLabel(txtStartx + 100, txtStarty, "ACRO");
      statsLabel(txtStartx + 200, txtStarty, "SOH");
      statsLabel(txtStartx + 285, txtStarty, "STLTH");
      // Draw saving throw values
      //          x / y / variable
      statsPrint (140, 340, dexSave);
      statsPrint (240, 340, acrobatics);
      statsPrint (340, 340, slightOfHand);
      statsPrint (440, 340, stealth);
      Serial.println(stealth);
      readDEXSaveStage();
      if (stageNumber == 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
  if (stageNumber == 3) {
    drawSaveStage();
    while (drawSaveStage) {
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "CON");
      // Draw saving throw values
      //          x / y / variable
      statsPrint (140, 340, conSave);
      readCONSaveStage();
      if (stageNumber == 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
  if (stageNumber == 4) {
    drawSaveStage();
    while (drawSaveStage) {
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "INT");
      statsLabel(txtStartx + 100, txtStarty, "ARCA");
      statsLabel(txtStartx + 200, txtStarty, "HIST");
      statsLabel(txtStartx + 300, txtStarty, "INVS");
      statsLabel(txtStartx + 400, txtStarty, "NATR");
      statsLabel(txtStartx + 500, txtStarty, "RELI");

      // Draw saving throw values
      //          x / y / variable
      statsPrint (140, 340, intSave);
      statsPrint (240, 340, arcana);
      statsPrint (340, 340, history);
      statsPrint (440, 340, investigation);
      statsPrint (540, 340, nature);
      statsPrint (640, 340, religion);
      readINTSaveStage();
      if (stageNumber == 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
  if (stageNumber == 5) {
    drawSaveStage();
    while (drawSaveStage) {
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "WIS");
      statsLabel(txtStartx + 100, txtStarty, "ANML");
      statsLabel(txtStartx + 200, txtStarty, "INSI");
      statsLabel(txtStartx + 300, txtStarty, "MEDI");
      statsLabel(txtStartx + 400, txtStarty, "PERC");
      statsLabel(txtStartx + 500, txtStarty, "SURV");

      // Draw saving throw values
      //          x / y / variable
      statsPrint (140, 340, intSave);
      statsPrint (240, 340, animalHandling);
      statsPrint (340, 340, insight);
      statsPrint (440, 340, medicine);
      statsPrint (540, 340, perception);
      statsPrint (640, 340, survival);
      readWISSaveStage();
      if (stageNumber == 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
  if (stageNumber == 6) {
    drawSaveStage();
    while (drawSaveStage) {
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "CHA");
      statsLabel(txtStartx + 100, txtStarty, "DECE");
      statsLabel(txtStartx + 200, txtStarty, "INTM");
      statsLabel(txtStartx + 300, txtStarty, "PERF");
      statsLabel(txtStartx + 400, txtStarty, "PERS");

      // Draw saving throw values
      //          x / y / variable
      statsPrint (140, 340, charSave);
      statsPrint (240, 340, deception);
      statsPrint (340, 340, intimidation);
      statsPrint (440, 340, performance);
      statsPrint (540, 340, persuasion);
      //      Serial.println("Charisma block load test complete");
      //      Serial.println(persuasion);
      readCHASaveStage();
      if (stageNumber == 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
  if (stageNumber == 7) {
    drawHealthStage();
    while (drawHealthStage) { //  copy savestage for base
      // Draw saving throw text identifiers
      statsLabel(txtStartx, txtStarty, "HL+1");
      statsLabel(txtStartx + 100, txtStarty, "Hl+5");
      statsLabel(txtStartx + 200, txtStarty, "HL-1");
      statsLabel(txtStartx + 300, txtStarty, "HL-5");
      statsLabel(txtStartx + 400, txtStarty, "AC+1");
      statsLabel(txtStartx + 500, txtStarty, "AC-1");

      // Draw saving throw values
      //          x / y / variable
      //      statsPrint (140, 340, charSave);
      //      statsPrint (240, 340, deception);
      //      statsPrint (340, 340, intimidation);
      //      statsPrint (440, 340, performance);
      //      statsPrint (540, 340, persuasion);
      //      Serial.println("Charisma block load test complete");
      //      Serial.println(persuasion);
      readHealthStage(); // copy readSave Stage for base
      if (stageNumber == 0) {
        tft.fillRect (0, 0, 800, 480, RA8875_BLACK); // clears screen for next stage
        return;
      }
    }
  }
}
