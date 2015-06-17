// Friday June 06 2015
// Play Tic-Tac-Toe with the circuit board
/* run on an ATTiny circuit board with 9 pixels arranged in a 3x3 grid*/
// By Ethan Middleton
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(9, PIN, NEO_GRB + NEO_KHZ800);

// button variables
int buttonMove;
boolean newStateA, oldStateA;
int buttonSelect;
boolean newStateB, oldStateB;

// LED variables
uint32_t black, red, green, blue;
uint32_t leds[] = {
  black, black, black, black, black, black, black, black, black};
int pos;
uint32_t turn;
uint32_t turnShow;

// Variables for testing if won
uint32_t won;
int winpos[3], tie;

// Counter variables
int blueCount, blueCountCount, flashSelectCount, flashWinCount;
boolean flashSelectState, posChange, flashWinState;

void setup(){
  // set button inputs
  buttonMove = 1;
  buttonSelect = 2;

  // Set colours
  black = strip.Color(0, 0, 0);
  red = strip.Color(64, 0, 0);
  green = strip.Color(0, 64, 0);
  blue = strip.Color(0, 0, 64);

  // Set victory condition variables
  won = black;
  tie = 0;

  // Set counter variables
  blueCount = 65;
  blueCountCount = 0;
  flashSelectCount = 0;
  flashWinCount = 0;
  flashSelectState = true;
  posChange = true;
  flashWinState = true;

  turn = red;
  turnShow = red;

  pinMode(buttonMove, INPUT);
  pinMode(buttonSelect, INPUT);

  pos = 0;

  // Setup neopixel
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// Method that resets the arduino
void(* resetFunc) (void) = 0;


void loop(){
  // Get new button values
  newStateB = digitalRead(buttonSelect);
  newStateA = digitalRead(buttonMove);

  // if noone has won
  if (won == black){
    // move the "cursor"
    pos = buttonCheck(pos);
    // Update LEDs
    turnLEDs();
    // Go to next turn
    nextTurn();
    // Flash the "cursor"
    flashSelect();
    // Check if anyone won
    testWon();
  }
  // If all tiles are full
  else if (won == blue){
    tieLEDs();
  }
  // if someone has won
  else if (won == red || won == green){
    winFlash();
  }
  // update old button values
  oldStateA = newStateA;
  oldStateB = newStateB;
}

void testWon(){
  // Test for victory
  for (int i = 0; i < 3; i++){
    // if diagonal
    if (i == 0 && leds[4] == leds[0] && leds[8] == leds[0] && leds[0] != black){
      // set victory values
      won = leds[i];
      winpos[0] = 0;
      winpos[1] = 4;
      winpos[2] = 8;
    }
    // if vertical
    else if (i == 0 && leds[0] == leds[5] && leds[5] == leds[6] && leds[0] != black){
      // set victory values
      won = leds[i];
      winpos[0] = 0;
      winpos[1] = 5;
      winpos[2] = 6;
    }
    // if vertical
    else if (i == 1 && leds[1] == leds[4] && leds[4] == leds[7] && leds[1] != black){
      // set victory values
      won = leds[i];
      winpos[0] = 1;
      winpos[1] = 4;
      winpos[2] = 7;
    }
    // if vertical
    else if (i == 2 && leds[2] == leds[3] && leds[3] == leds[8] && leds[0] != black){
      // set victory values
      won = leds[i];
      winpos[0] = 2;
      winpos[1] = 3;
      winpos[2] = 8;
    }
    // if horizontal
    else if (leds[3 * i] == leds[3 * i + 1] && leds[3 * i + 1] == leds[3 * i + 2] && leds[3 * i + 2] != black){
      // set victory values
      won = leds[i*3];
      winpos[0] = 3*i;
      winpos[1] = 3*i + 1;
      winpos[2] = 3*i + 2;
    } 
    // if other diagonal
    else if (i == 2 && leds[4] == leds[2] && leds[6] == leds[2] && leds[2] != black){
      // set victory values
      won = leds[i];
      winpos[0] = 2;
      winpos[1] = 4;
      winpos[2] = 6;
    }
  }
}

void nextTurn(){
  // if select button is pressed
  if (newStateB == HIGH && oldStateB == LOW){
    // debouncing code
    delay(175);
    newStateB = digitalRead(buttonSelect);
    if (newStateB == LOW){
      // select current led
      leds[pos] = turn;
      // force led to be on
      posChange = true;
      // set position beginning
      pos = 0;
      // move cursor to first open spot
      while (leds[pos] != black){
        pos++;
        // if board is full then begin tie sequence
        if (pos == 8 && leds[8] != black){
          won = blue;
          break;
        }
      }
      // switch turn
      if (turn == green){
        turn = red;
        return;
      }
      else {
        turn = green;
      }
    }
  }
}

void turnLEDs(){
  // update LEDs
  for (int i = 0; i < 9; i++){
    if (i != pos){
      //if (i == 3){
      //  strip.setPixelColor(5, leds[5]);
      //}
      //else if (i == 5){
      //  strip.setPixelColor(3, leds[3]);
      //}
      //else{
        strip.setPixelColor(i, leds[i]);
      //}
    }
  }
  // show cursor
  strip.setPixelColor(pos, turnShow);
  // update neopixel
  strip.show();
}

int buttonCheck(int pos){
  // if move button is pressed
  if (newStateA == HIGH && oldStateA == LOW){
    // debouncing code
    delay(175);
    newStateA = digitalRead(buttonMove);
    if (newStateA == LOW){
      // force LED to be on
      posChange = true;
      // move cursor 1
      pos++;
      //if cursor is at the end then wrap
      if (pos == 9){
        pos = 0;
      }
      // move cursor to nearest available space
      while (leds[pos] != black){
        pos++;
        // if cursor is at the end then wrap
        if (pos == 9){
          pos = 0;
        }
      }
    }
  }
  else {
    // don't force led to be on
    posChange = false;
  }
  // return position
  return pos;
}

void tieLEDs(){
  // if it has been long enough
  if (blueCount > 0 && blueCountCount % 500 == 0){
    // decrease bluecount
    blueCount--;
    // update colours
    for (int j = 0; j < 9; j++){
      strip.setPixelColor(j, 0, 0, blueCount);
    }
    // update neopixel
    strip.show();
  }
  // increase bluecount
  blueCountCount++;
  // if animation is over reset arduino
  if (blueCount == 0){
    resetFunc();
  }
}

void flashSelect(){
  // if cursor has not been moved recently
  if (! posChange){
    // if it has been long enough
    if (flashSelectCount % 1000 == 0){
      // if LED is on
      if (flashSelectState){
        // turn it off
        flashSelectState = false;
        turnShow = black;
      }
      else {
        // turn it on
        flashSelectState = true;
        turnShow = turn;
      }
    }
  }
  else {
    // turn it on
    turnShow = turn;
  }
  flashSelectCount++;
}

void winFlash(){
  // if it has been long enough
  if (flashWinCount % 1000 == 0){
    // if leds are on
    if (flashWinState){
      // turn them off
      flashWinState = false;
      turnShow = black;
    }
    else {
      // turn them on
      flashWinState = true;
      turnShow = won;
    }
  }
  // update strip
  for (int i = 0; i < 3; i++){
    strip.setPixelColor(winpos[i], turnShow);
  }
  flashWinCount++;
  // update neopixel
  strip.show();
  // if select button pressed
  if (newStateB == HIGH && oldStateB == LOW){
    // debouncing code
    delay(175);
    newStateB = digitalRead(buttonSelect);
    if (newStateB == LOW){
      // reset arduino
      resetFunc();
    }
  }
}

