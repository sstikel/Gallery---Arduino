#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd address 0x27, 16 char, 2 line display

// pins
int ssBtnPin = 2;
int modeBtnPin = 4;
int motorPin = 8;
int pointsSwitchPin = 6;

// score
unsigned int score = 0;
unsigned int highScore[3] = {0,0,0}; // [continuous, timer, stop-go]
String routineNames[3] = {"CONTINUOUS", "TIMER", "STOP 'n GO"};

// run gallery
int mode = 0; // routine/game mode. 0 = continuous, 1 = timer, 2 = stop-go
int motorMode = LOW; // run motor
unsigned long routineTime = 10000;
unsigned long routineTimeStart = 0;

int ssBtnPreviousState = HIGH;
int modeBtnPreviousState = HIGH;
int pointsSwitchPreviousState = HIGH;

void updateScore(int routine, bool resetScore = false);
void stopGallery(bool forceStop = false);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(ssBtnPin, INPUT_PULLUP);
  pinMode(modeBtnPin, INPUT_PULLUP);

  pinMode(motorPin, OUTPUT);
  pinMode(pointsSwitchPin, INPUT_PULLUP);
}

void loop() {
  // mode button
  // TODO: convert to a method
  int buttonReading = digitalRead(modeBtnPin);

  // prime mode button
  if (buttonReading == LOW) {
    modeBtnPreviousState = LOW;
  } 

  // activate mode button
  else if (buttonReading == HIGH && modeBtnPreviousState == LOW) {
    modeBtnPreviousState = HIGH;

    if (routineTimeStart > 0) {
      stopGallery(true);
    }

    // cycle through modes
    mode++;
    if (mode == 3) {
      mode = 0;
    }

    // refresh displayed scores
    updateScore(mode, true);
    displayStats(mode);

    // display mode
    displayMode(mode);
  }

  // run the game/gallery
  else {
    // scoring
    updateScore(mode);

    // display stats
    displayStats(mode);

    // routines
    runGallery(mode);
  }
}

void updateScore(int routine, bool resetScore) {
  int pointsSwitchReading = digitalRead(pointsSwitchPin);

  // TODO: adjust button read when actual sensor/switch/button is determined
  if (pointsSwitchReading == LOW) {
    pointsSwitchPreviousState = LOW;
  } 
  else if (pointsSwitchReading == HIGH && pointsSwitchPreviousState == LOW) {
    pointsSwitchPreviousState = HIGH;

    // only update score while routine is running
    if (routineTimeStart > 0) {
      score++;
    }

    // update high score
    if (score > highScore[routine]) {
      highScore[routine] = score;
    }
  }

  if (resetScore) {
    score = 0;
  }
}

void displayMode(int routine) {
  lcd.setCursor(0,1);
  lcd.print("Mode:           ");
  lcd.setCursor(5,1);
  lcd.print(routineNames[mode]);
  delay(1000);
}

void displayStats(int routine) {
  // score
  lcd.setCursor(0,0); // col, row
  lcd.print("Score:          ");
  lcd.setCursor(6,0);
  // pad score (6 -> 06)
  if (score < 10) {
    lcd.print(0);
  }
  lcd.print(score);

  // high score
  lcd.setCursor(8,0);
  lcd.print(" High:");
  lcd.setCursor(14,0);
  // pad score (6 -> 06)
  if (highScore[routine] < 10) {
    lcd.print(0);
  }
  lcd.print(highScore[routine]);
}

void startGallery(int routine, unsigned long timeStart) {
  int ssBtnReading = digitalRead(ssBtnPin);

  // display "PRESS START"
  lcd.setCursor(0,1);
  lcd.print("PRESS START     ");

  // prime start button
  if (digitalRead(ssBtnPin) == LOW) {
    ssBtnPreviousState = LOW;
  } 
  // activate start button
  else if (digitalRead(ssBtnPin) == HIGH && ssBtnPreviousState == LOW) {
    ssBtnPreviousState = HIGH;
    routineTimeStart = timeStart;  

    updateScore(routine, true);

    digitalWrite(motorPin, HIGH);
    
    // display mode name
    lcd.setCursor(0,1);
    lcd.print(routineNames[routine]);
    lcd.print("                ");
  }
}

void stopGallery(bool forceStop) {
  int ssBtnReading = digitalRead(ssBtnPin);

  // prime stop button
  if (ssBtnReading == LOW) {
    ssBtnPreviousState = LOW;
  } 

  // activate stop button
  else if ((ssBtnReading == HIGH && ssBtnPreviousState == LOW) || forceStop) {
    ssBtnPreviousState = HIGH;
    routineTimeStart = 0;  

    digitalWrite(motorPin, LOW);

    // display "STOPPING"
    lcd.setCursor(0,1);
    lcd.print("STOPPING..      ");
    delay(1000);
  }
}

void runGallery(int routine) {
  switch (routine) {
    case 0: // continuous
      // stop
      if (routineTimeStart > 0) {
        stopGallery();
      }
      
      // start
      if (routineTimeStart == 0) {
        startGallery(routine, 1);
      }

      // run continuous
      else {
        digitalWrite(motorPin, HIGH);
      }

      break;

    case 1: // Beat the timer (play until timer runs out.)      
      // stop
      if (routineTimeStart > 0) {
        stopGallery();
      }

      // start
      if (routineTimeStart == 0) {
        routineTime = 10000; // length of game
        startGallery(routine, millis());
      } 

      // count down timer
      else if ((millis() - routineTimeStart) < routineTime) {
        // diplay time remaining
        lcd.setCursor(7,1);
        lcd.print((routineTime - (millis() - routineTimeStart)) / 1000);
      }

      // stop game / display end game screen / reset timer
      else if ((millis() - routineTimeStart) > routineTime) {
        routineTimeStart = 0;

        digitalWrite(motorPin, LOW);
      }

      break;

    case 2: // stop and go
      // stop
      if (routineTimeStart > 0) {
        stopGallery();
      }

      // start
      if (routineTimeStart == 0) {
        routineTime = 3000; // time between possible transitions
        startGallery(routine, millis());
      }

      // run routine
      else if ((millis() - routineTimeStart) > routineTime) {
        if (random(1,10) % 2 == 0) {
          motorMode = LOW;
          routineTime = 500;
        }
        else {
          motorMode = HIGH;
          routineTime = 1000;
        }

        routineTimeStart = millis();
        digitalWrite(motorPin, motorMode);
      }

      break;
  }  
}