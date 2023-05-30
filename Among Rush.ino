// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int delayTime = 50;
int deadMan;
int button_A = 8;
int button_B = 7;
char button_A_state;
char button_B_state;
int aliveMan = 17;
int theDeadMan = 5;
int yMan = 0;
int previous_B_state;
int score;
int stop;
  
int me_angry_count = 5;


// make some custom characters:
byte man[8] = {
  0b000000,
  0b011110,
  0b001111,
  0b001111,
  0b011111,
  0b011110,
  0b010010,
  0b010010
};

byte rightLegMan[8] = {
  0b000000,
  0b011110,
  0b001111,
  0b001111,
  0b011111,
  0b011110,
  0b010010,
  0b010001
};

byte leftLegMan[8] = {
  0b000000,
  0b011110,
  0b001111,
  0b001111,
  0b011111,
  0b011110,
  0b010010,
  0b001010
};

byte deadManLegs[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11110,
  0b11110,
  0b10010,
  0b10010
};

byte deadManGround[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11101,
  0b01111,
  0b01111
};

byte empty[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  
  pinMode(A1, OUTPUT);
  pinMode(button_A, INPUT_PULLUP);
  pinMode(button_B, INPUT_PULLUP);
  
  digitalWrite(A1, LOW);
  
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);

  // create a new character for still man
  lcd.createChar(1, man);
  // create a new character for walking right leg man
  lcd.createChar(2, rightLegMan);
  // create a new character for walking left man
  lcd.createChar(3, leftLegMan);
  // create a new character for dead man thats upside-down and only has legs left ):
  lcd.createChar(4, deadManLegs);
  // create a new character for dead man thats lying on the ground
  lcd.createChar(5, deadManGround);
  // create a new empty charachter
  lcd.createChar(6, empty);

  // Print a message and amogusen to the lcd .
  lcd.setCursor(1, 0);
  lcd.write((byte)1);
  lcd.print("!AMONG-RUSH!");
  lcd.write((byte)1);
  
  while (digitalRead(button_A) == 1) {
    
    aliveMan -= 1;
    
  	if (aliveMan == -1) {
    aliveMan = 17;
  	}
    
    else {
    	lcd.setCursor(aliveMan, 1);
  		lcd.write(2);
    	delay(100);
  		lcd.setCursor(aliveMan , 1);
  		lcd.write(3);
    	delay(100);
     	lcd.setCursor(aliveMan, 1);
  		lcd.write(6);
    }
  }
  lcd.clear();
}

void(* resetFunc) (void) = 0;

void loop() {
  
  //reads for input of the buttons
  button_A_state = digitalRead(button_A); 
  button_B_state = digitalRead(button_B);
  
  if (button_A_state == 0) {
    lcd.setCursor(0, 1);
    lcd.print("A");
    stop += 1;
    if (stop > 4) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("restarting......");
      delay(1500);
      resetFunc();
    }
  }
  else {
        lcd.setCursor(0, 1);
    	lcd.print(" ");
    	stop = 0;
  }

  if (button_B_state == 0) {
    lcd.setCursor(1, 1);
    lcd.print("B");
    if (previous_B_state < 2) {
    	yMan = 0;
      	lcd.setCursor(8, 1);
    	lcd.write(6);
      	previous_B_state += 1;
    }
    else {
      yMan = 1;
    }
  	lcd.setCursor(8, 0);
    lcd.write(6);
  }
  
   else {
      	lcd.setCursor(1, 1);
    	lcd.print(" ");
     	yMan = 1;
     	lcd.setCursor(8, 0);
        lcd.write(6);
     	previous_B_state -= 1;
  }
  
    
  // draw right leg frame
  lcd.setCursor(8, yMan);
  lcd.write(2);
  delay(delayTime);

  // draw left leg frame
  lcd.setCursor(8, yMan);
  lcd.write(3);
  delay(delayTime);
  
  //moves deadman to 0 again
  deadMan += 1;
  
  if (deadMan == 17) {
    deadMan = 0;
    theDeadMan = random(4, 6);
    score += 1;
  }
  
  // moves deadman
  lcd.setCursor(deadMan, 1);
  lcd.write(theDeadMan);
  lcd.setCursor(deadMan - 1, 1);
  lcd.write(6);
  
  //prints deadMan count and clears the old count
  lcd.setCursor(13,0);
  lcd.write(6);
  lcd.setCursor(14,0);
  lcd.write(6);
  lcd.setCursor(15,0);
  lcd.write(6);
  lcd.setCursor(13, 0);
  lcd.print(score);
  
  // looks if there is collision between man and dead man
  // if so ends the game and restarts
  if (deadMan == 8) {
    if (yMan == 1) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("you are killed");
      lcd.setCursor(1, 1);
      lcd.print("you scored");
      lcd.setCursor(13, 1);
      lcd.print(score);
      delay(2000);
      resetFunc();
    }
  }
}
