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
#define NUM_LEDS 31
uint8_t brightness = 255;
uint8_t redValue = 255;
uint8_t greenValue = 0;
uint8_t blueValue = 0;

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
  // Update NTP client and get current time
  timeClient.update();
  // only update the time if the time is changed

  if (timeClient.getSeconds() != prevTimeSec)
  {
    prevTimeSec = timeClient.getSeconds();
    Serial.print("Current time: ");
    Serial.println(timeClient.getFormattedTime());
  }
  // dispaly the time on the led strip
  // We want the time to be displayed in binary in the following format
  // The first 4 leds will display the seconds units in binary and then a gap of 1 led and then the seconds tens in binary on the next 4 leds
  // The next 4 leds will display the minutes units in binary and then a gap of 1 led and then the minutes tens in binary on the next 4 leds with a gap of 1 led between the seconds and minutes  and then the hours units in binary on the next 4 leds and then a gap of 1 led and then the hours tens in binary on the next 4 leds

  // get the current time
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  // convert the time to binary
  int secondsUnits = seconds % 10;
  int secondsTens = seconds / 10;
  int minutesUnits = minutes % 10;
  int minutesTens = minutes / 10;
  int hoursUnits = hours % 10;
  int hoursTens = hours / 10;

  // display the time on the led strip
  for (int i = 0; i < 4; i++) {
    if (secondsUnits & (1 << i)) {
      strip.setPixelColor(i, strip.Color(redValue, greenValue, blueValue, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  for (int i = 5; i < 9; i++) {
    if (secondsTens & (1 << (i - 5))) {
      strip.setPixelColor(i, strip.Color(redValue, greenValue, blueValue, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  for (int i = 10; i < 14; i++) {
    if (minutesUnits & (1 << (i - 10))) {
      strip.setPixelColor(i, strip.Color(blueValue, redValue, greenValue, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  for (int i = 15; i < 19; i++) {
    if (minutesTens & (1 << (i - 15))) {
      strip.setPixelColor(i, strip.Color(blueValue, redValue, greenValue, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  for (int i = 20; i < 24; i++) {
    if (hoursUnits & (1 << (i - 20))) {
      strip.setPixelColor(i, strip.Color(greenValue, blueValue, redValue, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  for (int i = 25; i < 29; i++) {
    if (hoursTens & (1 << (i - 25))) {
      strip.setPixelColor(i, strip.Color(greenValue, blueValue, redValue, 0));
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
  }
  strip.show(); // Update all LEDs with the new colors




  // Test all LEDs with the current RGB values
  //testAllLEDs();

  // Increase the RGB values while the corresponding button is held down
  if (digitalRead(redInput) == LOW && digitalRead(greenInput) == LOW ) {
    brightness++ ;
    strip.setBrightness(brightness);
    displayBrightness();
  }
  else if (digitalRead(redInput) == LOW) {
    redValue++;
    displayButtonValues();
  }
  else if (digitalRead(greenInput) == LOW) {
    greenValue++;
    displayButtonValues();
  }
  else if (digitalRead(blueInput) == LOW) {
    blueValue++;
    displayButtonValues();
  }

  // Delay for 100 milliseconds to debounce the buttons
  delay(0);
}

void testAllLEDs() {
  // Loop through all LEDs and set them to different colors
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t color = strip.Color(redValue, greenValue, blueValue, 0);
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