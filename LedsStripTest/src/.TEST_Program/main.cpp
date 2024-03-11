#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi credentials
const char* ssid = "devbit";
const char* password = "Dr@@dloos!";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// LED strip details
#define PIN 3
#define NUM_LEDS 300

uint8_t brightness = 255;
uint8_t redValue = 0;
uint8_t greenValue = 0;
uint8_t blueValue = 0;
uint8_t whiteValue = 255;

// Button input pins
const int redInput = 4;
const int greenInput = 5;
const int blueInput = 2;

// Previous time in seconds
int prevTimeSec = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// Function prototypes
void testAllLEDs();
void displayButtonValues();
void displayBrightness();

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

  static bool prevRedButtonState = HIGH;
  static bool prevGreenButtonState = HIGH;
  static bool prevBlueButtonState = HIGH;

  static unsigned long buttonPressStartTime = 0;
  const unsigned long holdThreshold = 1000; // 1 second hold threshold
  // Increase the brightness on the rising edge if you press the red and green buttons at the same time
  if (digitalRead(redInput) == LOW && digitalRead(greenInput)== LOW) {
    brightness++;
    strip.setBrightness(brightness);
    displayBrightness();
  }

  // Check if the button is held for more than the hold threshold
  else if (digitalRead(redInput) == LOW) {
    if (prevRedButtonState == HIGH) { // Button was just pressed
      buttonPressStartTime = millis();
      redValue++;
      displayButtonValues();
    }
    if (millis() - buttonPressStartTime > holdThreshold) {
      redValue++;
      displayButtonValues();
    }
  }
  else if (digitalRead(greenInput) == LOW) {
    if (prevGreenButtonState == HIGH) { // Button was just pressed
      buttonPressStartTime = millis();
      greenValue++;
      displayButtonValues();
    }
    if (millis() - buttonPressStartTime > holdThreshold) {
      greenValue++;
      displayButtonValues();
    }
  } 
  else if (digitalRead(blueInput) == LOW) {
    if (prevBlueButtonState == HIGH) { // Button was just pressed
      buttonPressStartTime = millis();
      blueValue++;
      displayButtonValues();
    }
    if (millis() - buttonPressStartTime > holdThreshold) {
      blueValue++;
      displayButtonValues();
    }
  } 

  prevBlueButtonState = digitalRead(blueInput);
  prevRedButtonState = digitalRead(redInput);
  prevGreenButtonState = digitalRead(greenInput);

  // Delay for 100 milliseconds to debounce the buttons
  delay(0);
  // Update NTP client and get current time
  timeClient.update();
  // only update the time if the time is changed

  
testAllLEDs();
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
