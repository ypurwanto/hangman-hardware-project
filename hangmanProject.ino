
// LCD 16x2 library
#include <LiquidCrystal.h>
#include "pitches.h"

///////////////////////////////////////////////////////


//TFT LCD stuff program

//TFT libraries
#include <TFT.h>
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);//(10, 9, 8)

// char array to print to the screen
char sensorPrintout[4];

void tftTest() {
  

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("Sensor Value :\n ", 0, 0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(5);
}



//////////////////////////////////////////////////////////////////

void jeopardyTone(){
  //notes in the melody:
  int melody[] = {
    NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, 0, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5, NOTE_E5, 0, NOTE_D5, NOTE_C5
  };

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {
    2, 2, 2, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2
  };

    // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 15; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(A5, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(A5);
    }
}
//////////////////////////////////////////////////////////////////
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 0, 1, 2, 3);

const String alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void printToLCD(){

  
  String init = "_______";
  String test = "banana";

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //set to first line
  lcd.setCursor(0, 0);
  //Print a message to the LCD.
  lcd.print(init);
  //set to second line
  lcd.setCursor(0, 1);
    // Print a message to the LCD second line.
  lcd.print(alpha);
  lcd.setCursor(0, 1);
  
}
///////////////////////////////////////////////////////////////

int alphaIndex = 0;
int cursorIdx = 0;


void buttonPins(){

  //left button
  pinMode(A4, INPUT);
  //right button
  pinMode(A3, INPUT);
  //select button
  pinMode(A2, INPUT)
}

///////////////////////////////////////////////////////////////

void setup() {

//jeopardyTone();
printToLCD();
buttonPins();
tftTest();

}



void shiftCursor(){

  //number of times the alphabet array is getting shifted
  int shiftNum = 0;
  int idx1 = 0;
  int idx2 = 0;
  




  //if left button is pressed
  if(digitalRead(A4) == 0)
   {
    alphaIndex = alphaIndex - 1;
    
      if(alphaIndex < 0){

      shiftNum = alphaIndex - 15;


      for(idx1 = 0; idx1 < 16; idx1++){




      }
      
      }else{

    
            lcd.setCursor(alphaIndex, 1); 

           }

  }
  




  //if right button is pressed
  if(digitalRead(A3) == 0)
  {
    //increment alphaIndex to shift the cursor once to the right
    alphaIndex = alphaIndex + 1;
    cursorIdx = alphaIndex;

    //if alphaindex is higher than the alphabet limit then set it to the max which is 25
    if(alphaIndex > 15)
    {
      
      cursorIdx = 15;
      lcd.setCursor(cursorIdx, 1);
    
      if(alphaIndex > 25)
        {
          alphaIndex = 25;
       }
       
    }
    //the shifting begins here if cursor is on far right
    if(alphaIndex > 15 && alphaIndex < 26)
      {
        //how many times to shift
        shiftNum = alphaIndex - 15;
        
        //set cursor at the biginning of line 2 to start printing the new letters
        lcd.setCursor(0, 1);

        //loop reprints the alphabet array according to the shift
        for(idx2 = 0; idx2 < 16; idx2++)
          {
            //prints a character in the alphabet array
            lcd.print(alpha[shiftNum]);

            //increment the cursor for the new place for the next letter
            lcd.setCursor(idx2+1, 1);

            //increment character in array
            shiftNum = shiftNum +1;
      
          }
      
        //set the cursor at the end of the line if disappears
        cursorIdx = 15;
        lcd.setCursor(cursorIdx, 1);
    
       }else{
             //set new location of cursor
             lcd.setCursor(cursorIdx, 1);
            }

   }
}
   
 
   




void loop() {
  
  lcd.blink();
  shiftCursor();


  //////////////////////////////////////////////////////////


//tft screen program


  // Read the value of the sensor on A0
  String sensorVal = String(analogRead(A0));

  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 4);

  // set the font color
  TFTscreen.stroke(255, 255, 255);
  // print the sensor value
  TFTscreen.text(sensorPrintout, 0, 20);
  // wait for a moment
  delay(250);
  // erase the text you just wrote
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 0, 20);

}

