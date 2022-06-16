#include <Adafruit_NeoPixel.h>

#define recordButton 2  // grey 
#define led1 4  // pink
#define led2 5  // green
#define led3 6  // blue
#define led4 7  // yellow
// #define motionSensor 3
#define touchButton1 3   // pink
#define touchButton2 9   // green
#define touchButton3 10  // blue
#define touchButton4 11  // yellow

//int motionVal = 0;
//int motionState = LOW;

String command;
int topicNumber = 0;
int countInterrupts = 0;

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(12, led1, NEO_GRB + NEO_KHZ800); // actually 16 for final version
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(12, led2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(12, led3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(10, led4, NEO_GRB + NEO_KHZ800);

unsigned long current_millis = 0;
unsigned long print_interval = 5000UL; //10 seconds

void setup() {
  pinMode(led1, OUTPUT);      
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  
//  pinMode(motionSensor, INPUT);    

  pinMode(touchButton1, INPUT);
  pinMode(touchButton2, INPUT);
  pinMode(touchButton3, INPUT);
  pinMode(touchButton4, INPUT);
  pinMode(recordButton, INPUT);
  
  Serial.begin(9600);  
  clearAllLed();
  delay(1000);
  ledWakeUp();
  topicNumber = 0;
  attachInterrupt(digitalPinToInterrupt(recordButton), recordMessage, RISING);

}

void loop() {
  
  if (topicNumber < 3) {
    topicNumber ++;
  } else {
    topicNumber = 1;
  }
  
  Serial.println("Topic" + String(topicNumber));
  delay(100);
  askQuestion();
  while (countInterrupts < 6 ) {
    waitForInteraction();
  }
  endSession();
  delay(3000);
}

// --- communication --- //
void askQuestion() {
  Serial.println("Ask question");
  delay(300);
  
  while (Serial.available() == 0) {
    rainbow();
  }
  command = Serial.readStringUntil('\n');
  command.trim();
  if (command.equals("question done"))  {
    playMessage2();
  } 
}

void recordMessage() {
  Serial.println("record");
  while (Serial.available() == 0) {
    clearAllLed();
  }
  countInterrupts ++;
  playMessage1();
}

void playMessage1() {
  Serial.println("voice1");
  //clearAllLed();
  while (Serial.available() == 0) {
    colorWipe(strip1, strip1.Color(255,   0,   255), 100);
  }
  command = Serial.readStringUntil('\n');
  countInterrupts ++;
  return;
}

void playMessage2() {
  delay(200);
  Serial.println("voice2");
  clearAllLed();
  while (Serial.available() == 0) {
    colorWipe(strip2, strip2.Color(0, 255,   0), 100);
  }
  command = Serial.readStringUntil('\n');
  countInterrupts ++;
  return;
}

void playMessage3() {
  Serial.println("voice3");
  clearAllLed();
  while (Serial.available() == 0) {
    colorWipe(strip3, strip3.Color(0,   0, 255), 100);
  }
  command = Serial.readStringUntil('\n');
  countInterrupts ++;
  return;
}

void playMessage4() {
  Serial.println("voice4");
  clearAllLed();
  while (Serial.available() == 0) {
    colorWipe(strip4, strip4.Color(0,   255, 255), 100);
  }
  command = Serial.readStringUntil('\n');
  countInterrupts ++;
  return;
}

void waitForInteraction() {
  breathingMagentaStrip1();
  delay(20);
  breathingGreenStrip2();
  delay(20);
  breathingBlueStrip3();
  delay(20);
  breathingCyanStrip4();
  delay(20);
}

void endSession() {
  clearAllLed();
  countInterrupts = 0;
  Serial.println("endsession");
  
}

// ---- LED compound functions --- //

void ledWakeUp() {
  colorWipe(strip1, strip1.Color(255,   0,   255), 150); // magenta
  delay(100);
  colorWipe(strip2, strip2.Color(0, 255,   0), 150); // Green
  delay(100);
  colorWipe(strip3, strip3.Color(0,   0, 255), 150); // Blue
  delay(100);
  colorWipe(strip4, strip4.Color(0,   255, 255), 150); //cyan
  delay(100);
}



// a question is being played
void rainbow() {
  // Hue of first pixel runs 4 complete loops through the color wheel.

  for(long firstPixelHue = 0; firstPixelHue < 1*65536; firstPixelHue += 256) {
    strip1.rainbow(firstPixelHue);
    strip2.rainbow(firstPixelHue);
    strip3.rainbow(firstPixelHue);
    strip4.rainbow(firstPixelHue);
    strip1.show(); // Update strip with new contents
    strip2.show();
    strip3.show();
    strip4.show();
    delay(4);  // Pause for a moment
  }
  clearAllLed();
}


void clearAllLed() {
  ledTurnOff(strip1);
  ledTurnOff(strip2);
  ledTurnOff(strip3);
  ledTurnOff(strip4);
}
  
// ---- LED basic animations --- //
void ledTurnOff(Adafruit_NeoPixel &strip) {
  strip.clear();
  strip.show();
  }

// led wakes up
void colorWipe(Adafruit_NeoPixel &strip, uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait); 
  }
  strip.clear();
  strip.show();   
}

// leds waiting for interaction 
void breathingMagentaStrip1() {
  uint16_t i, j;
  for (j = 0; j < 255; j++)
  {
    for (i = 0; i < strip1.numPixels(); i++)
    {
      strip1.setPixelColor(i, j, 0, j);
    }
    strip1.show();
    delay(4);
  }
  for (j = 255; j > 0; j--) {
    for (i = 0; i < strip1.numPixels(); i++) {
      strip1.setPixelColor(i, j, 0, j);
    }
    strip1.show();
    delay(4);
  }
}

void breathingGreenStrip2() {
  uint16_t i, j;
  for (j = 0; j < 255; j++)
  {
    for (i = 0; i < strip2.numPixels(); i++)
    {
      strip2.setPixelColor(i, 0, j, 0);
    }
    strip2.show();
    delay(4);
  }
  for (j = 255; j > 0; j--) {
    for (i = 0; i < strip2.numPixels(); i++) {
      strip2.setPixelColor(i, 0, j, 0);
    }
    strip2.show();
    delay(4);
  }
}

void breathingBlueStrip3() {
  uint16_t i, j;
  for (j = 0; j < 255; j++)
  {
    for (i = 0; i < strip3.numPixels(); i++)
    {
      strip3.setPixelColor(i, 0, 0, j);
    }
    strip3.show();
    delay(4);
  }
  for (j = 255; j > 0; j--) {
    for (i = 0; i < strip3.numPixels(); i++) {
      strip3.setPixelColor(i, 0, 0, j);
    }
    strip3.show();
    delay(4);
  }
}

void breathingCyanStrip4() {
  uint16_t i, j;
  for (j = 0; j < 255; j++)
  {
    for (i = 0; i < strip4.numPixels(); i++)
    {
      strip4.setPixelColor(i, 0, j, j);
    }
    strip4.show();
    delay(4);
  }
  for (j = 255; j > 0; j--) {
    for (i = 0; i < strip4.numPixels(); i++) {
      strip4.setPixelColor(i, 0, j, j);
    }
    strip4.show();
    delay(4);
  }
}
