#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Initialize the LCD and set it to resolution 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad configuration
const byte ROW_NUM = 4; // Number of rows
const byte COLUMN_NUM = 4; // Number of columns

// This matrix binds the keys to their locations
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Setting the pin #'s for each keypad row and column.
byte pin_rows[ROW_NUM] = {2, 3, 4, 5};
byte pin_columns[COLUMN_NUM] = {6, 7, 8, 9};

// Create the keypad object which actually initializes the keypad
Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_columns, ROW_NUM, COLUMN_NUM);

// Code to unlock the electromagnet
// Must inlude # at the end or the code will not work.
const String correctCode = "5050#";
String enteredCode = "";

// Setting the pins for other functions
const int magnetPin = 10; // Gate solenoid
const int redLED = 11;    // LED for locked indicator
const int greenLED = 12;  // LED for unlocked indicator
const int buzzerPin = 13; // Buzzer for chimes


// Setup loop
void setup() {
  lcd.init();               // start LCD
  lcd.backlight();          // turn on backlight
  lcd.setCursor(0, 0);      // Move cursor
  lcd.print("Enter code:"); // Add text to the display

  pinMode(magnetPin, OUTPUT);   // Set solenoid pin to OUTPUT
  digitalWrite(magnetPin, LOW); // Disabled pin to 0

  pinMode(redLED, OUTPUT);    // Set red LED to OUTPUT
  pinMode(greenLED, OUTPUT);  // set green LED to OUTPUT
  pinMode(buzzerPin, OUTPUT); // set buzzer to OUTPUT

  digitalWrite(redLED, HIGH); // turn red LED on to indicate locked
  digitalWrite(greenLED, LOW);// set the other LED to OFF
  
  
  Serial.begin(9600); // starts serial monitor for debugging purposes and sets speed to 9600
}

void loop() {
  char key = keypad.getKey(); // sets var key to whatever key on the keypad is pressed

  if (key) {
    // Short keypress chime
    tone(buzzerPin, 800, 100); // Play 800 Hz tone for 100 ms
    delay(150);                // delay to prevent tones from overlaping


    Serial.print("Key pressed: ");  // Serial monitor output
    Serial.println(key);

    // Logic for checking for key presses
    if ((key >= '0' && key <= '9') || key == 'A' || key == 'B' || key == 'C' || key == 'D' || key == '*' || key == '#') {
      enteredCode += key; // adds whatever number is selected to the current code string
      lcd.setCursor(0, 1); // moves the cursor to the next spot on the LCD
      lcd.print(enteredCode); // prints the code currently entered
      if (enteredCode.length() > 16) {
        enteredCode = enteredCode.substring(1);   // Moves the string so that it fits on the LCD if the code is too long
      }
    }

    // Logic for validating the code, code is considered entered when the # key is entered
    if (key == '#') { 
      lcd.clear(); 

      // Instructions for when the enteredCode is correct:
      if (enteredCode == correctCode) {
        lcd.print("Code Correct!");
        Serial.println("Correct code entered!");
        
        // unlocks the door door
        digitalWrite(magnetPin, HIGH); // Sets solenoid pin to 1

        // switches LED
        digitalWrite(redLED, LOW); // turns red LED off
        digitalWrite(greenLED, HIGH); // turns green LED on to indicate the door is unlocked

        // Buzzer chime for unlock
        tone(buzzerPin, 1000, 2000); // Play 1 kHz tone for 2 seconds
        delay(2000);

        // Return to locked
        digitalWrite(magnetPin, LOW); // locks solenoid
        digitalWrite(redLED, HIGH);   // turns red LED back on
        digitalWrite(greenLED, LOW);  // turns green LED off

        // Instructions for when the enteredCode is not correct:

      } else {
        lcd.print("Incorrect code");    // writes to the LCD
        Serial.println("Incorrect code. Try again.");

        // Buzzer rapid chime and LED blink
        for (int i = 0; i < 5; i++) { // for loop to repeat 5 times
          digitalWrite(redLED, LOW);
          tone(buzzerPin, 1000, 200); // Short beep
          delay(200);
          digitalWrite(redLED, HIGH);
          delay(200);
        }
      }
          // resets the display for new attempt
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter code:");
      enteredCode = "";
    }

    // If the * key is pressed, it erases the enteredCode and starts the input again

    if (key == '*') {
      enteredCode = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter code:");
      Serial.println("Code cleared.");
    }
  }
}
