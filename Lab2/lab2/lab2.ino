/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void waitInput()
{
  while (!Serial.available())
    ;
}

int lcd_bot = 1;
char text_top[100] = "This text is for row 0 and longer than 16 characters.";
char text_bot[100] = "This text is for row 1 and longer than 16 characters.";
bool scroll_en_top = 0;
bool scroll_en_bot = 0;
bool scroll_dir_top = 0;
bool scroll_dir_bot = 0;
int scroll_sp_top = 1000;
int scroll_sp_bot = 1000;

void printMenu()
{
  Serial.println("0 - PRINT THIS STATUS SCREEN");
  Serial.println("LCD ROW 0 (TOP)");
  Serial.print("1 - TEXT : ");
  Serial.println(text_top);
  Serial.print("2 - SCROLLING STATUS : ");
  Serial.println(scroll_en_top ? "ENABLED" : "DISABLED");
  Serial.print("3 - SCROLLING DIRECTION : ");
  Serial.println(scroll_dir_top ? "RIGHT" : "LEFT");
  Serial.print("4 - SCROLLING SPEED (MS): ");
  Serial.println(scroll_sp_top);
  Serial.println("LCD ROW 1 (BOTTOM)");
  Serial.print("5 - TEXT : ");
  Serial.println(text_bot);
  Serial.print("6 - SCROLLING STATUS : ");
  Serial.println(scroll_en_bot ? "ENABLED" : "DISABLED");
  Serial.print("7 - SCROLLING DIRECTION : ");
  Serial.println(scroll_dir_bot ? "RIGHT" : "LEFT");
  Serial.print("8 - SCROLLING SPEED (MS): ");
  Serial.println(scroll_sp_bot);
  Serial.println("\n9 - START LCD SEQUENCE");
}

int getChoice(int min, int max, char *msg, char *err)
{
  int choice;

  do
  {
    Serial.print(msg);
    waitInput();
    choice = Serial.parseInt();
    Serial.println(choice);
    if (choice < min || choice > max)
    {
      Serial.println(err);
    }
  } while (choice < min || choice > max);

  return choice;
}

int getMenuChoice()
{
  return getChoice(0, 9, "\nPLEASE ENTER SETTING NUMBER [0-9]: ", "INVALID SETTING NUMBER");
}

int getScrollSpeed()
{
  return getChoice(100, 10000, "PLEASE ENTER SCROLL SPEED (MS)[100-10000]: ", "INVALID SCROLL SPEED");
}

int getScrollDir()
{
  return getChoice(0, 1, "PLEASE ENTER 0 FOR LEFT AND 1 FOR RIGHT SCROLLING DIRECTION: ", "INVALID SCROLL DIRECTION");
}

int getScrollStatus()
{
  return getChoice(0, 1, "PLEASE ENTER 0 TO DISABLE OR 1 TO ENABLE SCROLLING: ", "INVALID SCROLL STATUS");
}

void getText(char *text, int row)
{
  String buf = "";
  do
  {
    Serial.print("ENTER TEXT FOR ROW ");
    Serial.print(row);
    Serial.print(" (LENGTH [0-100]): ");
    waitInput();
    buf = Serial.readString();
    if (buf.length() > 100)
    {
      Serial.print("\nINVALID TEXT LENGTH (");
      Serial.print(buf.length());
      Serial.println(")");
    }
  } while (buf.length() > 100);
  strcpy(text, buf.c_str());
  text[100] = '\0';
  Serial.println(text);
}

void printText(char *text, int row)
{
  lcd.setCursor(0, row);
  lcd.print(text);
}

void startLCDSequence()
{
}

void menu()
{
  printMenu();
  int choice = getMenuChoice();
  do
  {
    switch (choice)
    {
    case 0:
      printMenu();
      break;
    case 1:
      getText(text_top, 0);
      break;
    case 2:
      scroll_en_top = getScrollStatus();
      break;
    case 3:
      scroll_dir_top = getScrollDir();
      break;
    case 4:
      scroll_sp_top = getScrollSpeed();
      break;
    case 5:
      getText(text_bot, 1);
      break;
    case 6:
      scroll_en_bot = getScrollStatus();
      break;
    case 7:
      scroll_dir_bot = getScrollDir();
      break;
    case 8:
      scroll_sp_bot = getScrollSpeed();
      break;
    default:
      break;
    }
    choice = getMenuChoice();
  } while (choice != 9);

  Serial.println("\nSTARTING LCD SEQUENCE");
  Serial.println("STARTING IN 3..");
  delay(1000);
  Serial.println("STARTING IN 2..");
  delay(1000);
  Serial.println("STARTING IN 1..");
  delay(1000);

  startLCDSequence();
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  printText(text_top, 0);
  printText(text_bot, 1);
}

void loop()
{
  menu();
}
