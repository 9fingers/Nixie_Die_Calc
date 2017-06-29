

/******************************************************************
  This is an example for the Adafruit RA8875 Driver board for TFT displays
  ---------------> http://www.adafruit.com/products/1590
  The RA8875 is a TFT driver for up to 800x480 dotclock'd displays
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
#define RA8875_CS 53
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


Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;


// define placeholder columns
int VAR_Grand_Total_Ones;        // Grand total Ones column
int VAR_Grand_Total_Tens;        // Grand Total Tens Column
int VAR_Grand_Total_Hund;        // Grand total Hundreds column

// character setup bit for run once.
boolean charIsSetup = false;


// Global button sizing variables
int TRyStart = 10; // y start position
int DxSize = 75; // w size in pixels
int DySize = 75; // y size in pixels
int DRad = 15; // corner radius

// Top Row Button Map positioning definitions
int TopiconSpace = 64; // X spacing between icons
int P1xStart = 15; // X starting position of bottom row
int P5xStart = ((P1xStart + DxSize) + TopiconSpace); // ****************************************
int M1xStart = ((P5xStart + DxSize) + TopiconSpace); //*****************************************
int M5xStart = ((M1xStart + DxSize) + TopiconSpace); // Next buton position based on first button in row, and previously defined variables.
int CLRxStart = ((M5xStart + DxSize) + TopiconSpace); //*****************************************
int TTLxStart = ((CLRxStart + DxSize) + TopiconSpace); //*****************************************


// character setup buttons
int charIconxSize = 75;
int charIconySize = 75;
int charIconXSpace = 15; // X spacing between icons
int charIconRad = 10;
int PcharIconYStart =  100;

int PSTRxStart = (15 + charIconXSpace + (charIconxSize * 1)); // X starting position of character plus icon
int PDEXxStart = (15 + charIconXSpace + (charIconxSize * 2));
int PCONxStart = (15 + charIconXSpace + (charIconxSize * 3));
int PINTxStart = (15 + charIconXSpace + (charIconxSize * 4));
int PWISxStart = (15 + charIconXSpace + (charIconxSize * 5));
int PCHAxStart = (15 + charIconXSpace + (charIconxSize * 6));
int PMELxStart = (15 + charIconXSpace + (charIconxSize * 7));
int PSPLxStart = (15 + charIconXSpace + (charIconxSize * 8));

int McharIconYStart =  300;

int MSTRxStart = (15 + charIconXSpace + (charIconxSize * 1)); // X starting position of character plus icon
int MDEXxStart = (15 + charIconXSpace + (charIconxSize * 2));
int MCONxStart = (15 + charIconXSpace + (charIconxSize * 3));
int MINTxStart = (15 + charIconXSpace + (charIconxSize * 4));
int MWISxStart = (15 + charIconXSpace + (charIconxSize * 5));
int MCHAxStart = (15 + charIconXSpace + (charIconxSize * 6));
int MMELxStart = (15 + charIconXSpace + (charIconxSize * 7));
int MSPLxStart = (15 + charIconXSpace + (charIconxSize * 8));




//Bottom Row Button Maps positioning definitions
int BTMiconSpace = 41; // X spacing between icons
int BRyStart = 395; // y start position
int D4xStart = 13; // X starting position of bottom row
int D6xStart = ((D4xStart + DxSize) + BTMiconSpace); // ****************************************
int D8xStart = ((D6xStart + DxSize) + BTMiconSpace); // ****************************************
int D10xStart = ((D8xStart + DxSize) + BTMiconSpace); // Next buton position based on first button in roa, and previously defined variables.
int D12xStart = ((D10xStart + DxSize) + BTMiconSpace); // ****************************************
int D20xStart = ((D12xStart + DxSize) + BTMiconSpace); // ****************************************
int D100xStart = ((D20xStart + DxSize) + BTMiconSpace); // ****************************************


// text display block definition for button labels
int TXTxStart = 100;
int TXTyStart = 100;
int TXTxWidth = 600;
int TXTyWidth = 280;

// Saving Throw Stats (to be defined by user on startup)
int STR = 4;
int DEX = 2;
int CON = 1;
int INT = 0;
int WIS = 0;
int CHA = 4;


// spell hit modifier (to be defined by user on startup)
int VAR_S_Mod = 0;
// melee hit modifier (to be defined by user on startup)????????????? TBD ????????????????
int VAR_M_Mod = 0;
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

// temporary place holder for sum of a singular die type (all D4 dice rolled, or all D5 dice rolled.
int DieRollSum = 0;
// number of dice in the dice bag
int buttonPushCounter = 0;

// total of all dice and modifiers for applicable action
int VAR_Grand_Total;

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

// labels for saving throw boxes
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
void resetCenterStage()
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

// draws all rectangular buttons and thier labels
void drawstage()
{
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
  buttonlabel("20", D20xStart + 4, BRyStart);

  tft.fillRoundRect(D100xStart , BRyStart, DxSize , DySize, DRad, RA8875_WHITE);
  tft.fillRoundRect(D100xStart + 3 , BRyStart + 3, DxSize - 6 , DySize - 6, DRad, RA8875_BLUE);
  tft.fillRoundRect(D100xStart + 6 , BRyStart + 12, DxSize - 12 , DySize - 24, DRad - 3, RA8875_CYAN);
  buttonlabel("100", D100xStart - 10 , BRyStart);
  resetCenterStage();
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





// Draws the saving throw set and roll buttons
//

// defines the touch maps for the saving throw buttons
void savetouchread(String saveName, int saveValue, int xStart, int yStart, int xSize, int ySize, float xScale, float yScale)
{
  if ((tx > (xStart * xScale) && (tx <= (xStart + xSize) * xScale))) {
    if ((ty > (yStart * yScale) && (ty <= (yStart + ySize) * yScale))) {
      // Preps print area for new text
      int saveRoll = random (1, 21);
      int saveRollSum = (saveRoll + saveValue);
      Serial.println (saveRoll);
      Serial.println (saveValue);
      Serial.print (saveName);
      Serial.print (saveRollSum);
      resetCenterStage(); // clears any previously draw data
      slots();
      Light_Em_Up(saveRollSum);
      tft.textMode(); // sets to text mode
      tft.textEnlarge(1); // temporary increase to font size
      tft.textSetCursor(280, 180 ); // defines starting text position for this code block
      tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
      tft.print("D20=" );
      tft.print(saveRoll);
      tft.print(" + ");
      tft.print(saveName);
      tft.print(" +");
      tft.print(saveValue);
      tft.textEnlarge(1); // temporary increase to font size
      tft.textSetCursor(270, 210 ); // defines starting text position for this code block
      tft.print("Saving throw: ");// prints data to the screen
      tft.print(saveRollSum); // prints the number of dice in the bag
      tft.graphicsMode(); // returns to graphics mode
    }
  }
}









void dieBagPrint()
{
  Serial.println (buttonPushCounter);
  resetCenterStage(); // clears any previously draw data
  tft.textMode(); // sets to text mode
  tft.textEnlarge(2); // temporary increase to font size
  tft.textSetCursor(110, 190 ); // defines starting text position for this code block
  tft.textColor(RA8875_CYAN, RA8875_BLACK); // sets text to CYAN and background to BLACK
  tft.print("# of dice in the bag: ");// prints data to the screen
  tft.print(buttonPushCounter); // prints the number of dice in the bag
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
  tft.textEnlarge(2); // temporary increase to font size
  tft.textSetCursor(xStart + 10, yStart + 10); // defines starting text position for this code block
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

// defines the touch maps for the bottom row of buttons
void dietouchread(int dieType, int DxStart, int DyStart, int DxSize, int DySize, float xScale, float yScale)
{
  if ((tx > (DxStart * xScale) && (tx <= (DxStart + DxSize) * xScale))) {
    if ((ty > (DyStart * yScale) && (ty <= (DyStart + DySize) * yScale))) {
      // Preps print area for new text
      resetCenterStage();
      // rolls dice, and prints to TFT
      rolldice(dieType);
    }
  }
}

// defines the touch maps for the character setup.
void charSaveTouchRead(int buttonType, int DxStart, int DyStart, int DxSize, int DySize, float xScale, float yScale)
{
  if ((tx > (DxStart * xScale) && (tx <= (DxStart + DxSize) * xScale))) {
    if ((ty > (DyStart * yScale) && (ty <= (DyStart + DySize) * yScale))) {
      // Preps print area for new text
      charIsSetup = true;
      Serial.print ("WTF WON'T THIS WORK!!!!");


    }
  }
}


void drawSetupStage()
{

  // Draw plus 1 buttons
  tft.fillRoundRect(PSTRxStart , PcharIconYStart, charIconxSize , charIconySize, charIconRad, RA8875_WHITE);
  tft.fillRoundRect(PSTRxStart + 3 , PcharIconYStart + 3, charIconxSize - 6 , charIconySize - 6, charIconRad, RA8875_BLUE);
  tft.fillRoundRect(PSTRxStart + 6 , PcharIconYStart + 12, charIconxSize - 12 , charIconySize - 24, charIconRad - 3, RA8875_CYAN);
  buttonlabel("+1", PSTRxStart , PcharIconYStart);

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
  DieRollSum = 0;                  //
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

void setup()
{
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
  drawstage();

}
// ******************************************//
// ******************************************//
// ****END Touch screen and display setup****//
// ******************************************//
// ******************************************//






void loop()
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
    dietouchread(10, D10xStart, BRyStart, DxSize, DySize, xScale, yScale);
    dietouchread(12, D12xStart, BRyStart, DxSize, DySize, xScale, yScale);
    dietouchread(20, D20xStart, BRyStart, DxSize, DySize, xScale, yScale);
    dietouchread(100, D100xStart, BRyStart, DxSize, DySize, xScale, yScale);

    // look for saving throw touches
    savetouchread("STR", STR, 101, 300, 98, 80, xScale, yScale);
    savetouchread("DEX", DEX, 201, 300, 98, 80, xScale, yScale);
    savetouchread("CON", CON, 301, 300, 98, 80, xScale, yScale);
    savetouchread("INT", INT, 401, 300, 98, 80, xScale, yScale);
    savetouchread("WIS", WIS, 501, 300, 98, 80, xScale, yScale);
    savetouchread("CHA", CHA, 601, 300, 98, 80, xScale, yScale);


    // Begin Total Button instructions block
    if ((tx > (TTLxStart * xScale) && (tx <= (TTLxStart + DxSize) * xScale))) {
      if ((ty > (TRyStart * yScale) && (ty <= (TRyStart + DySize) * yScale))) {
        VAR_Grand_Total = (VAR_4_Sum + VAR_6_Sum + VAR_8_Sum + VAR_10_Sum + VAR_12_Sum + VAR_Mod);
        Serial.println (VAR_Grand_Total);
        resetCenterStage();
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

    // Begin Total Button instructions block
    if ((tx > (D20xStart * xScale) && (tx <= (D20xStart + DxSize) * xScale))) {
      if ((ty > (BRyStart * yScale) && (ty <= (BRyStart + DySize) * yScale))) {
        VAR_Grand_Total = (random(1, 21));
        Serial.println (VAR_Grand_Total);
        resetCenterStage();
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

    // Begin Total Button instructions block
    if ((tx > (D100xStart * xScale) && (tx <= (D100xStart + DxSize) * xScale))) {
      if ((ty > (BRyStart * yScale) && (ty <= (BRyStart + DySize) * yScale))) {
        VAR_Grand_Total = (random(1, 101));
        Serial.println (VAR_Grand_Total);
        resetCenterStage();
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
        resetCenterStage();
      }
    }
    /*
        if (!charIsSetup) {
          Serial.println("Setup Needed");
          charSaveTouchRead(1, PSTRxStart, PcharIconYStart, charIconxSize, charIconySize, xScale, yScale);

        } while (1);
    */
  }
}
