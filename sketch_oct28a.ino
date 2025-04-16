#define BLYNK_TEMPLATE_ID "TMPL61SB4nDcC"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN       "XY9Axc_9864lcuWtLkezTtqeBxQUdI2a"
#include <Wire.h>
#include <DS3231.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// Replace with your network credentials
const char* ssid = "Hello";
const char* password = "motngaybennhau";
char auth[] = "XY9Axc_9864lcuWtLkezTtqeBxQUdI2a";


// Define the I2C address and size for your LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the pins for SDA, SCL, Relay, and Soil Moisture Sensor
#define SDA_PIN 4 // GPIO 4 (D2)
#define SCL_PIN 5 // GPIO 5 (D1)
#define RELAY_PIN 0 // GPIO 0 (D3)
#define SOIL_SENSOR_PIN A0 // Analog pin for soil moisture sensor

DS3231 rtc;


unsigned long previousMillis; // Stores the last time the pump was turned on
int interval = 10000; // 10s interval (in milliseconds)
bool automaticmode = false;
bool manualmode = false;
bool sensormode = false;

BlynkTimer timer;

void waterplant1() {
  {
  digitalWrite(RELAY_PIN, HIGH); // Turn on the water pump
  delay(5000); // Water for 10 seconds
  digitalWrite(RELAY_PIN, LOW); // Turn off the water pump
  }
}


void waterplant2()
{
  digitalWrite(RELAY_PIN,HIGH);
}
void plant3()
{
  digitalWrite(RELAY_PIN,HIGH);
}
BLYNK_WRITE(V1) 
{ // Mode switch on virtual pin V1
  automaticmode = param.asInt(); // 1 for automatic, 0 for set mode
}

BLYNK_WRITE(V2) 
{ // Manual watering button on virtual pin V2
manualmode=param.asInt();
}
BLYNK_WRITE(V3)
{
  sensormode=param.asInt();
}
void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); // Start I2C bus with defined pins
  
  lcd.begin(16, 2);
  lcd.backlight();
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure relay is off initially

  Blynk.begin(auth, ssid, password);

  timer.setInterval(1000L, []() { Blynk.run(); });

  Serial.println("Watering System with Blynk has started!");
}

void loop() {
  timer.run();
digitalWrite(RELAY_PIN, LOW); //set relay @ low
  bool h12, PM_time;
    int currentHour = rtc.getHour(h12, PM_time); // Get the current hour (24-hour format)
  int currentMinute = rtc.getMinute();
  int currentSecond = rtc.getSecond();
  unsigned long currentMillis = millis();
   

// Check if it is time to water and within the watering hours
  if (sensormode==1 && manualmode==1){ digitalWrite(RELAY_PIN, LOW);}
  else if (manualmode==1 && automaticmode==1){ digitalWrite(RELAY_PIN, LOW);}
  else if (sensormode==1 && automaticmode==1){ digitalWrite(RELAY_PIN, LOW);}
    if (currentHour >= 0 && currentHour < 20) 
    {
      if (currentMillis - previousMillis >= interval) 
      {
        previousMillis = currentMillis;
        if (automaticmode==1 && manualmode==0) {waterplant1();}
      }
    }
  if (manualmode==1 && automaticmode==0) 
  {
    waterplant2();
  }
   

  long long value = analogRead(SOIL_SENSOR_PIN);
  value = map(value,0,2000,0,100);
  if (sensormode==1)
  {
    if(value >40){plant3();}
    if(value<20){digitalWrite(RELAY_PIN,LOW);}
  }
  char buffet[20];  
snprintf(buffet, sizeof(buffet), "%02d:%02d:%02d", currentHour, currentMinute, currentSecond);

  // Display the current time on the LCD
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.setCursor(6, 0);
  lcd.print(buffet);
  lcd.setCursor(0, 1); 
  lcd.print("M: "); 
  lcd.print(value);
delay(1000); // Update every second
}
