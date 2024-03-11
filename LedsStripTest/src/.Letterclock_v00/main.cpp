#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arrays.h>

// WiFi credentials
const char* ssid = "devbit";
const char* password = "Dr@@dloos!";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// LED strip details
#define PIN 3
#define NUM_LEDS 600

int outputArray[NUM_LEDS] = {0};


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

// Previous time in seconds
int prevTimeSec = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// Function prototypes
void testAllLEDs();
void displayButtonValues();
void displayBrightness();
void color_brightnessManipulation();
void buildOutputArray(int hours, int minutes);
void displayOutputArray();

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
  timeClient.setTimeOffset(3600); // Timezone offset for Brussels (UTC+1)

  strip.begin(); // Initialize the NeoPixel strip
  strip.show(); // Update all LEDs with the new colors
  strip.setBrightness(brightness); // Set the brightness to 50%
  
  pinMode(redInput, INPUT_PULLUP);   // Set the red input pin as input
  pinMode(greenInput, INPUT_PULLUP); // Set the green input pin as input
  pinMode(blueInput, INPUT_PULLUP);  // Set the blue input pin as input
}

void loop() {
  // Update NTP client and get current time
  timeClient.update();
  // only update the time if the time is changed

  if (timeClient.getSeconds() != prevTimeSec)
  {
    prevTimeSec = timeClient.getSeconds();
    Serial.print("Current time: ");
    Serial.println(timeClient.getFormattedTime());
    // Serial.println(digitalRead(redInput));
    // Serial.println(digitalRead(greenInput)); 
    // Serial.println(digitalRead(blueInput));
  }
  // get the current time
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  //every 5 minutes
  if (minutes % 5 == 0)
  {
    if (flag == 0)
    {
      flag = 1;
      strip.clear();

    }

    // build the output array
    buildOutputArray(hours, minutes);
    // display the output array
    displayOutputArray();
    // change color and brightness
  }
  else {
    flag = 0;
  }
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
  }

  else if (millis() - dubblePressReleaseTime > 200) {
    // Check if the button is held for more than the hold threshold
      if (prevRedButtonState == LOW && digitalRead(redInput) == HIGH) { // Button was just pressed
        redValue++;
        displayButtonValues();
      }
      if (digitalRead(redInput) == LOW ) {
        if (prevRedButtonState == HIGH) { 
          buttonPressStartTime = millis();
        }
        if (millis() - buttonPressStartTime > holdThreshold) {
          redValue++;
          displayButtonValues();
        }
    }

    else if (prevGreenButtonState == LOW && digitalRead(greenInput) == HIGH) { // Button was just pressed
      greenValue++;
      displayButtonValues();
    }
    if (digitalRead(greenInput) == LOW) {
      if (prevGreenButtonState == HIGH) { 
        buttonPressStartTime = millis();
      }
      if (millis() - buttonPressStartTime > holdThreshold) {
        greenValue++;
        displayButtonValues();
      }
    } 

    else if (prevBlueButtonState == LOW && digitalRead(blueInput) == HIGH) { // Button was just pressed
      blueValue++;
      displayButtonValues();
    }
    if (digitalRead(blueInput) == LOW) {
      if (prevBlueButtonState == HIGH) { 
        buttonPressStartTime = millis();
      }
      if (millis() - buttonPressStartTime > holdThreshold) {
        blueValue++;
        displayButtonValues();
      }
    }
  }

  prevBlueButtonState = digitalRead(blueInput);
  prevRedButtonState = digitalRead(redInput);
  prevGreenButtonState = digitalRead(greenInput);

  // Delay for 100 milliseconds to debounce the buttons
  delay(10);
}

//function for building the output array
void buildOutputArray(int hours, int minutes) {
  // clear the output array
  for(int i = 0; i < NUM_LEDS; i++) {
    outputArray[i] = 0;
  }
  if (hours > 12)
  {
    hours = hours - 12;
  } 
  if (minutes >= 20)
  {
    hours++;
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
    else
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }

  strip.show();

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
