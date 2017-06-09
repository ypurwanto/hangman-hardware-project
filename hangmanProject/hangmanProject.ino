// LCD 16x2 library
#include <LiquidCrystal.h>
#include "pitches.h"
//TFT libraries
#include <TFT.h>  // Arduino TFT library
#include <SPI.h>
#include <Esplora.h>
#include <SD.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8
#define SD_CS 8  // Chip select line for SD card in Esplora

/*-------------------------------------------------------------------------------------------------------*/

//TFT LCD stuff program
TFT TFTscreen = TFT(cs, dc, rst);//(10, 9, 8) // create an instance of the library
char sensorPrintout[4]; // char array to print to the screen

void tftTest(){
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  TFTscreen.setRotation(0);//makes the screen vertical
  TFTscreen.background(0, 0, 0);  // clear the screen with a black background

  // write the static text to the screen
  TFTscreen.stroke(255, 255, 255);  // set the font color to white
  TFTscreen.setTextSize(3); // set the font size
  TFTscreen.text("Can\nyou\nsurvive\nthe\nzombies\n!?", 0, 0);  // write the text to the top left corner of the screen
  delay(3000);
}

/*-------------------------------------------------------------------------------------------------------*/

void jeopardyTone() {
  //notes in the melody:
  int melody[] = {NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, 
                  NOTE_G4, 0, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5, NOTE_E5, 0, NOTE_D5, NOTE_C5};

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {2, 2, 2, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2};

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 15; thisNote++){  //where is this 15 from?
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
/*-------------------------------------------------------------------------------------------------------*/

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 0, 1, 2, 3);

const String alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const String list[10] = {"RESPECTXXX", "SHORTXXXXX", "LOVINGXXXX", "COHERENTXX", "CONFIDENTX",
                           "GUNXXXXXXX", "PRICKLYXXX", "DAILYXXXXX", "CONCERNEDX", "SOCIETYXXX"
                          };
String selectWord;
char reveal[16];

/*-------------------------------------------------------------------------------------------------------*/

void printToLCD(){  //are we going to change this?
  //randonly select the random word
  randomSeed(analogRead(A1));
  long r = random(10);
  selectWord = list[r]; //get the random word using r
  char temp[16];
  int idx;
  int cpy;
  String letterOrWord = "Letter or Word";
   
  for(cpy = 0; cpy < 16; cpy++){
   temp[cpy] = selectWord[cpy];
  }
    
  //get length of selectword
  int l = strlen(temp);

//  start the dashes string
    char dash = '_';

  //write a for loop that creates a string of __ of length of the selcted word
  for (idx = 0; idx < l; idx++) {
    reveal[idx] = dash;
  }
  reveal[l] = '\0';

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //set to first line
  lcd.setCursor(0, 0);
  //Print a message to the LCD.
  lcd.print(letterOrWord);
  //set to second line
  lcd.setCursor(0, 1);
  // Print a message to the LCD second line.
  lcd.print(alpha);
  lcd.setCursor(0, 1);
}

/*-------------------------------------------------------------------------------------------------------*/

int alphaIdx = 0;
int cursorIdx = 0;

void buttonPins(){
  pinMode(A4, INPUT); //left button
  pinMode(A3, INPUT); //right button
  pinMode(A2, INPUT); //select button
}

/*-------------------------------------------------------------------------------------------------------*/

void shiftCursor() {
  //number of times the alphabet array is getting shifted
  int shiftNum = 0;
  int idx1 = 0;
  int idx2 = 0;
  int idx3 = 0;
  int alphaIdx = alphaIdx - 1;
  int cursorIdx = cursorIdx - 1;
  int alphaGap1 = 10;
  int alphaGap2 = 15;
  int alphaLimit = 25;
  int alphaEnd = 26;
  int lcdSpaceAvail = 16;

  //if LEFT BUTTON is pressed
  if (digitalRead(A4) == 0){
    if (alphaIdx < 0){
      alphaIdx = 0;
    }

    if (alphaIdx < alphaGap1 && alphaIdx >= 0 && cursorIdx < 0){
      shiftNum = alphaIdx;
      lcd.setCursor(0, 1);

      for (idx3 = 0; idx3 < lcdSpaceAvail; idx3++){
        lcd.print(alpha[shiftNum]);
        lcd.setCursor(idx3 + 1, 1);
        shiftNum = shiftNum + 1;
      }
      cursorIdx = 0;
      lcd.setCursor(cursorIdx, 1);
    } 
    else{
      lcd.setCursor(cursorIdx, 1);
    }
  }

  //if RIGHT BUTTON is pressed
  if (digitalRead(A3) == 0){
    //increment alphaIndex to shift the cursor once to the right
    alphaIdx = alphaIdx + 1;
    cursorIdx = cursorIdx + 1;
   
    //if alphaindex is higher than the alphabet limit then set it to the max which is 25
    if (alphaIdx > alphaGap2 && cursorIdx > alphaGap2){
      cursorIdx = alphaGap2;
      lcd.setCursor(cursorIdx, 1);
    }
 
    if (alphaIdx > alphaLimit){
      alphaIdx = alphaLimit;
    }

    //the shifting begins here if cursor is on far right
    if (alphaIdx > alphaGap2 && alphaIdx < alphaEnd){
      shiftNum = alphaIdx - alphaGap2;   //shift n-times
      lcd.setCursor(0, 1);  //set cursor at the biginning of line 2 to start printing the new letters

      //loop reprints the alphabet array according to the shift
      for (idx2 = 0; idx2 < lcdSpaceAvail; idx2++){
        lcd.print(alpha[shiftNum]); //prints a character in the alphabet array
        lcd.setCursor(idx2 + 1, 1); //increment the cursor for the new place for the next letter
        shiftNum = shiftNum + 1;  //increment character in array
      }

      //set the cursor at the end of the line if disappears
      cursorIdx = 15;
      lcd.setCursor(cursorIdx, 1);
      shiftNum = 0;
    } 
    else{
      lcd.setCursor(cursorIdx, 1);  //set new location of cursor
    }
  }
}

/*-------------------------------------------------------------------------------------------------------*/

char selected = '\0';
void getSelect(){
  if (digitalRead(A2) == 0){
    selected = alpha[alphaIdx];
    lcd.setCursor(14, 0);
    lcd.print(selected);
    lcd.setCursor(cursorIdx, 1);
  }
}

/*-------------------------------------------------------------------------------------------------------*/

int start = 0;
void displayProgress(){
  if (start == 0){
    //display the guy on the tft screen
    TFTscreen.background(0, 0, 0);  // clear the screen with a black background
    TFTscreen.setTextSize(3);
    TFTscreen.text("\n   o\n  /|\\\n  / \\", 0, 0); // write the text to the top left corner of the screen
    TFTscreen.setTextSize(2);
    TFTscreen.text( reveal, 4, 110);
    TFTscreen.setTextSize(1);
    TFTscreen.text("6 chances left", 20, 150);
    start = 1;
  }
}

/*-------------------------------------------------------------------------------------------------------*/

void setup(){
  //jeopardyTone();
  printToLCD();
  buttonPins();
  tftTest();
}

/*-------------------------------------------------------------------------------------------------------*/

void loop(){
  lcd.blink();//blinks the cursor
  delay(50);
  displayProgress();
  delay(50);
  shiftCursor();//shifts the alphabet array within row 1 as the cursor reaches either ends of the screen
  delay(50);
  getSelect();//selects the character the cursor is set on to be a choice for the game
  delay(50);
}

/*-------------------------------------------------------------------------------------------------------*/

//NEW ADDED FUNCTION

//Displaying an initial image avail on the SD card, and renders it to the screen
PImage imgCase1, imgCase2, imgCase3, imgCase4, imgCase5, imgCase6, imgDefaultCase, imgZombieHead;

void displayImage() {
  EsploraTFT.begin(); // initialize the screen
  //SD.begin(SD_CS);  // initialize the SD card
  EsploraTFT.background(0, 0, 0); // set the background
  
  // load the image into the named instance of PImage 
  imgZombieHead = EsploraTFT.loadImage("z_head.png");  
  imgCase1 = EsploraTFT.loadImage("man_case1.png"); 
  imgCase2 = EsploraTFT.loadImage("man_case2.png");  
  imgCase3 = EsploraTFT.loadImage("man_case3.png");  
  imgCase4 = EsploraTFT.loadImage("man_case4.png"); 
  imgCase5 = EsploraTFT.loadImage("man_case5.png");  
  imgCase6 = EsploraTFT.loadImage("man_case6.png"); 
  imgDefaultCase = EsploraTFT.loadImage("man_default.png");

  switch (noMatchCounter){
  case 1:
      // if it is a valid image file, turn the Esplora's LED green
      if (imgZombieHead.isValid() && imgCase1.isValid()) {
           Esplora.writeGreen(255); //!!we have to allocate the area of white space for image!!
      }
      else{
          Esplora.writeRed(255);  // if it is not valid, turn the LED red
      }
      
      //display img: z_head
      EsploraTFT.image(imgZombieHead, 0, 0);
      delay(2000);
      //display img: man_case1
      EsploraTFT.image(imgCase1, 0, 0);
  case 2:
      //display img: z_head
      //display img: man_case2
    break;
  case 3:
      //display img: z_head
      //display img: man_case3
    break;
  case 4:
      //display img: z_head
      //display img: man_case4
    break;
  case 5:
      //display img: z_head
      //display img: man_case5
    break;
  case 6:
      //display img: z_head
      //display img: man_case6
    break;
  default:
      //display img: man_default
    break;
  }
}

void loop(){

}


