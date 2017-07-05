#include <LiquidCrystal.h>
#include "pitches.h"

LiquidCrystal lcd(0,1,8,9,10,11);
const int menuPin = 2;
const int selectPin = 12;
const int piezoPin = 4;

//flags
int pooped = false; //keep track of buddy's waste of course
int onHomeScreen = true; //true since this is the default view

int menuState = HIGH; //the state of the left button, used for toggling menu options
int selectState = HIGH; //the state of the right button, used for selecting menu options

unsigned long prev_move = 0;
unsigned long move_delay = 700;

int menuPresses = 0;
int count = 0;

void setup() {
  pinMode(menuPin,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  lcd.begin(16,2);

  //startup screen
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("VIRTUAL PET");
  lcd.setCursor(0,1);
  delay(2500);
  lcd.print("by Leslie Cheung");
  delay(2000);

  drawPet(2); //smile

  welcomeTone();
}

void loop() { 
  menuState = digitalRead(menuPin);
    
  if(menuState == LOW) {
    menuPresses++;
    onHomeScreen = false;
    delay(250);
  }

  if(onHomeScreen) { pet_movements(); }

  //event handling for menu button presses
  switch(menuPresses) {
    case 1:
    menuStart();
    break;
    case 2:
    feed();
    break;
    case 3:
    rest();
    break;
    case 4:
    bath();
    break;
    case 5:
    exitMenu();
    break;
    case 6:
    menuPresses = 2;
    break;
  }
}

void pet_movements() {
  if(!onHomeScreen) { return; } // only want to scroll if on home screen
  unsigned long currentTime = millis();
  if(currentTime - prev_move >= move_delay) {
    prev_move = currentTime;
    if(count < 3) {
      lcd.scrollDisplayLeft();
      count++;
    }
    else if(count < 6) {
      lcd.scrollDisplayRight();
      count++;
    }
    else { count = 0; }
  }
  if(currentTime % 30000 == 0 && pooped == false) {
    drawPet(4); //change mood to frown
    drawPoop();
    pooped = true;
    poopAlertTone();
  }
}
