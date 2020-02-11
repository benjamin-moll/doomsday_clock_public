//DOOMSDAY CLOCK//

//idea: set times during historical periods i.e cold war etc

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//current Doomsday Clock is "100 seconds to midnight" aka 23:58:20
int hours = 23;
int minutes = 58;
int seconds = 20;
long lastUpdate = 0;
//previous hours, minutes, seconds button states
//int lastHours, lastMinutes, lastSeconds;
volatile int lastSet;
int lastUp, lastDown, lastRocker;

volatile int setState;



int untilHours = 0;
int untilMinutes = 1;
int untilSeconds = 20;


const int downButton = 2;
const int upButton = 3;
const int setButton = 4;
const int rockerSwitch = 5;
int setcounter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(setButton, INPUT_PULLUP);
  pinMode(rockerSwitch, INPUT_PULLUP);
  digitalPinToInterrupt(setButton);
  attachInterrupt(setButton, blink, FALLING);
  int setcounter = 0;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  //clear display buffer
  display.clearDisplay();


  //Display print intro
  doomIntro();


}

void loop() {

  //constantly display current Doomsday readout until buttons touched

  //if current time is not midnight
  if (!(hours == 0 && minutes == 0 && seconds == 0)) {


    //trigger set mode on setButton push
    setState = digitalRead(setButton);
    if (setState != lastSet) {
      delay(10);
      if (setState == LOW) {
        setcounter++;
      }
    }
    lastSet = setState;

    //default state, clock readout
    if (setcounter == 0) {
      displayDoomTime();
    }
    //setting Hours on one Set press
    else if (setcounter == 1) {
      displaySetHour();
    }
    //setting Minutes on two Set presses
    else if (setcounter == 2) {
      displaySetMinute();
    }
    //setting Seconds on three Set presses
    else if (setcounter == 3) {
      displaySetSecond();
    }
    //reset back to default state on four Set presses
    else if (setcounter == 4) {
      displayDoomTime();
      setcounter == 0;
    }


  }

  //midnight warning
  else if (hours == 0 && minutes == 0 && seconds == 0) {
    midnightRocker();
    
  }


}

void blink() {
  setState = !setState;
}

void midnightRocker(){
   display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Midnight\nreached!\n");
    display.display();
    delay(3000);
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.print("Are you sure?\n");
    display.display();
    delay(4000);

    int currentRocker = digitalRead(rockerSwitch);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
//    if (currentRocker != lastRocker) {
      if(currentRocker == LOW){
      display.print("Midnight\nconfirmed Initiating shutdown");
      display.display();
      display.clearDisplay();
      delay(2000);
      fillcircle();
      display.display();
      }
//      }
     
    
    
}

void doomIntro() {
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  //display.println(F("The Doomsday Clock is a design that warns the public about how close we are to destroying our world with dangerous technologies of our own making."));
  display.println(F("The Doomsday Clock\nis a design that"));
  display.display();
  delay(4000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("warns the public about how close we are");
  display.display();
  delay(4000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("to destroying our world with\ndangerous");
  display.display();
  delay(4000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("technologies of our own making");
  display.display();
  delay(4000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("The\ncurrent\ntime\nof the");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("official\nDoomsday\nClock is:\n");


  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print(hours);
  display.print(":");
  display.print(minutes);
  display.print(":");
  display.print(seconds);
  display.print("\n");
  display.setTextSize(1);
  display.display();
  delay(5000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("This means we are \n");
  display.setTextSize(2);
  display.print("100\nseconds");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.setTextSize(2);
  display.print("until midnight, or\nglobal fallout.");
  display.display();
  delay(5000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("Set your\nDoomsday\ntime...");
  display.print("IF YOU DARE");
  display.display();
  delay(5000);


}

void displayDoomTime() {
  setcounter = 0;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 0);
  display.print("Doomsday Clock: \n");

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 35);
  if (hours < 10) display.print("0");
  display.print(hours);
  display.print(":");
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);
  display.print("\n");
  display.setTextSize(1);
  display.display();

}

void displaySetHour() {
  int currentUp = digitalRead(upButton);
  int currentDown = digitalRead(downButton);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 0);
  display.print("Setting\n Hours: ");
  if (currentUp != lastUp) {
    delay(10);

    if (digitalRead(upButton) == LOW) {
      hours++;
    }
  }
  lastUp = currentUp;
  if (currentDown != lastDown) {
    delay(10);

    if (digitalRead(downButton) == LOW) {
      hours--;
    }
  }
  lastDown = currentDown;
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 35);
  if (hours < 10) display.print("0");
  display.print(hours);
  display.print(":");
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);
  display.print("\n");
  display.display();

  if (seconds > 59) {   // rollover seconds and update minutes
    seconds = 0;
    minutes++;
  }
  if (seconds < 0) {
    seconds = 59;
    minutes--;
  }
  if (minutes > 59) {   // rollover minutes and update hours
    minutes = 0;
    hours++;
  }
  if (minutes < 0) {
    minutes = 59;
    hours--;
  }
  if (hours > 23) {     // rollover hours
    hours = 0;
  }
  if (hours < 0) {
    hours = 23;
  }

}

void displaySetMinute() {
  int currentUp = digitalRead(upButton);
  int currentDown = digitalRead(downButton);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 0);
  display.print("Setting\nMinutes: ");
  if (currentUp != lastUp) {
    delay(10);

    if (digitalRead(upButton) == LOW) {
      minutes++;
    }

  }
  lastUp = currentUp;
  if (currentDown != lastDown) {
    delay(10);

    if (digitalRead(downButton) == LOW) {
      minutes--;
    }
  }
  lastDown = currentDown;

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 35);
  if (hours < 10) display.print("0");
  display.print(hours);
  display.print(":");
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);
  display.print("\n");
  display.display();

  if (seconds > 59) {   // rollover seconds and update minutes
    seconds = 0;
    minutes++;
  }
  if (seconds < 0) {
    seconds = 59;
    minutes--;
  }
  if (minutes > 59) {   // rollover minutes and update hours
    minutes = 0;
    hours++;
  }
  if (minutes < 0) {
    minutes = 59;
    hours--;
  }
  if (hours > 23) {     // rollover hours
    hours = 0;
  }
  if (hours < 0) {
    hours = 23;
  }

}

void displaySetSecond() {
  int currentUp = digitalRead(upButton);
  int currentDown = digitalRead(downButton);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 0);
  display.print("Setting\nSeconds: ");
  if (currentUp != lastUp) {
    delay(10);

    if (digitalRead(upButton) == LOW) {
      seconds++;
    }

  }
  lastUp = currentUp;
  if (currentDown != lastDown) {
    delay(10);

    if (digitalRead(downButton) == LOW) {
      seconds--;
    }
  }
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15, 35);
  if (hours < 10) display.print("0");
  display.print(hours);
  display.print(":");
  if (minutes < 10) display.print("0");
  display.print(minutes);
  display.print(":");
  if (seconds < 10) display.print("0");
  display.print(seconds);
  display.print("\n");
  display.display();

  if (seconds > 59) {   // rollover seconds and update minutes
    seconds = 0;
    minutes++;
  }
  if (seconds < 0) {
    seconds = 59;
    minutes--;
  }
  if (minutes > 59) {   // rollover minutes and update hours
    minutes = 0;
    hours++;
  }
  if (minutes < 0) {
    minutes = 59;
    hours--;
  }
  if (hours > 23) {     // rollover hours
    hours = 0;
  }
  if (hours < 0) {
    hours = 23;
  }


}



void fillcircle(void) {
  display.clearDisplay();

  for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}
