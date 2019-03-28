#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pins for leds
const byte BLUE = 10;
const byte RED = 8;
const byte YELLOW = 6;
const byte GREEN = 4;

// number of secuences must play to win
byte plays[25] = {};

// cursor for the position in the sequence while 
// the player is repeating it
byte playPosition = 0;

// position where the sequence ends
byte playLimit = 0;

// for avoid the functionality of button 
// and control if game is started
boolean bloquedButtons = true;
boolean gameStarted = false;

void setup() {

  // Init leds
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  // Init buttons
  pinMode(BLUE + 1, INPUT);
  pinMode(RED + 1, INPUT);
  pinMode(YELLOW + 1, INPUT);
  pinMode(GREEN + 1, INPUT);

  // Init the LCD and show the welcome message
  lcd.begin();
  startMessage();
  
}

void loop() {

  // checking if the games is started
  if(!gameStarted) {
    checkStartGame();
  }   

  // when the buttons are unblocked, it must check if 
  // any is pressed, and execute their corresponsive actions
  if(!bloquedButtons) {
    
    byte buttons[4] = { BLUE + 1, RED + 1, YELLOW + 1, GREEN + 1 };

    for(byte i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {

      if(digitalRead(buttons[i]) == HIGH) {

        // delay for avoid rebounds and electrical noise
        delay(100);

        digitalWrite(buttons[i] - 1, HIGH);
        delay(500);
        digitalWrite(buttons[i] - 1, LOW);
        delay(500);
        
        if(plays[playPosition] == buttons[i] - 1) {
          
          // the player has pressed the correct button. Shows a message to inform it and, 
          // if the player as finished the current sequence, shows the next.
          
          showMessage(0, 1, "Correct!", false);
          playPosition++;
          if(playPosition == playLimit) {
            playPosition = 0;
            bloquedButtons = true;
            showSequence();            
          }
          
        } else {

          // the player has produced an error. Shows a message to inform the error and resets the game.
          showMessage(0, 0, "Error!", true);
          showMessage(0, 1, "You have lost!", false);
          bloquedButtons = true;
          gameStarted = false;
          delay(3000);
          startMessage();
        }
        
      }
          
    }
    
  }
  
}

/**
 * checkStartGame
 * checks if blue button has been pressed and, if so, 
 * shows messages to start the game and the first sequence
 */
void checkStartGame() {
          
  if(digitalRead(BLUE + 1) == HIGH) {

    // delay for avoid rebounds and electrical noise
    delay(100);
    
    if(digitalRead(BLUE + 1) == LOW) {

      // game is started, shows the messages
      gameStarted = true;
      showMessage(0, 0, "Ready?", true);
      delay(2000);
      showMessage(0, 0, "Game started!", true);    
      delay(1000);

      // resets the variables to control positions and shows the first sequence
      playPosition = 0;
      playLimit = 0;
      showSequence();
    }
    
  }
      
}

/**
 * startMessage
 * shows two messages in the LCD about the game
 */
void startMessage() {  
  showMessage(0, 0, "Simon", true);
  showMessage(0, 1, "Blue to start", false);
}

/**
 * showMessage
 * print messages in the LCD
 * col byte the column of the LCD where the message must starts
 * row byte the row of the LCD where the message must printed
 * message String the message to show in the LCD
 * clearScreen boolena if is necessary to clean the screen before show the message
 */
void showMessage(byte col, byte row, String message, boolean clearScreen) {
  if(clearScreen) {
    lcd.clear();
  }
  lcd.setCursor(col, row);
  lcd.print(message);
}

/**
 * showSequence
 * choose aleatory a light and checks if the player has completed 
 * the total of sequences can store the array 'plays[]'. If so, 
 * show the message that the player is a winner, otherwise, plays the sequence
 */
void showSequence() {
  
  randomSeed(millis() / (playLimit + 1));
  byte id = random(4);

  switch(id){
    case 0:
      plays[playLimit] = BLUE;
    break;
    case 1:
      plays[playLimit] = RED;
    break;
    case 2:
      plays[playLimit] = YELLOW;
    break;
    case 3:
      plays[playLimit] = GREEN;
    break;
  }

  playLimit++;

  if(playLimit > sizeof(plays)/sizeof(plays[0])) {

    // player has completed all the sequences of the game, 
    // must shows the message to inform he wins the game
    gameStarted = false;
    showMessage(0, 0, "Congratulations!", true);
    showMessage(0, 1, "You have win!", false);
    delay(3000);
    startMessage();
    
  } else {

    // player still hasn't finished all the sequences, it shows the next
    
    showMessage(0, 0, "Play #" + (String)playLimit, true);
    
    for(byte i = 0; i < playLimit; i++) {
      digitalWrite(plays[i], HIGH);
      delay(1000);
      digitalWrite(plays[i], LOW);
      delay(500);
    }
  
    bloquedButtons = false;
    
  }
  
}
