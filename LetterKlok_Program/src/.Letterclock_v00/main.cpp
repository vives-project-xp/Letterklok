#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arrays.h>
#include <esp32-hal-timer.h>

// WiFi credentials
const char* ssid = "devbit";
const char* password = "Dr@@dloos!";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// LED strip details
#define PIN 3
#define NUM_LEDS 600

boolean outputArray[NUM_LEDS] = {0};


uint8_t brightness = 255;
uint8_t redValue = 255;
uint8_t greenValue = 0;
uint8_t blueValue = 0;
uint8_t whiteValue = 0;

// Button input pins
const int redInput = 4;
const int greenInput = 5;
const int blueInput = 2;
bool flag = 0;
bool colorFlag = 0;

// Previous time in seconds
int prevTimeSec = 0;

//time variables
int hours = 0;
int minutes = 0;
int seconds = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// Define the timer number (0-3 for ESP32)
#define TIMER_NUMBER 0

// Define the timer interval in microseconds
#define TIMER_INTERVAL_US 1000000 // 1 second

// Function prototypes
void testAllLEDs();
void displayButtonValues();
void displayBrightness();
void color_brightnessManipulation();
void buildOutputArray(int hours, int minutes);
void displayOutputArray();
void startUpProtocol();
void timerSetup();
void IRAM_ATTR onTimer();

void setup() {
  Serial.begin(9600); // Initialize the serial port
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();
  timeClient.setTimeOffset(7200); // Timezone offset for Brussels (UTC+1)

  strip.begin(); // Initialize the NeoPixel strip
  strip.show(); // Update all LEDs with the new colors
  strip.setBrightness(brightness); // Set the brightness to 50%
  
  pinMode(redInput, INPUT_PULLUP);   // Set the red input pin as input
  pinMode(greenInput, INPUT_PULLUP); // Set the green input pin as input
  pinMode(blueInput, INPUT_PULLUP);  // Set the blue input pin as input

  // show the current time on the clock
  startUpProtocol();
  timerSetup();
}

void loop() {
  //every 5 minutes
  if (minutes % 5 == 0)
  {
    if (flag == 0)
    {

      flag = 1;
      strip.clear();
      strip.show();
      // build the output array
      buildOutputArray(hours, minutes);
      // display the output array
      displayOutputArray();
      // change color and brightness
    }
  }
  else {
    flag = 0;
  }
  /*
for (int j = 0; j <=55  ; j+=5)
    {
      buildOutputArray(1, j);
      displayOutputArray();
      delay(2500);
    } 
  for(int i = 2; i <= 12; i++) {
    buildOutputArray(i, 0);
      displayOutputArray();
      delay(2500);
  }*/
  color_brightnessManipulation();
  strip.show();
  // testAllLEDs();

}

void color_brightnessManipulation(){
    
  static bool prevRedButtonState = HIGH;
  static bool prevGreenButtonState = HIGH;
  static bool prevBlueButtonState = HIGH;

  static unsigned long buttonPressStartTime = 0;
  static unsigned long dubblePressReleaseTime = 0;
  const unsigned long holdThreshold = 1000; // 1 second hold threshold
  // Increase the brightness on the rising edge if you press the red and green buttons at the same time
  if (digitalRead(redInput) == LOW && digitalRead(greenInput)== LOW) {
    brightness++;
    strip.setBrightness(brightness);
    displayBrightness();
    dubblePressReleaseTime = millis();
    colorFlag = 1;
  }

  else if (millis() - dubblePressReleaseTime > 200) {
    // Check if the button is held for more than the hold threshold
      if (prevRedButtonState == LOW && digitalRead(redInput) == HIGH) { // Button was just pressed
        redValue++;
        displayButtonValues();
        colorFlag = 1;
      }
      if (digitalRead(redInput) == LOW ) {
        if (prevRedButtonState == HIGH) { 
          buttonPressStartTime = millis();
        }
        if (millis() - buttonPressStartTime > holdThreshold) {
          redValue++;
          displayButtonValues();
          colorFlag = 1;
        }
    }

    else if (prevGreenButtonState == LOW && digitalRead(greenInput) == HIGH) { // Button was just pressed
      greenValue++;
      displayButtonValues();
      colorFlag = 1;
    }
    if (digitalRead(greenInput) == LOW) {
      if (prevGreenButtonState == HIGH) { 
        buttonPressStartTime = millis();
      }
      if (millis() - buttonPressStartTime > holdThreshold) {
        greenValue++;
        displayButtonValues();
        colorFlag = 1;
      }
    } 

    else if (prevBlueButtonState == LOW && digitalRead(blueInput) == HIGH) { // Button was just pressed
      blueValue++;
      displayButtonValues();
      colorFlag = 1;
    }
    if (digitalRead(blueInput) == LOW) {
      if (prevBlueButtonState == HIGH) { 
        buttonPressStartTime = millis();
      }
      if (millis() - buttonPressStartTime > holdThreshold) {
        blueValue++;
        displayButtonValues();
        colorFlag = 1;
      }
    }
  }
  

  if(redValue >= 128 && greenValue >= 128 && blueValue >= 128) 
  {
    if(abs((redValue - blueValue)) <= 5 && abs((redValue - greenValue)) <= 5 && abs((greenValue - blueValue)) <= 5)
    {
      whiteValue = redValue;
      redValue = 0;
      greenValue = 0;
      blueValue = 0;
      colorFlag = 1;
    }
  }

  prevBlueButtonState = digitalRead(blueInput);
  prevRedButtonState = digitalRead(redInput);
  prevGreenButtonState = digitalRead(greenInput);

  // Delay for 100 milliseconds to debounce the buttons
  delay(10);

  if (colorFlag == 1)
  {
    colorFlag = 0;
    displayOutputArray();
    strip.show();
  }
}

//function for building the output array
void buildOutputArray(int hours, int minutes) {
  // clear the output array
  for(int i = 0; i < NUM_LEDS; i++) {
    outputArray[i] = 0;
  }

  if (minutes >= 20)
  {
    hours++;
  }
  if (hours > 12)
  {
    hours = hours - 12;
  } 
  if (hours == 0)
  {
    hours = 12;
  }
  
  // convert hours to output array
  if (hours == 1) for(int i : one) outputArray[i] = 1;
  else if (hours == 2) for(int i : two) outputArray[i] = 1;
  else if (hours == 3) for(int i : three) outputArray[i] = 1;
  else if (hours == 4) for(int i : four) outputArray[i] = 1;
  else if (hours == 5) for(int i : five) outputArray[i] = 1;
  else if (hours == 6) for(int i : six) outputArray[i] = 1;
  else if (hours == 7) for(int i : seven) outputArray[i] = 1;
  else if (hours == 8) for(int i : eight) outputArray[i] = 1;
  else if (hours == 9) for(int i : nine) outputArray[i] = 1;
  else if (hours == 10) for(int i : ten) outputArray[i] = 1;
  else if (hours == 11) for(int i : eleven) outputArray[i] = 1;
  else if (hours == 12) for(int i : twelve) outputArray[i] = 1;

  // convert minutes to output array
  if (minutes == 5) for(int i : fiveMinutes) outputArray[i] = 1;
  else if (minutes == 10) for(int i : tenMinutes) outputArray[i] = 1;
  else if (minutes == 15) for(int i : fifteenMinutes) outputArray[i] = 1;
  else if (minutes == 20) for(int i : twentyMinutes) outputArray[i] = 1;
  else if (minutes == 25) for(int i : twentyFiveMinutes) outputArray[i] = 1;
  else if (minutes == 30) for(int i : thirtyMinutes) outputArray[i] = 1;
  else if (minutes == 35) for(int i : thirtyFiveMinutes) outputArray[i] = 1;
  else if (minutes == 40) for(int i : fortyMinutes) outputArray[i] = 1;
  else if (minutes == 45) for(int i : fortyFiveMinutes) outputArray[i] = 1;
  else if (minutes == 50) for(int i : fiftyMinutes) outputArray[i] = 1;
  else if (minutes == 55) for(int i : fiftyFiveMinutes) outputArray[i] = 1;
  else if (minutes == 0) for(int i : sixtyMinutes) outputArray[i] = 1;
}

void displayOutputArray()
{
  Serial.println("Displaying output array:");
  for(int i = 0; i < NUM_LEDS; i++)
  {
    if (outputArray[i] == 1)
    {
      strip.setPixelColor(i, strip.Color(redValue, greenValue, blueValue, whiteValue));
    }
    else if (outputArray[i] == 0)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
strip.show();
  
/*
  for(int i = 0; i < NUM_LEDS; i++)
  {
    if ((i-2) % 44 == 0)
    {
      Serial.println();
    }
    if ((i-2) % 4 == 0)
    {
      Serial.print(" ");
    }
    Serial.print(outputArray[i]);
  }
  Serial.println("\n ");

   for(int i = 0; i < NUM_LEDS; i++)
  {
    Serial.println(strip.getPixelColor(i));
  }
  Serial.println("\n ");
  */
}


void testAllLEDs() {
  // Loop through all LEDs and set them to different colors
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t color = strip.Color(redValue, greenValue, blueValue, whiteValue);
    strip.setPixelColor(i, color);
  }
  
  strip.show(); // Update all LEDs with the new colors
}

void displayButtonValues() {
  Serial.print("Red: ");
  Serial.print(redValue);
  Serial.print(", Green: ");
  Serial.print(greenValue);
  Serial.print(", Blue: ");
  Serial.println(blueValue);
}

void displayBrightness() {
  Serial.print("Brightness: ");
  Serial.println(brightness);
}


void startUpProtocol() {
  timeClient.update();
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  minutes = minutes - (minutes % 5);
  buildOutputArray(hours, minutes);
  displayOutputArray();
  Serial.println("Start up protocol done");
}

void timerSetup() {
  // Initialize Timer 0 with a divider of 80 and count up
  hw_timer_t *timer = timerBegin(TIMER_NUMBER, 80, true);
  
  // Set an ISR (Interrupt Service Routine) to be called when the timer fires
  timerAttachInterrupt(timer, &onTimer, true);
  
  // Set the timer's alarm value (when it should trigger)
  timerAlarmWrite(timer, TIMER_INTERVAL_US, true); // 1 second (1,000,000 microseconds)

  //set the start time 
  timeClient.update();
  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
  seconds = timeClient.getSeconds();

  // Start the timer
  timerAlarmEnable(timer);
}

void IRAM_ATTR onTimer()
{
  seconds++;
  if (seconds >= 60)
  {
    seconds = 0;
    minutes++;
  }
  if (minutes >= 60)
  {
    minutes = 0;
    hours++;
  }
  if (hours >= 24)
  {
    hours = 0;
  }
  //print formatted time
  Serial.print("Current Time: ");
  Serial.printf("%02d:%02d:%02d\n", hours, minutes, seconds);
}